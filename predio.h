#ifndef PREDIO_H_INCLUDED
#define PREDIO_H_INCLUDED

#include "jirobaldo.h"
#define PREDIO_CHAO 0
#define PREDIO_SAIDA 0
#define LEFT_WALL 0
#define LEFT_TOP_WALL 1
#define TOP_WALL 2
#define RIGHT_TOP_WALL 3
#define RIGHT_WALL 4

typedef struct{
	char **pontos;
	int w, h;
}Pavimento;

typedef struct{
	Pavimento *pisos;
	int altura, w, h;
	Jirobaldo jirobaldo;
	int frame; //para animar o fogo
	SDL_Texture *chaoSaida, *paredes, *fogo, *escadas, *torneira;
	SDL_Rect chaoSaidaRect[2], paredesRect[5], fogoRect[5], escadasRect[3], torneiraRect;
}Predio;

//Inicia o predio, retorna false caso tenha ocorrido algum erro
bool novoPredio(Predio *predio, char *arquivo);
//Aloca predio na memória, retorna false caso tenha ocorrido um erro
bool alocaPredio(Predio *predio);
//Lê o arquivo que contém o prédio
bool lePredio(Predio *predio, FILE *file);
//Carrega as texturas do predio
void carregarTexturasPredio(SDL_Renderer *screen, Predio *predio);
//Retorna true se o ponto está no andar
bool isPontoNoAndar(Pavimento *pavimento, int x, int y);
//Retorna true caso o ponto esteja no prédio, false caso contrário
bool isPontoNoPredio(Predio *predio, int x, int y, int z);
//Mostra, graficamente, um andar do prédio
void renderAndarPredio(Pavimento *pavimento);


#endif