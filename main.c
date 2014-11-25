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
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include "predio.h"
#include "libpredio.h"
#include "global.h"

//Lê os parâmetros passados na linha de comando, retorna false caso ocorra um erro
bool lerParametros(int argc, char **argv);

//Retorna true se o SDL inicializou de forma correta, false caso contrário
bool iniciaSDL();

//Libera da memória as texturas, janelas, renderers e termina o SDL
void fechaSDL();

//Lida com os eventos de teclado durante o jogo
void validaEventos(SDL_Event event);

//Retorna true se Jirobaldo está numa posição válida no mapa
bool jirobaldoValido();

//Renderiza texto
void renderText(TTF_Font *fonte, char *texto, SDL_Rect aux, SDL_Color cor, int align);

//Renderiza a barra de informações (baldes, passos, andar)
void renderInfoBar(SDL_Rect aux);

//Retorna true se a posição atual do Jirobaldo é a saída
bool isSaida();

//Gera um novo Edificio (Edificio é a antiga estrutura Predio na solução do Flávio)
void novoEdificio(Edificio *edificio);

//Tela de abertura
void splashScreen();

//Tela de encerramento
void fimJogo();

//Encerramento ruim
void jiroDeath();

//Função para esperar o tempo correto
void renderDelay(Uint32 renderTime);

int main(int argc, char **argv){
    bool quit = false;
    SDL_Event event;
    SDL_Rect aux;
    Uint32 renderTime, ticks = 0; //Tempo inicial

    //Lê os parâmetros da linha de comando
    if(!lerParametros(argc, argv)){
        puts("ERRO: Digite um número inteira na entrada");
        return 1;
    }

    //Lê o arquivo do prédio
    if(arquivo == NULL){
        puts("ERRO: Nenhum arquivo foi especificado.");
        return 1;
    }

    if(!novoPredio(&predio, arquivo)){
        puts("ERRO: Não foi possível criar o prédio. O arquivo é válido?");
        return 1;
    }

    /*
    * Edificio é a "antiga" struct Predio na solução do Flávio.
    */
    Edificio edificio;
    novoEdificio(&edificio);
    
    //Inicia SDL
    if(!iniciaSDL()){
        puts("ERRO: não foi possível iniciar o SDL");
        return 1;
    }

    //Cria a janela e o renderizador
    window = SDL_CreateWindow("Jirobaldo: Sobrevivente", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Carrega as texturas do prédio e do Jirobaldo
    carregarTexturasPredio(screen, &predio);
    carregarTexturasJirobaldo(screen, &predio.jirobaldo);

    //Carrega fontes e sons
    titleFont = TTF_OpenFont("data/fonts/Plane-Crash.ttf", 48);
    water = Mix_LoadWAV("data/audio/water-splash.wav");
    titleTheme = Mix_LoadMUS("data/audio/bost-imagine-the-fire.ogg");
    fire = Mix_LoadMUS("data/audio/fire.wav");
    doors = Mix_LoadMUS("data/audio/light-my-fire.ogg");
    footstep = Mix_LoadWAV("data/audio/footstep3.wav");
    Mix_PlayMusic(titleTheme, -1);
    
    //SplashScreen
    splashScreen();
    
    //Gera a solução
    resp = predio_resolve(&edificio);
    if(resp->len < 0){
        puts("ERRO: o mapa não tem solução");
        return 1;
    }
    
    //Rect auxiliar para gerar as texturas dos andares
    aux.h = (predio.h >= predio.w) ? (gameViewport.h/predio.h) : (gameViewport.w/predio.w);
    aux.w = aux.h;
    aux.x = 0;
    aux.y = 0;
    /*
    * Gera as texturas do andares (para mapas grandes pode demorar um pouco,
    * mas compensa na velocidade de renderização)
    */
    gerarTexturasAndares(screen, &predio, aux);

    
    /*
     * Loop princpial:
     *     Render do mapa
     *     Lida com os eventos:
     *         Se for simulador lida com a jogabilidade
     *         Se for resolvedor, avança/retrocede um passo
     */
    Mix_FadeOutMusic(2000);
    Mix_PlayMusic(fire, -1);
    while(!quit){
        renderTime = SDL_GetTicks();
        SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
        SDL_RenderClear(screen);

        //Renderiza a barra de status
        SDL_RenderSetViewport(screen, &infoBarViewport);
        aux.h = INFO_BAR_HEIGHT;
        aux.x = 0;
        aux.y = 0;
        renderInfoBar(aux);
        
        //Renderiza o 'jogo' principal
        aux.h = (predio.h >= predio.w) ? (gameViewport.h/predio.h) : (gameViewport.w/predio.w);
        aux.w = aux.h;
        SDL_RenderSetViewport(screen, &gameViewport);
        renderAndarPredio(screen, &predio, predio.jirobaldo.z, aux);

        //Renderiza o mini-mapa do andar de cima
        if(predio.jirobaldo.z-1 >= 0){
            aux.h = downMapViewport.h/predio.h;
            aux.w = aux.h;
            SDL_RenderSetViewport(screen, &downMapViewport);
            renderAndarPredio(screen, &predio, predio.jirobaldo.z - 1, aux);
        }

        //Renderiza o mini-mapa do andar de baixo
        if(predio.jirobaldo.z+1 < predio.altura){
            aux.h = topMapViewport.h/predio.h;
            aux.w = aux.h;
            SDL_RenderSetViewport(screen, &topMapViewport);
            renderAndarPredio(screen, &predio, predio.jirobaldo.z + 1, aux);
        }

        SDL_RenderPresent(screen);

        //Se for resolvedor, com passos por segundo > 0 e tiver passado o tempo para renderizar
        if(isModoResolvedor && passosPorSegundo > 0
            && (SDL_GetTicks() - ticks >= (1000/passosPorSegundo))){
            ticks = SDL_GetTicks();
            //Empilha um evento com valor da seta direita
            SDL_Event evento;
            SDL_zero(evento);
            evento.type = SDL_KEYDOWN;
            evento.key.keysym.sym = SDLK_RIGHT;
            SDL_PushEvent(&evento);
        }

        quit = (bool) isSaida();
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = true;
            }else{
                validaEventos(event);
            }
        }

        renderDelay(renderTime);
    }

    if(passos >= resp->len){ //Animação de fim de jogo
        Mix_PlayMusic(doors, -1);
        fimJogo();
    }
    Mix_HaltMusic();
    fechaSDL();
    return 0;
}

