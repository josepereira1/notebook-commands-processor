#include <stdio.h>
#include <fcntl.h> // open() 
#include <unistd.h> // _exit()
#include <stdlib.h> // malloc()
#include <string.h> // memcpy()
#include "comando.h"

/**
@file main.c
Módulo principal do programa.
*/

#define ERRO_ARGUMENTOS_INSUFICIENTES		1
#define ERRO_ABRIR_FICHEIRO					2
#define ERRO_LER_FICHEIRO					3
#define ERRO_MEMCPY							4
#define ERRO_FORK							5
#define ERRO_EXECVP							6

#define KB								 1024
#define MB						    1024 * KB

//#define BUFFER_SIZE					  10 * MB
#define BUFFER_DIM				  			1

static void copyFile(const char* pathF, const char* pathI);
static void aumenta_buffer(void* buf, int *dim);
static void readFileToBuffer(void* buf, int* bufSize, const char* path);
static int lengthLnBuffer(void *buf, int dim, char ch);
void strToList(const char *cmd, char* args[20][30], int x);
void execEncadeada(TAD_COMANDOS comandos, int argx2, int path);
TAD_COMANDOS parser(void* buf, int bufSize);

void interrupt(){

	printf("programa foi interrompido pelo utilizador\n");
	_exit(SIGINT);
}

