#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "textures.c"
#include "global.c"

//Lê os parâmetros passados na invocação do programa
void lerOpcoes(int argc, char **argv);
//Inicia o SDL, retorna false caso tenha ocorrido algum erro
bool iniciarSdl();
//Abre uma splash screen e carrega texturas
void splashScreen();
//Lida com os eventos do jogo
void lidaComEventos(SDL_Event event);
//Retorna true se Jirobaldo está numa posição válida, false caso contrário
bool jirobaldoValido();

int main(int argc, char **argv){
    bool quit = false;
    SDL_Event event;

    lerOpcoes(argc, argv);
    // printf("pps: %d, mr: %d, pf: %s\n", passosPorSegundo, isModoResolvedor, arquivo);
    if(!novoPredio(&predio, arquivo)){
        puts("ERRO: Não foi possível abrir o arquivo especificado.");
        return 1;
    }
    if(!iniciarSdl()){
        puts("ERRO: Não foi possível inciar o SDL.");
        return 1;
    }
    splashScreen();
    //Loop principal
    while(!quit){
        SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
        SDL_RenderClear(screen);
        renderAndarPredio(&predio, predio.jirobaldo.z);
        SDL_RenderPresent(screen);
        
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                quit = true;
            }else{
                lidaComEventos(event);
            }
        }

        SDL_Delay(1000/30);
    }

    return 0;
}

void lerOpcoes(int argc, char **argv){
    int c;
    while((c =getopt(argc, argv, "f:s")) != -1){
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

bool iniciarSdl(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return false;
    } 
    if(!IMG_Init(IMG_INIT_PNG)){
        return false;
    }
    if(TTF_Init() < 0){
        return false;
    }
    return true;
}

void splashScreen(){
    window = SDL_CreateWindow("Jirobaldo: Survivor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *tmp;
    SDL_Rect titleRect, subtitleRect;
    TTF_Font *titleFont;
    int alpha = 0;

    titleFont = TTF_OpenFont("data/fonts/Plane-Crash.ttf", 48);
    tmp = TTF_RenderUTF8_Solid(titleFont, "jirobaldo",
        (SDL_Color) {180, 0, 0});
    titleText = SDL_CreateTextureFromSurface(screen, tmp);
    SDL_SetTextureAlphaMod(titleText, alpha);
    titleRect.w = tmp->w;
    titleRect.h = tmp->h;
    titleRect.x = SCREEN_WIDTH/2 - titleRect.w/2;
    titleRect.y = SCREEN_HEIGHT/2 - titleRect.h/2;
    SDL_FreeSurface(tmp);

    tmp = TTF_RenderUTF8_Solid(titleFont, "the survivor",
        (SDL_Color) {200, 200, 200});
    subtitleText = SDL_CreateTextureFromSurface(screen, tmp);
    SDL_SetTextureAlphaMod(subtitleText, 0);
    subtitleRect.w = tmp->w;
    subtitleRect.h = tmp->h;
    subtitleRect.x = SCREEN_WIDTH/2 - subtitleRect.w/2;
    subtitleRect.y = titleRect.y + titleRect.h;
    SDL_FreeSurface(tmp);

    while(alpha < 255){
        SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
        SDL_RenderClear(screen);
        // SDL_RenderCopy(screen, bg, NULL, NULL);
        SDL_RenderCopy(screen, titleText, NULL, &titleRect);
        SDL_RenderCopy(screen, subtitleText, NULL, &subtitleRect);
        SDL_RenderPresent(screen);
        
        alpha+=3;
        SDL_SetTextureAlphaMod(titleText, alpha);
        SDL_SetTextureAlphaMod(subtitleText, alpha);

        SDL_Delay(1000/30);
    }
    carregarTexturasPredio();
}

void lidaComEventos(SDL_Event event){
    if(event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
            case SDLK_LEFT:
                predio.jirobaldo.y--;
                if(!jirobaldoValido()){
                    predio.jirobaldo.y++;
                }
                break;
            case SDLK_RIGHT:
                predio.jirobaldo.y++;
                if(!jirobaldoValido()){
                    predio.jirobaldo.y--;
                }
                break;
            case SDLK_UP:
                predio.jirobaldo.x--;
                if(!jirobaldoValido()){
                    predio.jirobaldo.x++;
                }
                break;
            case SDLK_DOWN:
                predio.jirobaldo.x++;
                if(!jirobaldoValido()){
                    predio.jirobaldo.x--;
                }
                break;
        }
    }
}

bool jirobaldoValido(){
    int x = predio.jirobaldo.x, y = predio.jirobaldo.y, z = predio.jirobaldo.z;
    return isPontoNoPredio(&predio, x, y, z) && (predio.pisos[z].pontos[x][y] != '#');
}