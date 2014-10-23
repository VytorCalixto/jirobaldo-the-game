#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


int main(int argc, char **argv){
    int stepsPerSecond = 0;
    bool isModoResolvedor = false;
    int c;
    
    //Lê as opções na linha de comando
    while( (c = getopt(argc, argv, "f:s")) != -1 ){
        switch(c){
            case 'f':
                stepsPerSecond = atoi(optarg);
                break;
            case 's':
                isModoResolvedor = true;
                break;
        }
    }

    //printf("PPS: %d, MR: %d\n", stepsPerSecond, isModoResolvedor);
    
    char *path;
    if(optind < argc){
        path = argv[optind];
    }
    //printf("Path: %s\n", path);
    
    SDL_Window *window;
    SDL_Renderer *screen;
    SDL_Surface *tmp;
    SDL_Texture *bg, *titleText;
    SDL_Event event;
    TTF_Font *titleFont;
    SDL_Rect titleRect;
    Mix_Music *titleTheme;
    bool quit = 0;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0 
            || !IMG_Init(IMG_INIT_PNG) 
            || TTF_Init() < 0
            || Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 ){
        return 1;
    }

    window = SDL_CreateWindow("Jirobaldo: Survivor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    int alpha = 0;
    tmp = IMG_Load("data/bg1.jpg");
    bg = SDL_CreateTextureFromSurface(screen, tmp);
    SDL_FreeSurface(tmp);

    titleFont = TTF_OpenFont("data/fonts/Plane-Crash.ttf", 48);
    tmp = TTF_RenderUTF8_Solid(titleFont, "jirobaldo:survivor", (SDL_Color) {139, 0, 0});  
    titleText = SDL_CreateTextureFromSurface(screen, tmp);
    SDL_SetTextureAlphaMod(titleText, alpha);
    titleRect.h = tmp->h;
    titleRect.w = tmp->w;
    titleRect.x = SCREEN_WIDTH/2 - titleRect.w/2;
    titleRect.y = SCREEN_HEIGHT/2 - titleRect.h/2;
    SDL_FreeSurface(tmp);

    titleTheme = Mix_LoadMUS("data/audio/bost-imagine-the-fire.ogg");

    Mix_PlayMusic(titleTheme, -1);
    while(!quit){
        SDL_RenderClear(screen);
        // SDL_RenderCopy(screen, bg, NULL, NULL);
        SDL_RenderCopy(screen, titleText, NULL, &titleRect);
        SDL_RenderPresent(screen);
        
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = true;
            }
        }

        if(alpha < 255){
            alpha+=5;
            SDL_SetTextureAlphaMod(titleText, alpha);
        }else if(titleRect.y > SCREEN_HEIGHT/2 - titleRect.h/2 - 200){
            titleRect.y -= 1;
        }

        SDL_Delay(1000/30);
    }
    Mix_PauseMusic();

    Mix_FreeMusic(titleTheme);
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(titleText);
    TTF_CloseFont(titleFont);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
