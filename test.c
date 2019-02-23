#include <stdio.h>
#include <fcntl.h> // open() 
#include <unistd.h> // read() & _exit() & pipe() 
#include <stdlib.h> // malloc()
#include <string.h> // memcpy()

typedef struct comando{

	char* comentario; 
	char* nome; 
	char** argumentos;

	// char* output; (depois vemos se conseguimos)
} *p_comandos;


char** splitBySpace(const char *cmd){
	
	int i, v = 0, len;
	int count = 1; // contador de comandos (pelo menos: o primeiro comando e o NULL)
	
	for(len = 0; cmd[len]; len++);
	char *r = malloc( sizeof(char) * len );
	memcpy(r, cmd, len * sizeof(char) ); 
	
	for(i = 0; r[i]; i++) // contador do número de argumentos dados
		if (r[i] == ' ') 
			count++;

	char **strList = malloc( sizeof(char*) * count);
	strList[v++] = r;
	strList[count - 1] = (char*) NULL;

	for (i = 0; r[i]; i++)
		if (r[i] == ' '){
			r[i] = '\0'; // sem esta linha cada apontador mostrava a string toda desde o inicio
			strList[v++] = r + i + 1; 
		}

	return strList;
}


void aumenta_buffer(void* buf, int *dim){

	void* novo_buf = malloc(*dim * 2 + 1); // iniciliza o buffer novo com o dobro do tamanho

	if ( memcpy(novo_buf, buf, *dim) == NULL){ // copia os bytes do buffer "antigo" para o novo
		perror("Erro na cópia do buffer: memcpy()");
		_exit(1);	
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

void readFileToBuffer(void* buf, int* bufSize, const char* path){

	int fd; // descritor de ficheiro
	ssize_t n; // número de bytes lidos na função read()
	int k = 0;

	if ( (fd = open(path, O_RDONLY)) == -1){ // abre e atribui o descritor de ficheiro do ficheiro dado como argumento do executável
 		perror("Erro ao abrir ficheiro: open()");
 		_exit(1);
 	}

	while( (n = read(fd, buf + k, *bufSize)) > 0 ){ // coloca no buffer (buf) os bytes lidos do ficheiro
 		k += n;
 		if (n == *bufSize){
 			aumenta_buffer(buf, bufSize);
 		}
	}

	if ( n == -1 ){ 
 		perror("Erro ao ler ficheiro: read()");
 		_exit(1);
 	}	

	*( (char*)buf + k) = '\0'; // para garantir que no final acaba em '\0' 
}

int main(int argc, char const *argv[]){

	int bufSize = 1;
	void* buf = malloc(bufSize);
	int lineDim, k;
	 
	if (argc < 2){
		perror("Número de argumentos insuficientes");
		_exit(1);
	}

 	readFileToBuffer(buf, &bufSize, argv[1]);	

 	printf("buf=%s\n", buf);

 	p_comandos comandos = (p_comandos) malloc()










}