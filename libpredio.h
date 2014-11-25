typedef enum Passo {
  PASSO_CIMA,
  PASSO_BAIXO,
  PASSO_ESQUERDA,
  PASSO_DIREITA,
  PASSO_SOBE,
  PASSO_DESCE,
  PASSO_ENCHE
} Passo;

typedef struct Edificio {
  char m[128][128][128];
  int H, W, A, B;
} Edificio ;

typedef struct Passos {
  Passo *p;
  int len;
} Passos;

Passos *predio_resolve(const Edificio *predio);
void predio_free_passos(Passos *p);
