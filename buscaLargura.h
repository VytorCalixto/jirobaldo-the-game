#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#include "planta.h"
#include "predio.h"
#include "fila.h"

//Faz a busca em largura e devolve o tempo para Jirobaldo sair do prédio ou -1 caso seja impossível.
int buscaEmLargura(Predio predio, Vertice *raiz);

//Enfileira vértices adjacentes ao vértice passado como parâmetro
void enfileiraAdjacentes(Fila *fila, Vertice vertice, Predio predio, Planta *planta);

//Retorna um vetor com todos os vértices adjacentes ao vértice passado como parâmetro.
Vertice *getAdjacentes(Vertice vertice, Predio predio);

/**
 * Retorna true se o valor do vértice é valido (não é parede ou algo diferente dos caracteres válidos) e o vértice está dentro dos limites do mapa
 * false caso contrário
 */
bool isVerticeValido(Predio predio, Vertice v);

// Converte um ponto (x, y, z) do prédio para um Vertice
Vertice pontoParaVertice(Predio predio, int x, int y, int z);

#endif