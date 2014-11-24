#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "jirobaldo.h"

void carregarTexturasJirobaldo(SDL_Renderer *screen, Jirobaldo *jirobaldo){
	jirobaldo->face = FACE_SOUTH;
	jirobaldo->frame = 0;
	jirobaldo->isAnimating = false;

	SDL_Surface *tmp;

	tmp = IMG_Load("data/images/jirobaldo.png");
	jirobaldo->texture = SDL_CreateTextureFromSurface(screen, tmp);

	int i, j;
	for(i = FACE_NORTH; i <= FACE_EAST; i++){
		for(j = 0; j < 9; j ++){
			//Tamanho da textura
			jirobaldo->jirobaldoRect[i][j].h = 64;
			jirobaldo->jirobaldoRect[i][j].w = 64;

			//Guarda no array todos os frames para as animações
			jirobaldo->jirobaldoRect[i][j].x = j*64; //x para as colunas
			jirobaldo->jirobaldoRect[i][j].y = i*64 + (8*64); //y para as linhas
		}
	}
	SDL_FreeSurface(tmp);
}	

void renderJirobaldo(SDL_Renderer *screen, Jirobaldo *jirobaldo, SDL_Rect aux){
	if(jirobaldo->isAnimating && jirobaldo->frame < 8){ 
		jirobaldo->frame = jirobaldo->frame + 1;
	}else{
		jirobaldo->frame = 0;
		jirobaldo->isAnimating = false;
	}
	SDL_Rect jiroRect;
	jiroRect = jirobaldo->jirobaldoRect[jirobaldo->face][jirobaldo->frame];

	SDL_RenderCopy(screen, jirobaldo->texture, &jiroRect, &aux);
}