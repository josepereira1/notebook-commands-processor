#ifndef COMANDO_H
#define COMANDO_H

typedef struct TCD_COMANDOS *TAD_COMANDOS;

typedef struct TCD_COMANDO *TAD_COMANDO;

//API:
 
TAD_COMANDOS init();
void add(char* comentario, char* nome, char** argumentos, int encadeado, int num, TAD_COMANDOS comandosStruct);
int getDim(TAD_COMANDOS comandos);
void printTADComandos(TAD_COMANDOS comandosStruct);
int getOcupados(TAD_COMANDOS comandos);
TAD_COMANDO getComandos(TAD_COMANDOS comandos, int index);
char* getComentario(TAD_COMANDOS comandos, int index);
char* getNome(TAD_COMANDOS comandos, int index);
char** getArgumentos(TAD_COMANDOS comandos, int index);
int getEncadeado(TAD_COMANDOS comandos, int index);
int getNum(TAD_COMANDOS comandos, int index);

#endif