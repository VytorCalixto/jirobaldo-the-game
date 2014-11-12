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
#include "libpredio.h"
#include "global.h"

//Lê os parâmetros passados na linha de comando
void lerParametros(int argc, char **argv);
bool iniciaSDL();
//Libera da memória as texturas, janelas, renderers e termina o SDL
void fechaSDL();
//Lida com os eventos de teclado durante o jogo
void validaEventos(SDL_Event event);
bool jirobaldoValido();
void renderText(TTF_Font *fonte, char *texto, SDL_Rect aux, SDL_Color cor, int align);
bool isSaida();
void novoEdificio(Edificio *edificio);

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
    /*
    * Edificio é a "antiga" struct Predio na solução do Flávio.
    * 
    */
    Edificio edificio;
    novoEdificio(&edificio);
    
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

    //Se for modo resolvedor gera a solução
    resp = predio_resolve(&edificio);
    // printf("%d\n", resp->len);
    // int i;
    // for (i = 0; i < resp->len; i++) {
    //     printf("%d ", resp->p[i]);
    // }
    // putchar('\n');
    
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
        SDL_RenderSetViewport(screen, &infoBarViewport);
        aux.h = INFO_BAR_HEIGHT;
        aux.x = 0;
        aux.y = 0;
        //TODO: colocar a renderização da barra numa função
        char baldes[20];
        sprintf(baldes, "baldes: %d/%d", predio.jirobaldo.baldes, predio.jirobaldo.MAX_BALDES);
        renderText(titleFont, baldes, aux, (SDL_Color) {200, 200, 200}, 0);
        char andar[20];
        sprintf(andar, "andar: %d/%d", predio.jirobaldo.z + 1, predio.altura);
        renderText(titleFont, andar, aux, (SDL_Color) {200, 200, 200}, 1);
        char moves[20];
        sprintf(moves, "moves: %d", passos);
        renderText(titleFont, moves, aux, (SDL_Color) {200, 200, 200}, 2);

        //Renderiza o 'jogo' principal
        aux.h = (predio.h >= predio.w) ? (gameViewport.h/predio.h) : (gameViewport.w/predio.w);
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
                quit = isSaida() ? true : false;
            }
        }
        SDL_Delay(1000/30);
    }

    //fimJogo(); //Animação de fim de jogo
    fechaSDL();
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
    downMapViewport.y = INFO_BAR_HEIGHT + (SCREEN_WIDTH*0.2) + 10; //10px para separar os mapas
    downMapViewport.w = (SCREEN_WIDTH*0.2);
    downMapViewport.h = (SCREEN_WIDTH*0.2);
    return true;
}

void fechaSDL(){
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    TTF_CloseFont(titleFont);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void validaEventos(SDL_Event event){
    int x = predio.jirobaldo.x;
    int y = predio.jirobaldo.y;
    int z = predio.jirobaldo.z;
    if(isModoResolvedor && passosPorSegundo == 0){
        if(event.type == SDL_KEYDOWN){
            int passo = resp->p[passos];
            int tecla = event.key.keysym.sym;
            //FIXME: bug na seta da esquerda
            switch(passo){
                case PASSO_CIMA:
                    if(tecla == SDLK_RIGHT){
                        predio.jirobaldo.x--;
                        passos++;
                        predio.jirobaldo.face = FACE_NORTH;
                        predio.jirobaldo.isAnimating = true;
                    }else if(tecla == SDLK_LEFT){
                        predio.jirobaldo.x++;
                        passos--;
                        predio.jirobaldo.face = FACE_SOUTH;
                        predio.jirobaldo.isAnimating = true;
                    }
                    break;
                case PASSO_BAIXO:
                    if(tecla == SDLK_RIGHT){
                        predio.jirobaldo.x++;
                        passos++;
                        predio.jirobaldo.face = FACE_SOUTH;
                        predio.jirobaldo.isAnimating = true;
                    }else if(tecla == SDLK_LEFT){
                        predio.jirobaldo.x--;
                        passos--;
                        predio.jirobaldo.face = FACE_NORTH;
                        predio.jirobaldo.isAnimating = true;
                    }
                    break;
                case PASSO_ESQUERDA:
                    if(tecla == SDLK_RIGHT){
                        predio.jirobaldo.y--;
                        passos++;
                        predio.jirobaldo.face = FACE_WEST;
                        predio.jirobaldo.isAnimating = true;
                    }else if(tecla == SDLK_LEFT){
                        predio.jirobaldo.y++;
                        passos--;
                        predio.jirobaldo.face = FACE_EAST;
                        predio.jirobaldo.isAnimating = true;
                    }
                    break;
                case PASSO_DIREITA:
                    if(tecla == SDLK_RIGHT){
                        predio.jirobaldo.y++;
                        passos++;
                        predio.jirobaldo.face = FACE_EAST;
                        predio.jirobaldo.isAnimating = true;
                    }else if(tecla == SDLK_LEFT){
                        predio.jirobaldo.y--;
                        passos--;
                        predio.jirobaldo.face = FACE_WEST;
                        predio.jirobaldo.isAnimating = true;
                    }
                    break;
                case PASSO_SOBE:
                    if(tecla == SDLK_RIGHT){
                        predio.jirobaldo.z++;
                        passos++;
                    }else if(tecla == SDLK_LEFT){
                        predio.jirobaldo.z--;
                        passos--;
                    }
                    break;
                case PASSO_DESCE:
                    if(tecla == SDLK_RIGHT){
                        predio.jirobaldo.z--;
                        passos++;
                    }else if(tecla == SDLK_LEFT){
                        predio.jirobaldo.z++;
                        passos--;
                    }
                    break;
                case PASSO_ENCHE:
                    if(tecla == SDLK_RIGHT){
                        predio.jirobaldo.baldes++;
                        passos++;
                    }else if(tecla == SDLK_LEFT){
                        predio.jirobaldo.baldes--;
                        passos--;
                    }
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
                            passos++;
                        }
                    }
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

bool isSaida(){
    int x = predio.jirobaldo.x;
    int y = predio.jirobaldo.y;
    int z = predio.jirobaldo.z;
    return (predio.pisos[z].pontos[x][y] == 'S');
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