bool lerParametros(int argc, char **argv){
    int c;
    while((c = getopt(argc, argv, "sf:")) != -1){
        switch(c){
            case 's':
            isModoResolvedor = true;
            break;
            case 'f':
            passosPorSegundo = atof(optarg);
            if(passosPorSegundo != atoi(optarg)){
                passosPorSegundo = 0;
                return false;
            }
            break;
        }
    }

    isModoResolvedor = (passosPorSegundo > 0);

    if(optind < argc){
        arquivo = argv[optind];
    }

    return true;
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

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1){
        return false;
    }

    //Define as dimensões das viewports
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
    downMapViewport.y = INFO_BAR_HEIGHT + (SCREEN_WIDTH*0.2) + 10; //10px para separar os mapas
    downMapViewport.w = (SCREEN_WIDTH*0.2);
    downMapViewport.h = (SCREEN_WIDTH*0.2);
    
    return true;
}

void fechaSDL(){
    Mix_FreeMusic(titleTheme);
    Mix_FreeMusic(fire);
    Mix_FreeMusic(doors);
    Mix_FreeChunk(footstep);
    Mix_FreeChunk(water);
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    TTF_CloseFont(titleFont);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void validaEventos(SDL_Event event){
    int x = predio.jirobaldo.x;
    int y = predio.jirobaldo.y;
    int z = predio.jirobaldo.z;

    //Se for resolvedor, aceita apenas eventos das setas para direita e esquerda
    if(isModoResolvedor){
        if(event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT)){
            int passo;
            int tecla = event.key.keysym.sym;
            if(tecla == SDLK_RIGHT){
                passo = resp->p[passos];
                if(passos < resp->len)
                    passos++;
            }else if(tecla == SDLK_LEFT && passosPorSegundo == 0){
                if(passos > 0)
                    passos--;
                else
                    return;
                passo = resp->p[passos];
            }else{
                return;
            }

            switch(passo){
                case PASSO_CIMA:
                    predio.jirobaldo.face = (tecla == SDLK_RIGHT) ? FACE_NORTH : FACE_SOUTH;
                    predio.jirobaldo.isAnimating = true;
                    tecla == SDLK_RIGHT ? predio.jirobaldo.x-- : predio.jirobaldo.x++;
                    break;
                case PASSO_BAIXO:
                    predio.jirobaldo.face = (tecla == SDLK_RIGHT) ? FACE_SOUTH : FACE_NORTH;
                    predio.jirobaldo.isAnimating = true;
                    tecla == SDLK_RIGHT ? predio.jirobaldo.x++ : predio.jirobaldo.x--;
                    break;
                case PASSO_ESQUERDA:
                    predio.jirobaldo.face = (tecla == SDLK_RIGHT) ? FACE_WEST : FACE_EAST;
                    predio.jirobaldo.isAnimating = true;
                    tecla == SDLK_RIGHT ? predio.jirobaldo.y-- : predio.jirobaldo.y++;
                    break;
                case PASSO_DIREITA:
                    predio.jirobaldo.face = (tecla == SDLK_RIGHT) ? FACE_EAST : FACE_WEST;
                    predio.jirobaldo.isAnimating = true;
                    tecla == SDLK_RIGHT ? predio.jirobaldo.y++ : predio.jirobaldo.y--;
                    break;
                case PASSO_SOBE:
                    tecla == SDLK_RIGHT ? predio.jirobaldo.z++ : predio.jirobaldo.z--;
                    break;
                case PASSO_DESCE:
                    tecla == SDLK_RIGHT ? predio.jirobaldo.z-- : predio.jirobaldo.z++;
                    break;
                case PASSO_ENCHE:
                    Mix_PlayChannel(-1, water, 0);
                    tecla == SDLK_RIGHT ? predio.jirobaldo.baldes++ : predio.jirobaldo.baldes--;
                    break;
            }
            jirobaldoValido();
        }
    }else{
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_LEFT:
                    predio.jirobaldo.y--;
                    if(jirobaldoValido()){
                        predio.jirobaldo.face = FACE_WEST;
                        predio.jirobaldo.isAnimating = true;
                        passos++;
                        Mix_PlayChannel(-1, footstep, 0);
                    }else{
                        predio.jirobaldo.y++;
                    }
                    break;
                case SDLK_RIGHT:
                    predio.jirobaldo.y++;
                    if(jirobaldoValido()){
                        predio.jirobaldo.face = FACE_EAST;
                        predio.jirobaldo.isAnimating = true;
                        passos++;
                        Mix_PlayChannel(-1, footstep, 0);
                    }else{
                        predio.jirobaldo.y--;
                    }
                    break;
                case SDLK_UP:
                    predio.jirobaldo.x--;
                    if(jirobaldoValido()){
                        predio.jirobaldo.face = FACE_NORTH;
                        predio.jirobaldo.isAnimating = true;
                        passos++;
                        Mix_PlayChannel(-1, footstep, 0);
                    }else{
                        predio.jirobaldo.x++;
                    }
                    break;
                case SDLK_DOWN:
                    predio.jirobaldo.x++;
                    if(jirobaldoValido()){
                        predio.jirobaldo.face = FACE_SOUTH;
                        predio.jirobaldo.isAnimating = true;
                        passos++;
                        Mix_PlayChannel(-1, footstep, 0);
                    }else{
                        predio.jirobaldo.x--;
                    }
                    break;
                case SDLK_x:
                    if(predio.pisos[z].pontos[x][y] == 'U'
                        || predio.pisos[z].pontos[x][y] == 'E'){
                        predio.jirobaldo.z++;
                        passos++;
                    }
                    break;
                case SDLK_z:
                    if(predio.pisos[z].pontos[x][y] == 'D'
                        || predio.pisos[z].pontos[x][y] == 'E'){
                        predio.jirobaldo.z--;
                        passos++;
                    }
                    break;
                case SDLK_SPACE:
                    if(predio.pisos[z].pontos[x][y] == 'T'){
                        predio.jirobaldo.baldes++;
                        if(predio.jirobaldo.baldes > predio.jirobaldo.MAX_BALDES){
                            predio.jirobaldo.baldes--;
                        }else{
                            Mix_PlayChannel(-1, water, 0);
                            passos++;
                        }
                    }
                    break;
                case SDLK_RETURN:
                    predio.jirobaldo.x = predio.jirobaldo.sx;
                    predio.jirobaldo.y = predio.jirobaldo.sy;
                    predio.jirobaldo.z = predio.jirobaldo.sz;
                    predio.jirobaldo.baldes = 0;
                    passos = 0;
                    break;
            }
        }
    }
}

