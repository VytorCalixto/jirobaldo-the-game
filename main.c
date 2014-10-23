#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "predio.c"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

Predio predio;

//Carrega o SDL, retorna true se não ocorrerem erros durante a inicialização
bool iniciarSdl();
//Lê as opções dadas na invocação do programa
void lerOpcoes(int argc, char **argv, int *passosPorSegundo, bool *isModoResolvedor);
//Splash Screen
void splashScreen(SDL_Renderer *screen, bool isModoResolvedor);
//Operações ao finalizar o jogo
void fimJogo();

int main(int argc, char **argv){
    int passosPorSegundo = 0;
    bool isModoResolvedor = false;
    char *pathFile;

    if(!iniciarSdl()){
        return 1;
    }
    lerOpcoes(argc, argv, &passosPorSegundo, &isModoResolvedor);
    if(optind < argc){
        pathFile = argv[optind];
    }
    // printf("pps: %d, mr: %d, pf: %s\n", passosPorSegundo, isModoResolvedor, pathFile);
    if(!novoPredio(&predio, pathFile)){
        puts("Não foi possível abrir o arquivo.");
        return 1;
    }
    
    SDL_Window *window;
    SDL_Renderer *screen;
    window = SDL_CreateWindow("Jirobaldo: Survivor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
    screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    splashScreen(screen, isModoResolvedor);
    // SDL_Event event;

    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    fimJogo();
    return 0;
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

void lerOpcoes(int argc, char **argv, int *passosPorSegundo, bool *isModoResolvedor){
    int c;
    while( (c = getopt(argc, argv, "f:s")) != -1 ){
        switch(c){
            case 'f':
                *passosPorSegundo = atoi(optarg);
                break;
            case 's':
                *isModoResolvedor = true;
                break;
        }
    }
}

void splashScreen(SDL_Renderer *screen, bool isModoResolvedor){
    SDL_Surface *tmp;
    SDL_Texture *bg, *titleText, *subtitleText;
    TTF_Font *titleFont;
    SDL_Rect titleRect, subtitleRect;
    int alpha=0;

    tmp = IMG_Load("data/images/fire.jpg");
    bg = SDL_CreateTextureFromSurface(screen, tmp);
    SDL_FreeSurface(tmp);

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
    carregarTexturasPredio(screen);
    if(isModoResolvedor){
        // carregarModoResolvedor();
        // executarModoResolvedor();
    }else{
        SDL_RenderClear(screen);
        renderAndarPredio(&predio, 0, screen);
        SDL_RenderPresent(screen);
        int i;
        scanf("%d", &i);
        // carregarModoInterativo();
        // executarModoInterativo();
    }

    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(titleText);
    TTF_CloseFont(titleFont);
}   

void fimJogo(){
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}