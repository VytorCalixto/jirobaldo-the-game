/*
 * Trabalho 3 de Oficina de Programação
 * Vytor S.B. Calixto
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "predio.h"
#include "global.h"

//Lê os parâmetros passados na linha de comando
void lerParametros(int argc, char **argv);
bool iniciaSDL();
//Libera da memória as texturas, janelas, renderers e termina o SDL
void fimJogo();
//Lida com os eventos de teclado durante o jogo
void validaEventos(SDL_Event event);

int main(int argc, char **argv){
    bool quit = false;
    SDL_Event event;
    SDL_Rect aux;
    SDL_Surface *tmp;

    //Lê os parâmetros da linha de comando
    lerParametros(argc, argv);

    //Lê o arquivo do prédio
    if(arquivo == NULL){
        puts("ERRO: Nenhum arquivo foi especificado.");
        return 1;
    }

    if(!novoPredio(&predio, arquivo)){
        puts("ERRO: Não foi possível criar o prédio. O arquivo é válido?");
        return 1;
    }
    
    //Inicia SDL
    if(!iniciaSDL()){
        puts("ERRO: não foi possível iniciar o SDL");
        return 1;
    }

    window = SDL_CreateWindow("Jirobaldo: Sobrevivente", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Splash screen e carregar texturas
    carregarTexturasPredio(screen, &predio);
    carregarTexturasJirobaldo(screen, &predio.jirobaldo);
    titleFont = TTF_OpenFont("data/fonts/Plane-Crash.ttf", 48);
    char baldes[20];
    sprintf(baldes, "baldes: %d/%d", predio.jirobaldo.baldes, predio.jirobaldo.MAX_BALDES);
    tmp = TTF_RenderUTF8_Solid(titleFont, baldes, (SDL_Color) {200,200,200});
    baldesText = SDL_CreateTextureFromSurface(screen, tmp);
    baldesRect.w = tmp->w;
    baldesRect.h = tmp->h;
    baldesRect.x = 0;
    baldesRect.y = 0;
    SDL_FreeSurface(tmp);

    //Se for modo resolvedor gera a solução
    // TODO: gerar solução

    /*
     * Loop princpial:
     *     Render do mapa
     *     Lida com os eventos:
     *         Se for simulador lida com a jogabilidade
     *         Se for resolvedor, avança/retrocede um passo
     */
    while(!quit){
        SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
        SDL_RenderClear(screen);

        //Renderiza a barra de status
        aux.h = INFO_BAR_HEIGHT;
        aux.w = baldesRect.w/2;
        aux.x = 0;
        aux.y = 0;
        SDL_RenderSetViewport(screen, &infoBarViewport);
        SDL_RenderCopy(screen, baldesText, &baldesRect, &aux);
        
        //Renderiza o 'jogo' principal
        aux.h = gameViewport.h/predio.h;
        aux.w = aux.h;
        SDL_RenderSetViewport(screen, &gameViewport);
        renderAndarPredio(screen, &predio, predio.jirobaldo.z, aux);

        //Renderiza os mini-mapas
        if(predio.jirobaldo.z-1 >= 0){
            aux.h = downMapViewport.h/predio.h;
            aux.w = aux.h;
            SDL_RenderSetViewport(screen, &downMapViewport);
            renderAndarPredio(screen, &predio, predio.jirobaldo.z - 1, aux);
        }

        if(predio.jirobaldo.z+1 < predio.altura){
            aux.h = topMapViewport.h/predio.h;
            aux.w = aux.h;
            SDL_RenderSetViewport(screen, &topMapViewport);
            renderAndarPredio(screen, &predio, predio.jirobaldo.z + 1, aux);
        }

        SDL_RenderPresent(screen);

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = true;
            }else{
                //Valida eventos
                validaEventos(event);

            }
        }
        SDL_Delay(1000/30);
    }

    fimJogo();
    return 0;
}

void lerParametros(int argc, char **argv){
    int c;
    while((c = getopt(argc, argv, "f:s")) != -1){
        switch(c){
            case 'f':
            passosPorSegundo = atoi(optarg);
            break;
            case 's':
            isModoResolvedor = true;
            break;
        }
    }

    if(optind < argc){
        arquivo = argv[optind];
    }
}

bool iniciaSDL(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return false;
    }
    if(!IMG_Init(IMG_INIT_PNG)){
        return false;
    }
    if(TTF_Init() < 0){
        return false;
    }

    //Define as viewports
    infoBarViewport.x = 0;
    infoBarViewport.y = 0;
    infoBarViewport.w = SCREEN_WIDTH;
    infoBarViewport.h = INFO_BAR_HEIGHT;

    gameViewport.x = 0;
    gameViewport.y = INFO_BAR_HEIGHT;
    gameViewport.w = SCREEN_WIDTH - (SCREEN_WIDTH*0.2);
    gameViewport.h = SCREEN_HEIGHT - INFO_BAR_HEIGHT;

    topMapViewport.x = SCREEN_WIDTH - (SCREEN_WIDTH*0.2);
    topMapViewport.y = INFO_BAR_HEIGHT;
    topMapViewport.w = (SCREEN_WIDTH*0.2);
    topMapViewport.h = (SCREEN_WIDTH*0.2);

    downMapViewport.x = SCREEN_WIDTH - (SCREEN_WIDTH*0.2);
    downMapViewport.y = INFO_BAR_HEIGHT + (SCREEN_WIDTH*0.2);
    downMapViewport.w = (SCREEN_WIDTH*0.2);
    downMapViewport.h = (SCREEN_WIDTH*0.2);
    return true;
}

void fimJogo(){
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void validaEventos(SDL_Event event){
    if(isModoResolvedor && passosPorSegundo == 0){
        //TODO: Eventos para o modo resolvedor
    }else{
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_LEFT:
                    predio.jirobaldo.y--;
                    if(predio.jirobaldo.y < 0){
                        predio.jirobaldo.y++;
                    }
                    break;
                case SDLK_RIGHT:
                    predio.jirobaldo.y++;
                    if(predio.jirobaldo.y >= predio.w){
                        predio.jirobaldo.y--;
                    }
                    break;
                case SDLK_UP:
                    predio.jirobaldo.x--;
                    if(predio.jirobaldo.x < 0){
                        predio.jirobaldo.x++;
                    }
                    break;
                case SDLK_DOWN:
                    predio.jirobaldo.x++;
                    if(predio.jirobaldo.x >= predio.h){
                        predio.jirobaldo.x--;
                    }
                    break;
            }
        }
    }
}