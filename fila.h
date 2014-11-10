#ifndef FILA_H
#define FILA_H

#include "predio.h"

typedef struct Vertice{
  char valor;
  int x, y, z;
  int baldes; //baldes com água naquele vértice
  int tempo; //tempo que girobaldo levou até chegar nesse vértice
  struct Vertice *anterior; //Vertice que "chegou" neste vértice
}Vertice;

typedef struct ElementoFila{
  Vertice vertice;
  struct ElmentoFila *prox;
}ElementoFila;

typedef struct{
  ElementoFila *frente, *final;

}Fila;

void iniciaFila(Fila *fila);
//Retorna true se a fila está vazia, false caso contrário
bool vaziaFila(Fila *fila);
void enfileira(Fila *fila, Vertice vertice);
Vertice desenfileira(Fila *fila);
#endif