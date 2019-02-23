#include "comando.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
@file comando.c
Módulo principal do programa.
*/

/**\brief estrutura que armazena um comando */
typedef struct TCD_COMANDO{

	char* comentario;
	char* nome;
	char** argumentos;
	int encadeado;
	int num;

} TCD_COMANDO;

/**\brief estrutura que armazena todos os comandos */
typedef struct TCD_COMANDOS{

	int dim;
	int ocupados;
	TCD_COMANDO* comandos;

} TCD_COMANDOS;

/**\brief capacideade inicial da estrutura que armazena os comandos*/
static int capacidade_inicial = 50;

/**
\brief Aumenta a capacidade do array que armazena os comandos, re-dimensionando.
@param oldArray array que vai ser re-dimensionado
@param oldDim antiga dimensão do array
@param newDim nova dimensão do array 
@return retorna o novo array
*/
static TCD_COMANDO* aumentaCapacidade(TCD_COMANDO* oldArray, int oldDim, long newDim){
	TCD_COMANDO* newArray = malloc(newDim * sizeof(TCD_COMANDO));	
	memcpy(newArray, oldArray, oldDim * sizeof(TCD_COMANDO));
	free(oldArray);
	return newArray;
}

/**
\brief Cria e inicializa a estrutura que armazena os comandos.
@return retorna a estrutura
*/
TAD_COMANDOS init(){
	printf("a1\n");
	TAD_COMANDOS comandosStruct = malloc( sizeof(TCD_COMANDOS) );
	printf("a2\n");
	comandosStruct->dim = capacidade_inicial;
	printf("a3\n");
	comandosStruct->ocupados = 0;
	printf("a4\n");
	comandosStruct->comandos = malloc(sizeof(TCD_COMANDO) * comandosStruct->dim);
	printf("a5\n");
	return comandosStruct;
}

/**
\brief Cria e adiciona um comando á estrutura que armazena os comandos.
@param comentario comentário antecedido ao comando
@param nome nome do comando
@param argumentos argumentos do comando
@param encadeado indica se o comando é encadeado de outro
@param comandosStruct estrutura que armazena todos os comandos
*/
void add(char* comentario, char* nome, char** argumentos, int encadeado, int num,  TAD_COMANDOS comandosStruct){

	TCD_COMANDO* comando = malloc(sizeof(TCD_COMANDO));
	
	int len;
	for (len = 0; argumentos[len] != NULL; len++);
	comando->argumentos = malloc(sizeof(char*) * len + 1); // +1 para o NULL
	for(len = 0; argumentos[len]; len++){
		int strLen = strlen(argumentos[len]);
		comando->argumentos[len] = malloc(sizeof(char) * strLen + 1);
		memcpy(comando->argumentos[len], argumentos[len], strLen + 1);
		printf("%s\n",comando->argumentos[len]);
	}
	comando->argumentos[len] = (char*) NULL;
	int comentarioLen = strlen(comentario);
	comando->comentario = malloc(sizeof(char) * comentarioLen + 1);
	memcpy(comando->comentario, comentario, comentarioLen + 1);
	printf("%s\n",comando->comentario);
	int nomeLen = strlen(nome);
	comando->nome = malloc(sizeof(char) * nomeLen + 1);
	memcpy(comando->nome, nome, nomeLen + 1);
	printf("%s\n",comando->nome);
	comando->encadeado = encadeado;
	comando->num = num;
	printf("encadeado = %d,num = %d\n",comando->encadeado, comando->num);
	int dim = comandosStruct->dim;
	int ocupados = comandosStruct->ocupados;
	if (dim == ocupados){
		comandosStruct->comandos = aumentaCapacidade(comandosStruct->comandos, dim, dim * 2);
		comandosStruct->dim *= 2;
	}
	comandosStruct->comandos[comandosStruct->ocupados++] = *comando;
}

/**
\brief Devolve o número de comandos na estrutura.
@param comandosStruct estrutura que armazena todos os comandos
@return retorna o número de comandos
*/
int getOcupados(TAD_COMANDOS comandos){
	return comandos->ocupados;
}

TAD_COMANDO getComandos(TAD_COMANDOS comandos, int index){
	return &comandos->comandos[index];
}

char* getComentario(TAD_COMANDOS comandos, int index){
	return comandos->comandos[index].comentario;
}

char* getNome(TAD_COMANDOS comandos, int index){
	return comandos->comandos[index].nome;
}

char** getArgumentos(TAD_COMANDOS comandos, int index){
	return comandos->comandos[index].argumentos;
}

int getEncadeado(TAD_COMANDOS comandos, int index){
	return comandos->comandos[index].encadeado;
}

//	este num é para o requesito extra
int getNum(TAD_COMANDOS comandos, int index){
	return comandos->comandos[index].num;
}




 
// DEBUGGING ---------------------------------------------------------------------------------

void printTADComandos(TAD_COMANDOS comandosStruct){

	printf("TAD_COMANDOS{ \n\n");
	printf("     dim=%d\n", comandosStruct->dim);
	printf("     ocupados=%d\n\n", comandosStruct->ocupados);
	printf("     comandos={\n");
	TCD_COMANDO* comandos = comandosStruct->comandos;
	for(int i = 0; i < comandosStruct->ocupados; i++){
		printf("          TCD_COMANDO{ comentario=%s, nome=%s, argumentos=[", comandos[i].comentario, comandos[i].nome);
		for (int v = 0; comandos[i].argumentos[v]; v++) printf("%s, ", comandos[i].argumentos[v]);
		printf("], encadeado=%d, num=%d }\n", comandos[i].encadeado, comandos[i].num);
	}
	printf("     }\n");
	printf("}\n");
} 

/*
int main(){
	TAD_COMANDOS comandos = init();
	char* argumentos[] = {"arg1", "arg2", "arg3", (char*) NULL};
	
	add("comentario 1", "nome 1", argumentos, 0, comandos);
	add("comentario 2", "nome 2", argumentos, 1, comandos);
	add("comentario 3", "nome 3", argumentos, 1, comandos);
	printTADComandos(comandos);
	return 0;
}
*/