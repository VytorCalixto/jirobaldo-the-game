/*
 * Conjunto de variáveis globais, como a janela do SDL e flags passadas na execução do programa
 */
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window *window;
SDL_Renderer *screen;
int passosPorSegundo = 0;
bool isModoResolvedor = false;
char *arquivo;

#include "predio.c"
Predio predio;