#ifndef PREDIO_H_INCLUDED
#define PREDIO_H_INCLUDED

#include "jirobaldo.h"
//Definições para os retângulos das texturas
#define PREDIO_CHAO 0
#define PREDIO_SAIDA 1
#define UP_STAIR 0
#define DOWN_STAIR 1

typedef struct{
	char **pontos;
	int w, h;
}Pavimento;

typedef struct{
	Pavimento *pisos;
	int altura, w, h;
	Jirobaldo jirobaldo;
	int frame; //para animar o fogo
	SDL_Texture *chaoSaida, *paredes, *fogo, *escadas, *torneira, *upArrow;
	SDL_Texture **andares; //Texturas que guardam todo o andar
	SDL_Rect chaoSaidaRect[2], paredeRect, fogoRect[5], escadasRect[2], torneiraRect, upArrowRect;
}Predio;

//Inicia o predio, retorna false caso tenha ocorrido algum erro
bool novoPredio(Predio *predio, char *arquivo);
//Aloca predio na memória, retorna false caso tenha ocorrido um erro
bool alocaPredio(Predio *predio);
//Lê o arquivo que contém o prédio
bool lePredio(Predio *predio, FILE *file);
//Gera uma textura para cada andar, de forma que não é necessário ficar renderizando toda hora
void gerarTexturasAndares(SDL_Renderer *screen, Predio *predio, SDL_Rect aux);
//Carrega as texturas do predio
void carregarTexturasPredio(SDL_Renderer *screen, Predio *predio);
//Retorna true se o ponto está no andar
bool isPontoNoAndar(Pavimento *pavimento, int x, int y);
//Retorna true caso o ponto esteja no prédio, false caso contrário
bool isPontoNoPredio(Predio *predio, int x, int y, int z);
//Mostra, graficamente, um andar do prédio
void renderAndarPredio(SDL_Renderer *screen, Predio *predio, int andar, SDL_Rect aux);

#endif