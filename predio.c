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
        fclose(file);
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
                    predio->jirobaldo.z = i;
                    predio->jirobaldo.x = j;
                    predio->jirobaldo.y = k;
                }
            }
        }
    }
    
    predio->jirobaldo.sx = predio->jirobaldo.x;
    predio->jirobaldo.sy = predio->jirobaldo.y;
    predio->jirobaldo.sz = predio->jirobaldo.z;

    return true;
}

bool alocaPredio(Predio *predio){
    int i, j;
    predio->pisos = (Pavimento *) malloc(sizeof(Pavimento) * predio->altura);
    if(predio->pisos == NULL){
        return false;
    }

    predio->andares = (SDL_Texture *) malloc(sizeof(SDL_Texture**) * predio->altura);

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

void gerarTexturasAndares(SDL_Renderer *screen, Predio *predio, SDL_Rect aux){
    int i, j, k;
    char pos;
    SDL_Rect viewport;
    SDL_RenderGetViewport(screen, &viewport);
    int size = (predio->h >= predio->w) ? (predio->h) : (predio->w);
    for(i = 0; i < predio->altura; i++){
        predio->andares[i] = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888, 
            SDL_TEXTUREACCESS_TARGET, aux.w * size, aux.h * size);
        SDL_SetRenderTarget(screen, predio->andares[i]);
        for(j = 0; j < predio->h; j++){
            for(k = 0; k < predio->w; k++){
                pos = predio->pisos[i].pontos[j][k];
                aux.y = j * aux.h;
                aux.x = k * aux.h;
                if(pos == '.' || pos == 'J' || pos == 'F' || pos == 'T' || pos == 'S'){
                    SDL_RenderCopy(screen, predio->chao, &predio->chaoRect, &aux);
                }else if(pos == '#'){
                    SDL_RenderCopy(screen, predio->paredes, &predio->paredeRect, &aux);
                }else if(pos == 'U'){
                    SDL_RenderCopy(screen, predio->escadas, &predio->escadasRect[UP_STAIR], &aux);
                }else if(pos == 'D'){
                    SDL_RenderCopy(screen, predio->escadas, &predio->escadasRect[DOWN_STAIR], &aux);
                }else if(pos == 'E'){
                    SDL_RenderCopy(screen, predio->escadaCimaBaixo, &predio->escadasRect[UP_DOWN_STAIR], &aux);
                }

                if(pos == 'S'){
                    SDL_RenderCopy(screen, predio->saida, &predio->saidaRect, &aux);
                }else if(pos == 'T'){
                    SDL_RenderCopy(screen, predio->torneira, &predio->torneiraRect, &aux);
                }
            }
        }
        SDL_SetRenderTarget(screen, NULL);
    }
}

//Carrega uma textura
SDL_Texture *carregarTextura(SDL_Renderer *screen, SDL_Texture *texture, char *path, SDL_Rect *rect){
    SDL_Surface *tmp;
    tmp = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(screen, tmp);
    if(rect != NULL){
        rect->h = tmp->h;
        rect->w = tmp->w;
    }
    SDL_FreeSurface(tmp);
    return texture;
}

void carregarTexturasPredio(SDL_Renderer *screen, Predio *predio){
    predio->frame = 0;
    // SDL_Surface *tmp;

    //Torneira
    predio->torneira = carregarTextura(screen, predio->torneira, "data/images/Decor0.png", NULL);
    predio->torneiraRect.h = 16;
    predio->torneiraRect.w = 16;
    predio->torneiraRect.x = 0;
    predio->torneiraRect.y = 21*16;

    //Chão
    predio->chao = carregarTextura(screen, predio->chao, "data/images/Interior-Furniture.png", NULL);
    predio->chaoRect.w = 32;
    predio->chaoRect.h = 32;
    predio->chaoRect.x = 6*32;
    predio->chaoRect.y = 2*32;

    //Saida
    predio->saida = carregarTextura(screen, predio->saida, "data/images/Door0.png", NULL);
    predio->saidaRect.w = 16;
    predio->saidaRect.h = 16;
    predio->saidaRect.x = 0;
    predio->saidaRect.y = 16;

    //Fogo
    predio->fogo = carregarTextura(screen, predio->fogo, "data/images/flames.png", NULL);
    int i;
    for(i = 0; i < 12; i++){
        predio->fogoRect[i].w = 16;
        predio->fogoRect[i].h = 24;
        predio->fogoRect[i].x = i*16;
        predio->fogoRect[i].y = 0;
    }

    //Paredes
    predio->paredes = carregarTextura(screen, predio->paredes, "data/images/Wall.png", NULL);
    predio->paredeRect.w = 16;
    predio->paredeRect.h = 16;
    predio->paredeRect.x = 3*16;
    predio->paredeRect.y = 12*16;

    //Escadas
    predio->escadas = carregarTextura(screen, predio->escadas, "data/images/Tile.png", NULL);
    predio->escadasRect[UP_STAIR].w = 16;
    predio->escadasRect[UP_STAIR].h = 16;
    predio->escadasRect[UP_STAIR].x = 4*16;
    predio->escadasRect[UP_STAIR].y = 16;

    predio->escadasRect[DOWN_STAIR].w = 16;
    predio->escadasRect[DOWN_STAIR].h = 16;
    predio->escadasRect[DOWN_STAIR].x = 5*16;
    predio->escadasRect[DOWN_STAIR].y = 16;

    //Escada pra cima e pra baixo
    predio->escadaCimaBaixo = carregarTextura(screen, predio->escadaCimaBaixo, "data/images/stair-up-down.jpg", NULL);
    predio->escadasRect[UP_DOWN_STAIR].w = 16;
    predio->escadasRect[UP_DOWN_STAIR].h = 16;
    predio->escadasRect[UP_DOWN_STAIR].x = 0;
    predio->escadasRect[UP_DOWN_STAIR].y = 0;

    predio->fumaca = carregarTextura(screen, predio->fumaca, "data/images/fog01.png", &predio->fumacaRect);
    predio->fumacaRect.x = 0;
    predio->fumacaRect.y = 0;
}