bool jirobaldoValido(){
    int x = predio.jirobaldo.x;
    int y = predio.jirobaldo.y;
    int z = predio.jirobaldo.z;
    int baldes = predio.jirobaldo.baldes;

    if(isPontoNoAndar(&predio.pisos[z], x, y)){
        if(predio.pisos[z].pontos[x][y] == 'F'){
            if(baldes > 0){
                predio.jirobaldo.baldes--;
            }else{
                return false;
            }
        }
    }

    return (isPontoNoAndar(&predio.pisos[z], x, y))
        && (predio.pisos[z].pontos[x][y] != '#'); 
}

void renderText(TTF_Font *fonte, char *texto, SDL_Rect aux, SDL_Color cor, int align){
    SDL_Surface *tmp;
    SDL_Texture *textoTex;
    SDL_Rect textoRect;

    tmp = TTF_RenderUTF8_Solid(fonte, texto, cor);
    textoTex = SDL_CreateTextureFromSurface(screen, tmp);
    textoRect.w = tmp->w;
    textoRect.h = tmp->h;
    textoRect.x = 0;
    textoRect.y = 0;
    aux.w = textoRect.w/2;
    if(align == 1){
        aux.x = SCREEN_WIDTH/2 - aux.w/2;
    }else if(align == 2){
        aux.x = SCREEN_WIDTH - aux.w;
    }
    SDL_RenderCopy(screen, textoTex, &textoRect, &aux);
    SDL_FreeSurface(tmp);

    SDL_DestroyTexture(textoTex);
}

