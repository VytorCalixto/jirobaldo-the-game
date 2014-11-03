/*
 * Conjunto de variáveis globais, como a janela do SDL e flags passadas na execução do programa
 */
#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

//Dimensões da tela
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//Janela e Renderer
SDL_Window *window;
SDL_Renderer *screen;

//Viewports
SDL_Rect infoBarViewport;
infoBarViewport.x = 0;
infoBarViewport.y = 0;
infoBarViewport.w = SCREEN_WIDTH;
infoBarViewport.h = 100;

SDL_Rect gameViewport;
gameViewport.x = 0;
gameViewport.y = 100;
gameViewport.w = (int) SCREEN_WIDTH - (SCREEN_WIDTH*0.2);
gameViewport.h = (int) SCREEN_HEIGHT - 100;

SDL_Rect topMapViewport;
topMapViewport.x = SCREEN_WIDTH - (SCREEN_WIDTH*0.2);
topMapViewport.y = 100;
topMapViewport.w = (SCREEN_WIDTH*0.2);
topMapViewport.h = (SCREEN_WIDTH*0.2);

SDL_Rect downMapViewport;
downMapViewport.x = SCREEN_WIDTH - (SCREEN_WIDTH*0.2);
downMapViewport.y = 100 + (SCREEN_WIDTH*0.2);
downMapViewport.w = (SCREEN_WIDTH*0.2);
downMapViewport.h = (SCREEN_WIDTH*0.2);

int passosPorSegundo = 0;
bool isModoResolvedor = false;
char *arquivo;

#include "predio.h"
Predio predio;

#endif