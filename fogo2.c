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

int main(int argc, char **argv){
    bool quit = false;
    SDL_Event event;
    SDL_Rect aux;

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


    //Se for modo resolvedor gera a solução
    // TODO: gerar solução

    /*
     * Loop princpial:
     *     Render do mapa
     *     Lida com os eventos:
     *         Se for simulador lida com a jogabilidade
     *         Se for resolvedor, avança/retrocede um passo
     */
    aux.h = gameViewport.h/predio.h;
    aux.w = aux.h;
    while(!quit){
        SDL_RenderSetViewport(screen, &gameViewport);
        SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
        SDL_RenderClear(screen);
        renderAndarPredio(screen, &predio, predio.jirobaldo.z, aux);
        SDL_RenderPresent(screen);

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = true;
            }else{
                //Valida eventos
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
    infoBarViewport.x = 0;
    infoBarViewport.y = 0;
    infoBarViewport.w = SCREEN_WIDTH;
    infoBarViewport.h = INFO_BAR_HEIGHT;

    gameViewport.x = 0;
    gameViewport.y = INFO_BAR_HEIGHT;
    gameViewport.w = (int) SCREEN_WIDTH - (SCREEN_WIDTH*0.2);
    gameViewport.h = (int) SCREEN_HEIGHT - INFO_BAR_HEIGHT;

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