void renderAndarPredio(SDL_Renderer *screen, Predio *predio, int andar, SDL_Rect aux){
    int i, j;
    bool isJirobaldo;
    predio->frame++;
    SDL_Rect andarRect;

    //Renderiza o andar
    andarRect.h = aux.h * ((predio->h >= predio->w) ? predio->h : predio->w);
    andarRect.w = andarRect.h;
    andarRect.x = 0;
    andarRect.y = 0;
    SDL_SetTextureColorMod(predio->andares[andar], 100, 100, 100);
    SDL_RenderCopy(screen, predio->andares[andar], NULL, &andarRect);

    //Renderiza o fogo
    for(i = 0; i < predio->h; i++){
        for(j = 0; j < predio->w; j++){
            isJirobaldo = false;
            aux.y = i * aux.h;
            aux.x = j * aux.h;
            char c = predio->pisos[andar].pontos[i][j];

            if(predio->jirobaldo.x == i && predio->jirobaldo.y == j && predio->jirobaldo.z == andar){
                isJirobaldo = true;
            }

            if(c == 'F' && !isJirobaldo && predio->jirobaldo.z == andar){
                SDL_SetTextureColorMod(predio->chao, 180, 100, 100);
                SDL_RenderCopy(screen, predio->chao, &predio->chaoRect, &aux);
                SDL_RenderCopy(screen, predio->fogo, &predio->fogoRect[predio->frame % 12], &aux);
            }
        }
    }

    if(predio->jirobaldo.z == andar){
        //Renderiza o Jirobaldo
        aux.x = predio->jirobaldo.y * aux.h;
        aux.y = predio->jirobaldo.x * aux.h;
        if(predio->jirobaldo.isAnimating){
            int face = predio->jirobaldo.face;
            int dist = aux.h/8; //n pixels que o jirobaldo anda a cada frame
            if(face == FACE_NORTH || face == FACE_SOUTH){
                face == FACE_NORTH ? (aux .y = aux.y + aux.h - (predio->jirobaldo.frame * dist)) 
                    : (aux.y = aux.y - aux.h + (predio->jirobaldo.frame * dist));
            }else{
                // aux.x = (aux.x - aux.h);
                face == FACE_WEST ? (aux.x = aux.x + aux.h - (predio->jirobaldo.frame * dist))
                    : (aux.x = aux.x - aux.h + (predio->jirobaldo.frame * dist));
            }
        }
        renderJirobaldo(screen, &predio->jirobaldo, aux);

        //Renderiza a fumaça
        SDL_RenderGetViewport(screen, &aux);
        int offset = -predio->frame;
        if(offset < -aux.w){
            predio->frame = 0;
            offset = 0;
        }
        aux.x = offset;
        aux.y = andarRect.h - aux.h;
        SDL_SetTextureColorMod(predio->fumaca, 20, 20, 20);
        SDL_SetTextureAlphaMod(predio->fumaca, 200);
        SDL_RenderCopy(screen, predio->fumaca, &predio->fumacaRect, &aux);
        aux.x = offset + aux.w;
        SDL_RenderCopy(screen, predio->fumaca, &predio->fumacaRect, &aux);

        SDL_Rect mask;
        mask.w = aux.w - andarRect.w;
        mask.h = aux.h;
        mask.x = andarRect.w;
        mask.y = 0;
        SDL_RenderFillRect(screen, &mask);
    }
}

bool isPontoNoAndar(Pavimento *pavimento, int x, int y){
    return (x >= 0) && (x < pavimento->h) && (y >= 0) && (y < pavimento->w);
}

bool isPontoNoPredio(Predio *predio, int x, int y, int z){
    return (z >= 0) && (z < predio->altura) && isPontoNoAndar(&predio->pisos[z], x, y);
}