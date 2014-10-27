typedef struct Vertice{
  char valor;
  int x, y, z
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

void iniciaFila(Fila *fila){
  fila->frente = (ElementoFila *) malloc(sizeof(ElementoFila)); //Primeiro nodo da fila é nodo cabeça
  fila->final = fila->frente;
  fila->final->prox = NULL;
}

//Retorna true se a fila está vazia, false caso contrário
bool vaziaFila(Fila *fila){
  return (fila->frente == fila->final);
}

void enfileira(Fila *fila, Vertice vertice){
  fila->final->prox = (ElementoFila *) malloc(sizeof(ElementoFila));
  fila->final = fila->final->prox;
  fila->final->vertice = vertice;
  fila->final->prox = NULL;
}

Vertice desenfileira(Fila *fila){
  ElementoFila *aux;
  if(!vaziaFila(fila)){
    aux = fila->frente;
    fila->frente = fila->frente->prox;
    free(aux);
    return fila->frente->vertice;
  }
}

void imprimeFila(Fila *fila){
  ElementoFila *aux = fila->frente->prox;
  while(aux != NULL){
    printf("%c %dx%dx%d - %db - %dt\n", aux->vertice.valor, aux->vertice.x, aux->vertice.y, aux->vertice.z, aux->vertice.baldes, aux->vertice.tempo);
    aux = aux->prox;
  }
}
