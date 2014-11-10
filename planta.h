#ifndef PLANTA_H
#define PLANTA_H

// #include "predio.h"
#include "fila.h"

//Serve para marcar os vértices em que já passamos. 0 caso não tenha passado por lá.
typedef struct{
    int *** planta;
}Planta;

//Aloca a planta com o mesmo tamanho do prédio e define todas as posições como 0
void iniciaPlanta(Planta *planta, Predio *predio);

//Marca que um vértice foi enfileirado
void marcaVertice(Planta *planta, Vertice v);

//Retorna true se o vértice já tiver sido marcado, false caso contrário
bool isVerticeMarcado(Planta *planta, Vertice v);

void imprimePlanta(Planta planta, Predio *predio);

#endif