void renderInfoBar(SDL_Rect aux){
    char baldes[20];
    sprintf(baldes, "baldes: %d/%d", predio.jirobaldo.baldes, predio.jirobaldo.MAX_BALDES);
    renderText(titleFont, baldes, aux, (SDL_Color) {200, 200, 200}, 0);
    char andar[20];
    sprintf(andar, "andar: %d/%d", predio.jirobaldo.z + 1, predio.altura);
    renderText(titleFont, andar, aux, (SDL_Color) {200, 200, 200}, 1);
    char moves[20];
    sprintf(moves, "moves: %d", passos);
    renderText(titleFont, moves, aux, (SDL_Color) {200, 200, 200}, 2);
}

bool isSaida(){
    int x = predio.jirobaldo.x;
    int y = predio.jirobaldo.y;
    int z = predio.jirobaldo.z;
    bool animating = isModoResolvedor ? false : predio.jirobaldo.isAnimating;
    return (predio.pisos[z].pontos[x][y] == 'S' && ! animating);
}

void novoEdificio(Edificio *edificio){
    int i, j;
    FILE *file = fopen(arquivo, "r");
    fscanf(file, "%d %d %d %d", &(edificio->A), &(edificio->W), &(edificio->H), &(edificio->B));
    for(i = 0; i < edificio->A; i++){
        for(j = 0; j < edificio->H; j++){
            fscanf(file, "%s", edificio->m[i][j]);
        }
    }
    fclose(file);
}

