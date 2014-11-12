/*
 * Conjunto de variáveis globais, como a janela do SDL e flags passadas na execução do programa
 */
#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

//Dimensões da tela
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define INFO_BAR_HEIGHT 32

//Janela e Renderer
SDL_Window *window;
SDL_Renderer *screen;

//Textos e fontes
SDL_Texture *baldesText, *movesText;
TTF_Font *titleFont;
SDL_Rect baldesRect, movesRect;

//Viewports
SDL_Rect infoBarViewport;

SDL_Rect gameViewport;

SDL_Rect topMapViewport;

SDL_Rect downMapViewport;

Passos *resp;
int passosPorSegundo = 0;
bool isModoResolvedor = false;
char *arquivo;
int passos = 0;
Mix_Music *titleTheme;

#include "predio.h"
Predio predio;

#endif