int main(int argc, char const *argv[]){


    signal(SIGINT, interrupt);

	int bufSize = BUFFER_DIM;
	void* buf = malloc(bufSize);
	int max;
    int fd1 = open("extrafile.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    int x2 = 0;
	dup2(0,1); // guarda o descritor de leitura no de leitura
	 
	if (argc < 2){
		perror("Número de argumentos insuficientes");
		_exit(ERRO_ARGUMENTOS_INSUFICIENTES);
	}

 	readFileToBuffer(buf, &bufSize, argv[1]);

 	//printf("\nbuf = \n%s\n\n", buf); // DEBUGGING

 	TAD_COMANDOS comandos = parser(buf, bufSize);

	printTADComandos(comandos);

    max = getOcupados(comandos);

    for(x2 = 0;x2 < max; x2++){
    	execEncadeada(comandos,x2,fd1);
    }
    copyFile( argv[1],"extrafile.txt");
    return 0;
}

/*
\brief Copia o contúdo do ficheiro inicial para o ficheiro final.
@param pathF - ficheiro final
@param pathI - ficheiro inicial
*/
static void copyFile(const char* ex, const char* extra){
    
    pid_t p;

    p = fork();
 
 	if (p == 0){
 		execlp("rm", "lixo", ex, (char*) NULL);
 	}

 	else {

 		p = fork();

 		if (p == 0){
 			execlp("mv", "lixo", extra, ex, (char*) NULL);
 		}
 	}  
}

/**
\brief Re-dimensiona o tamanho do buffer para o dobro. 
@param buf - buffer
@param dim - tamanho atual do buffer
*/
static void aumenta_buffer(void* buf, int *dim){

	void* novo_buf = malloc(*dim * 2 + 1); // iniciliza o buffer novo com o dobro do tamanho

	if ( memcpy(novo_buf, buf, *dim) == NULL){ // copia os bytes do buffer "antigo" para o novo
		perror("Erro na cópia do buffer: memcpy()");
		_exit(ERRO_MEMCPY);	
	}

	/*
	if (buf){
		free(buf);
		buf = NULL;
	}
	*/

	buf = novo_buf;
	
	*dim = *dim * 2; 
}


/**
\brief Indica uma linha do buffer.
@param buf - buffer
@param dim - tamanho atual do buffer
@return retorna a dimensão da linha
*/
static int lengthLnBuffer(void *buf, int dim, char ch){

	int i;

	for (i = 0; i < dim; i++)
		if ( *((char *) buf + i) == ch || *((char *) buf + i) == '\0') 
			break;
	
	return i;
}


/**
\brief Copia os bytes em ficheiro para o buffer.
@param buf - buffer
@param bufSize - tamanho atual do buffer
@param path - caminho para o ficheiro
*/
static void readFileToBuffer(void* buf, int* bufSize, const char* path){

	int fd; // descritor de ficheiro
	ssize_t n; // número de bytes lidos na função read()
	int k = 0;

	if ( (fd = open(path, O_RDONLY)) == -1){ // abre e atribui o descritor de ficheiro do ficheiro dado como argumento do executável
 		perror("Erro ao abrir ficheiro: open()");
 		_exit(ERRO_ABRIR_FICHEIRO);
 	}

	for(k = 0; (n = read(fd, buf + k, *bufSize)) > 0; k += n) // coloca no buffer (buf) os bytes lidos do ficheiro
 		if (n == *bufSize)
 			aumenta_buffer(buf, bufSize);

	if ( n == -1 ){ 
 		perror("Erro ao ler ficheiro: read()");
 		_exit(ERRO_LER_FICHEIRO);
 	}	

	*( (char*)buf + k) = '\0'; // para garantir que no final acaba em '\0' 
}




/**
\brief Converte uma string separada por espaços em uma array de strings.
@param cmd - string
@retrun retorna o array de strings.
*/
void strToList(const char *cmd, char* args[20][30], int x){
	
	int i, v = 0, len, init = 0;
	int count = 2; //contador de comandos (pelo menos: o primeiro comando e NULL)
	while(cmd[init] != ' ')init++;
	for(len = 0; cmd[len] != '\n' && cmd[len] != '\0'; len++);
	len = (len-init-1); // para garantir que começa onde deve sempre
	char *r = malloc( sizeof(char) * len );
	memcpy(r, cmd+init+1, len * sizeof(char) ); 
	
	for(i = 0; r[i]; i++) // contador do número de comandos e/ou argumentos dados
		if (r[i] == ' ') 
			count++;
    printf("%d\n",count );
	args[x][v] = r;
	args[x][count - 1] = (char*) NULL;

	for (i = 0; r[i]; i++)
		if (r[i] == ' '){
			r[i] = '\0'; // sem esta linha cada apontador mostrava a string toda desde o inicio
			args[x][v++] = r + i + 1; 
		}
}
void printIntro(TAD_COMANDOS comandos, int max, int argx2, int path){
	int x;
	char* inicioComentario = "\n>>>>>>\n";
	char* mudancaLinha = "\n";
	char* comandoSimples = "$";
	char* comandoEncadeado = "$|";
	char* barra = "|";
	char* espaco = " ";
	char numero[12];
	int i = 0;
	char** args = getArgumentos(comandos, argx2);
	write(path,getComentario(comandos,argx2),strlen(getComentario(comandos,argx2)) + 1);
	write(path,mudancaLinha,1);
	if(max == 0) write(path,comandoSimples,1);
	else if(getNum(comandos,argx2)){
		write(path,comandoSimples,1);
		sprintf(numero,"%d",getNum(comandos,argx2));
		while(numero[i] != '\0')i++;
		write(path,numero,i);
		write(path,barra,1);
	}
	else write(path,comandoEncadeado,2);
    for(x=0;args[x] != NULL;x++){
    	write(path,args[x],strlen(args[x])+1);
    	write(path,espaco,1);
    }
    write(path,inicioComentario,8);
}

int comandosEncadeados(TAD_COMANDOS comandos,int* args, int argx){
	int x = 0;
	while(getEncadeado(comandos,argx)){
		args[x] = argx;
		if(getNum(comandos,argx)) argx -= getNum(comandos,argx);
		else argx--;
		x++;
		if(argx < 0){
			perror("Erro, argumento inválido.");
		}
	}
	args[x] = argx;
	return x;
}

void execEncadeada(TAD_COMANDOS comandos, int argx2, int path){
	pid_t pid;
	int fd[2];
	int x,status;
	char* fimComentario = "<<<<<<\n\n";
	int args[getOcupados(comandos)];
    int max = comandosEncadeados(comandos,args,argx2);
    for(int i = 0; i < max; i++)printf("%d\n", args[i]);
    printIntro(comandos,max,argx2,path);
   	for( x = max ; x >= 0 ; x--){ // o pai cria um filho que escreve num pipe anónimo
                                  // como reiniciar no inicio do pipe anónimo
      pipe(fd);
      if((pid = fork()) == 0){
         if(x == max && x > 0){ // caso inicial escreve no pipe

            close(STDOUT_FILENO);     // fecha descritor de saida
            dup(fd[1]);               // substitui descritor de saida apagado
            close(fd[0]);
            close(fd[1]);
            execvp(getNome(comandos,args[x]),getArgumentos(comandos,args[x]));
            _exit(1);
         }
         if(x == 0){
         	dup2(path,1);
            execvp(getNome(comandos,args[x]),getArgumentos(comandos,args[x]));
         }
      }
      else{
         close(fd[1]);
         dup2(fd[0],0);
         if(x == 0){
         	waitpid(pid,&status,WUNTRACED);
         	dup2(path,1);
            write(1,fimComentario,8);
         }
      }
   }
}


/**
\brief Converte uma string separada por espaços num array de strings.
@param r - string
@return retorna o array de strings.
*/
char** splitBySpace(char *r){
	
	int i, v = 0;
	int count = 2; // contador de comandos (pelo menos: o primeiro comando e o NULL)

	for(i = 0; r[i]; i++) // contador do número de argumentos dados
		if (r[i] == ' ') 
			count++;

	char **strList = malloc( sizeof(char*) * count);

	strList[v++] = r;

	if (count == 2){
		strList[1] = (char*) NULL;
		return strList;
	}

	strList[count-1] = (char*) NULL;

	for (i = 0; r[i]; i++)
		if (r[i] == ' '){
			r[i] = '\0'; // sem esta linha cada apontador mostrava a string toda desde o inicio
			strList[v++] = r + i + 1; 
		}

	return strList;
}


TAD_COMANDOS parser(void* buf, int bufSize){
	
	TAD_COMANDOS comandos = init(); // cria-se a estrutura
	
	char* comentario = NULL, **argumentos = NULL, *number;
	int encadeado = 0;
	int i, dim = 0,num = 0, numlen = 0;

	for(i = 0; ((char*)buf)[i]; i += dim + 1,num = 0, numlen = 0){ // percorre o buffer

		dim = lengthLnBuffer(buf + i, bufSize, '\n'); // localiza uma linha
		
		if ( ((char*)buf)[i] == '$'){

			char* tmp;

			if (((char*)buf)[i+1] != '|'){
				if(((char*)buf)[i+1] != ' '){
					while(((char*)buf)[i+1] != '|'){
						num++;i++;
					} i -= num;
					numlen = num + 1;// conta com o '|'
					number = malloc(sizeof(char) * num + 1);
					memcpy(number,buf+i+1,num+1);
					number[num] = '\0';
					num = atoi(number);
					free(number);
					encadeado = 1;
				}
				else encadeado = 0;
				tmp = malloc(sizeof(char) * dim-numlen - 2); // -2 por causa do '$' e do ' ' e no lugar do '\n' passa a estar o '\0'
				memcpy(tmp, buf + i +numlen + 2, sizeof(char) * dim-numlen-2);
				tmp[dim-numlen-2] = '\0'; // onde esta '\n' passa a estar '\0'
			} else{
				tmp = malloc(sizeof(char) * dim - 3); // -3 por causa do '$' e do '|' e do ' ' e no lugar do '\n' passa a estar o '\0'
				memcpy(tmp, buf + i + 3, sizeof(char) * dim-3);
				tmp[dim-3] = '\0'; // onde esta '\n' passa a estar '\0'
				encadeado = 1;
			}			
			
			argumentos = splitBySpace(tmp);
		}

		else if (((char*)buf)[i+dim-1] == ':'){ // comentário

			comentario = malloc(sizeof(char) * dim + 1);
			memcpy(comentario, buf + i, sizeof(char) * dim + 1);
			comentario[dim] = '\0'; // onde esta '\n' passa a estar '\0'
		}

		if (comentario && argumentos){

			add(comentario, argumentos[0], argumentos, encadeado, num, comandos);
			comentario = NULL;
			argumentos = NULL;
		}
	}

	return comandos;
}