void splashScreen(){
    SDL_Texture *title, *subtitle;
    SDL_Rect titleRect, subtitleRect;
    SDL_Surface *tmp;
    Uint8 alpha = 0;

    tmp = TTF_RenderUTF8_Solid(titleFont, "jirobaldo", (SDL_Color) {180, 0, 0});
    title = SDL_CreateTextureFromSurface(screen, tmp);
    SDL_SetTextureAlphaMod(title, alpha);
    titleRect.w = tmp->w;
    titleRect.h = tmp->h;
    titleRect.x = SCREEN_WIDTH/2 - titleRect.w/2;
    titleRect.y = SCREEN_HEIGHT/2 - titleRect.h/2;
    SDL_FreeSurface(tmp);

    tmp = TTF_RenderUTF8_Solid(titleFont, "sobrevivente", (SDL_Color) {200, 200, 200});
    subtitle = SDL_CreateTextureFromSurface(screen, tmp);
    SDL_SetTextureAlphaMod(subtitle, alpha);
    subtitleRect.w = tmp->w;    
    subtitleRect.h = tmp->h;
    subtitleRect.x = SCREEN_WIDTH/2 - subtitleRect.w/2;
    subtitleRect.y = titleRect.y + subtitleRect.h;
    SDL_FreeSurface(tmp);

    while(alpha < 255){
        SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
        SDL_RenderClear(screen);
        SDL_RenderCopy(screen, title, NULL, &titleRect);
        SDL_RenderCopy(screen, subtitle, NULL, &subtitleRect);
        SDL_RenderPresent(screen);
        
        alpha+=3;
        SDL_SetTextureAlphaMod(title, alpha);
        SDL_SetTextureAlphaMod(subtitle, alpha);

        SDL_Delay(1000/30);
    }

    SDL_DestroyTexture(title);
    SDL_DestroyTexture(subtitle);
}

void fimJogo(){
    SDL_Texture *congrats;
    SDL_Rect congratsRect, jiroRect;
    SDL_Surface *tmp;
    Uint8 alpha = 0;
    bool quit = false;
    SDL_Event event;

    tmp = TTF_RenderUTF8_Solid(titleFont, "jirobaldo vive", (SDL_Color) {255, 165, 0});
    congrats = SDL_CreateTextureFromSurface(screen, tmp);
    SDL_SetTextureAlphaMod(congrats, alpha);
    congratsRect.w = tmp->w;
    congratsRect.h = tmp->h;
    congratsRect.x = SCREEN_WIDTH/2 - congratsRect.w/2;
    congratsRect.y = SCREEN_HEIGHT/2 - congratsRect.h/2;
    jiroRect.w = 64;
    jiroRect.h = 64;
    jiroRect.x = SCREEN_WIDTH/2 - jiroRect.w/2;
    jiroRect.y = congratsRect.y + jiroRect.h;
    SDL_FreeSurface(tmp);

    predio.jirobaldo.face = FACE_SOUTH;
    predio.jirobaldo.isAnimating = true;
    predio.jirobaldo.frame = 0;

    SDL_RenderSetViewport(screen, NULL);
    while(!quit){
        SDL_SetRenderDrawColor(screen, 0, 0, 0, 255);
        SDL_RenderClear(screen);
        SDL_RenderCopy(screen, congrats, NULL, &congratsRect);
        renderDanceJirobaldo(screen, &predio.jirobaldo, jiroRect);
        SDL_RenderPresent(screen);

        alpha+=5;
        SDL_SetTextureAlphaMod(congrats, alpha);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = true;
            }
        }

        SDL_Delay(1000/30);
    }

    SDL_DestroyTexture(congrats);
}

void renderDelay(Uint32 renderTime){
    Uint32 t = SDL_GetTicks();
    Uint32 delay = 1000/30; //"30 fps"
    Uint32 render = t - renderTime;
    if(isModoResolvedor && passosPorSegundo > 30 && passosPorSegundo <= 1000){
        delay = 1000/passosPorSegundo;
    }else if(passosPorSegundo > 1000){
        delay = 1;
    }

    if(render < delay){
        SDL_Delay(delay - render - 1);
    }
}