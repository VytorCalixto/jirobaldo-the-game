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

SDL_Rect gameViewport;

SDL_Rect topMapViewport;

SDL_Rect downMapViewport;

int passosPorSegundo = 0;
bool isModoResolvedor = false;
char *arquivo;

#include "predio.h"
Predio predio;

#endif