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
                if(pos == '.' || pos == 'J' || pos == 'F' || pos == 'T'){
                    SDL_SetTextureColorMod(predio->chaoSaida, 100, 100, 100);
                    SDL_RenderCopy(screen, predio->chaoSaida, &predio->chaoSaidaRect[PREDIO_CHAO], &aux);
                }else if(pos == '#'){
                    SDL_SetTextureColorMod(predio->paredes, 100, 100, 100);
                    SDL_RenderCopy(screen, predio->paredes, &predio->paredeRect, &aux);
                }else if(pos == 'U'){
                    SDL_RenderCopy(screen, predio->escadas, &predio->escadasRect[UP_STAIR], &aux);
                }else if(pos == 'D'){
                    SDL_RenderCopy(screen, predio->escadas, &predio->escadasRect[DOWN_STAIR], &aux);
                }else if(pos == 'E'){
                    SDL_RenderCopy(screen, predio->escadaCimaBaixo, &predio->escadasRect[UP_DOWN_STAIR], &aux);
                }else if(pos == 'S'){
                    SDL_RenderCopy(screen, predio->chaoSaida, &predio->chaoSaidaRect[PREDIO_SAIDA], &aux);
                }

                if(pos == 'T'){
                    SDL_RenderCopy(screen, predio->torneira, &predio->torneiraRect, &aux);
                }
            }
        }
        SDL_SetRenderTarget(screen, NULL);
    }
}

void carregarTexturasPredio(SDL_Renderer *screen, Predio *predio){
    predio->frame = 0;
    SDL_Surface *tmp;

    //Torneira
    tmp = IMG_Load("data/DawnLike_3/Objects/Decor0.png");
    predio->torneira = SDL_CreateTextureFromSurface(screen, tmp);
    predio->torneiraRect.h = 16;
    predio->torneiraRect.w = 16;
    predio->torneiraRect.x = 0;
    predio->torneiraRect.y = 21*16;
    SDL_FreeSurface(tmp);

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
    predio->chaoSaidaRect[PREDIO_SAIDA].y = 5*32;
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
    tmp = IMG_Load("data/DawnLike_3/Objects/Wall.png");
    predio->paredes = SDL_CreateTextureFromSurface(screen, tmp);
    predio->paredeRect.w = 16;
    predio->paredeRect.h = 16;
    predio->paredeRect.x = 3*16;
    predio->paredeRect.y = 12*16;
    SDL_FreeSurface(tmp);

    //Escadas
    tmp = IMG_Load("data/DawnLike_3/Objects/Tile.png");
    predio->escadas = SDL_CreateTextureFromSurface(screen, tmp);
    predio->escadasRect[UP_STAIR].w = 16;
    predio->escadasRect[UP_STAIR].h = 16;
    predio->escadasRect[UP_STAIR].x = 0;
    predio->escadasRect[UP_STAIR].y = 16;

    predio->escadasRect[DOWN_STAIR].w = 16;
    predio->escadasRect[DOWN_STAIR].h = 16;
    predio->escadasRect[DOWN_STAIR].x = 16;
    predio->escadasRect[DOWN_STAIR].y = 16;
    SDL_FreeSurface(tmp);

    //Escada pra cima e pra baixo
    tmp = IMG_Load("data/stair-up-down.jpg");
    predio->escadaCimaBaixo = SDL_CreateTextureFromSurface(screen, tmp);
    predio->escadasRect[UP_DOWN_STAIR].w = 16;
    predio->escadasRect[UP_DOWN_STAIR].h = 16;
    predio->escadasRect[UP_DOWN_STAIR].x = 0;
    predio->escadasRect[UP_DOWN_STAIR].y = 0;
    SDL_FreeSurface(tmp);
}

void renderAndarPredio(SDL_Renderer *screen, Predio *predio, int andar, SDL_Rect aux){
    //TODO: Iluminação "spot" (círculo ao redor do fogo) ou degradê nos vizinhos
    int i, j;
    bool isJirobaldo;
    predio->frame++;
    SDL_Rect andarRect;

    //Renderiza o andar
    andarRect.h = aux.h * ((predio->h >= predio->w) ? predio->h : predio->w);
    andarRect.w = andarRect.h;
    andarRect.x = 0;
    andarRect.y = 0;
    SDL_RenderCopy(screen, predio->andares[andar], NULL, &andarRect);

    //Renderiza o Jirobaldo
    if(predio->jirobaldo.z == andar){
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
    }

    for(i = 0; i < predio->h; i++){
        for(j = 0; j < predio->w; j++){
            isJirobaldo = false;
            aux.y = i * aux.h;
            aux.x = j * aux.h;
            char c = predio->pisos[andar].pontos[i][j];

            if(predio->jirobaldo.x == i && predio->jirobaldo.y == j && predio->jirobaldo.z == andar){
                isJirobaldo = true;
            }

            //Coisas que ficam na frente do Jirobaldo
            if(c == 'F' && !isJirobaldo && predio->jirobaldo.z == andar){
                SDL_RenderCopy(screen, predio->fogo, &predio->fogoRect[predio->frame % 5], &aux);
            }
        }
    }
}

bool isPontoNoAndar(Pavimento *pavimento, int x, int y){
    return (x >= 0) && (x < pavimento->h) && (y >= 0) && (y < pavimento->w);
}

bool isPontoNoPredio(Predio *predio, int x, int y, int z){
    return (z >= 0) && (z < predio->altura) && isPontoNoAndar(&predio->pisos[z], x, y);
}