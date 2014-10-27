#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "predio.h"

bool novoPredio(Predio *predio, char *arquivo){
    FILE *file;
    if((file = fopen(arquivo, "r")) != NULL){
        fscanf(file, "%d %d %d %d", &predio->altura, &predio->w, 
            &predio->h, &predio->jirobaldo.MAX_BALDES);
        if(lePredio(predio, file)){
            return true;
        }
    }
    return false;
}

bool lePredio(Predio *predio, FILE *file){
    int i, j, k;
    if(!alocaPredio(predio)){
        return false;
    }

    for(i = 0; i < predio->altura; i++){
        for(j = 0; j < predio->h; j++){
            for(k = 0; k < predio->w; k++){
                fscanf(file, " %c", &predio->pisos[i].pontos[j][k]);
                if(predio->pisos[i].pontos[j][k] == 'J'){
                    predio->jirobaldo.x = j;
                    predio->jirobaldo.y = k;
                    predio->jirobaldo.z = i;
                }
            }
        }
    }

    return true;
}

bool alocaPredio(Predio *predio){
    int i, j;
    predio->pisos = (Pavimento *) malloc(sizeof(Pavimento) * predio->altura);
    if(predio->pisos == NULL){
        return false;
    }

    for(i = 0; i < predio->altura; i++){
        predio->pisos[i].pontos = (char **) malloc(sizeof(char*) * predio->h);
        predio->pisos[i].w = predio->w;
        predio->pisos[i].h = predio->h;
        if(predio->pisos[i].pontos == NULL){
            return false;
        }
        for(j = 0; j < predio->h; j++){
            predio->pisos[i].pontos[j] = (char *) malloc(sizeof(char) * predio->w);
            if(predio->pisos[i].pontos[j] == NULL){
                return false;
            }
        }
    }
    return true;
}

void carregarTexturasPredio(SDL_Renderer *screen, Predio *predio){
    predio->frame = 0;
    SDL_Surface *tmp;

    //Torneira
    tmp = IMG_Load("data/DawnLike_3/Objects/Decor0.png");
    predio->torneira = SDL_CreateTextureFromSurface(screen, tmp);
    predio->torneiraRect.h = 16;
    predio->torneiraRect.w = 16;

    //Chão e saída
    tmp = IMG_Load("data/skorpios-pack/Interior-Furniture.png");
    predio->chaoSaida = SDL_CreateTextureFromSurface(screen, tmp);
    predio->chaoSaidaRect[PREDIO_CHAO].w = 32;
    predio->chaoSaidaRect[PREDIO_CHAO].h = 32;
    predio->chaoSaidaRect[PREDIO_CHAO].x = 6*32;
    predio->chaoSaidaRect[PREDIO_CHAO].y = 2*32;

    predio->chaoSaidaRect[PREDIO_SAIDA].w = 32;
    predio->chaoSaidaRect[PREDIO_SAIDA].h = 32;
    predio->chaoSaidaRect[PREDIO_SAIDA].x = 7*32;
    predio->chaoSaidaRect[PREDIO_SAIDA].y = 2*32;
    SDL_FreeSurface(tmp);

    //Fogo
    tmp = IMG_Load("data/skorpios-pack/CampFireFinished.png");
    predio->fogo = SDL_CreateTextureFromSurface(screen, tmp);
    int i;
    for(i = 0; i < 5; i++){
        predio->fogoRect[i].w = 64;
        predio->fogoRect[i].h = 64;
        predio->fogoRect[i].x = i*64;
        predio->fogoRect[i].y = 0;
    }
    SDL_FreeSurface(tmp);

    //Paredes
    tmp = IMG_Load("data/DawnLike_3/Objects/Pit0.png");
    predio->paredes = SDL_CreateTextureFromSurface(screen, tmp);
    predio->paredesRect[LEFT_WALL].w = 16;
    predio->paredesRect[LEFT_WALL].h = 16;
    predio->paredesRect[LEFT_WALL].x = 0;
    predio->paredesRect[LEFT_WALL].y = 3*16;

    predio->paredesRect[LEFT_TOP_WALL].w = 16;
    predio->paredesRect[LEFT_TOP_WALL].h = 16;
    predio->paredesRect[LEFT_TOP_WALL].x = 0;
    predio->paredesRect[LEFT_TOP_WALL].y = 2*16;

    predio->paredesRect[TOP_WALL].w = 16;
    predio->paredesRect[TOP_WALL].h = 16;
    predio->paredesRect[TOP_WALL].x = 16;
    predio->paredesRect[TOP_WALL].y = 2*16;

    predio->paredesRect[RIGHT_TOP_WALL].w = 16;
    predio->paredesRect[RIGHT_TOP_WALL].h = 16;
    predio->paredesRect[RIGHT_TOP_WALL].x = 2*16;
    predio->paredesRect[RIGHT_TOP_WALL].y = 2*16;

    predio->paredesRect[RIGHT_WALL].w = 16;
    predio->paredesRect[RIGHT_WALL].h = 16;
    predio->paredesRect[RIGHT_WALL].x = 2*16;
    predio->paredesRect[RIGHT_WALL].y = 3*16;
    SDL_FreeSurface(tmp);
}