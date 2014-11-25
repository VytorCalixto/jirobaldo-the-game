#include <string.h>
#include <stdlib.h>
#include "libpredio.h"

#define min(a,b) (a < b? a : b)

typedef struct Pos {
  int x, y, z, b;
} Pos;

typedef struct Fila {
  int _ini, _fim;
  Pos _d[128*128*128*10];
} Fila;

static void fila_limpa(Fila *f) {
  f->_ini = f->_fim = 0;
}

static void fila_push(Fila *f, Pos v) {
  f->_d[(f->_fim)++]= v;
}

static Pos fila_pop(Fila *f) {
  return f->_d[(f->_ini)++];
}

static int fila_vazia(Fila *f) {
  return (f->_ini == f->_fim);
}

static int dist[128][128][128][9];
static Pos pai[128][128][128][9];
static Fila q;
static int sol_size;
static Pos solution[1000024];
static int dx[] = {0, 1, 0, -1};
static int dy[] = {-1, 0, 1, 0};

void get_solution(Pos fim) {
  int i;
  Pos p = fim;

  for (i = sol_size; i >= 0; i--) {
    solution[i] = p;
    p = pai[p.z][p.y][p.x][p.b];
  }
}

static int bfs(Pos ini, Pos fim, const Edificio *predio) {
  int r, i, d;
  Pos n, p;

  memset(dist, 0x3f, sizeof(dist));
  fila_limpa(&q);

  ini.b = 0;
  dist[ini.z][ini.y][ini.x][0] = 0;
  pai[ini.z][ini.y][ini.x][0] = ini;
  fila_push(&q, ini);

  while (!fila_vazia(&q)) {
    n = fila_pop(&q);
    d = dist[n.z][n.y][n.x][n.b];

    /* Andar para os lados */
    for (i = 0; i < 4; i++) {
      p.x = n.x + dx[i];
      p.y = n.y + dy[i];
      p.z = n.z;

      if (p.x < 0 || p.x >= predio->W || p.y < 0 || p.y >= predio->H) {
        continue;
      }

      switch (predio->m[p.z][p.y][p.x]) {
        case '.':
        case 'J':
        case 'S':
        case 'D':
        case 'E':
        case 'U':
        case 'T':
          p.b = n.b;
          if (dist[p.z][p.y][p.x][p.b] == 0x3f3f3f3f) {
            fila_push(&q, p);
            pai[p.z][p.y][p.x][p.b] = n;
            dist[p.z][p.y][p.x][p.b] = d+1;
          }
          break;

        case 'F':
          p.b = n.b-1;
          if (p.b >= 0 && dist[p.z][p.y][p.x][p.b] == 0x3f3f3f3f) {
            fila_push(&q, p);
            pai[p.z][p.y][p.x][p.b] = n;
            dist[p.z][p.y][p.x][p.b] = d+1;
          }
          break;

        default:;
      }
    }

    if (predio->m[n.z][n.y][n.x] == 'T' && n.b < predio->B) {
      p = n;
      p.b++;
      if (dist[p.z][p.y][p.x][p.b] == 0x3f3f3f3f) {
        fila_push(&q, p);
        pai[p.z][p.y][p.x][p.b] = n;
        dist[p.z][p.y][p.x][p.b] = d+1;
      }
    }
    if (predio->m[n.z][n.y][n.x] == 'D'
        || predio->m[n.z][n.y][n.x] == 'E') {
      p = n;
      p.z--;
      if (dist[p.z][p.y][p.x][p.b] == 0x3f3f3f3f) {
        fila_push(&q, p);
        pai[p.z][p.y][p.x][p.b] = n;
        dist[p.z][p.y][p.x][p.b] = d+1;
      }
    }
    if (predio->m[n.z][n.y][n.x] == 'U'
        || predio->m[n.z][n.y][n.x] == 'E') {
      p = n;
      p.z++;
      if (dist[p.z][p.y][p.x][p.b] == 0x3f3f3f3f) {
        fila_push(&q, p);
        pai[p.z][p.y][p.x][p.b] = n;
        dist[p.z][p.y][p.x][p.b] = d+1;
      }
    }
  }

  r = 0x3f3f3f3f;
  for (i = 0; i <= predio->B; i++) {
    r = min(r, dist[fim.z][fim.y][fim.x][i]);
  }

  return (r == 0x3f3f3f3f? -1 : r);
}

Passos *predio_resolve(const Edificio *predio) {
  int i, j, k;
  Pos ini, fim;
  Passos *resp = (Passos *) malloc(sizeof(Passos));

  ini.x = ini.y = ini.z = ini.b = fim.x = fim.y = fim.z = fim.b = 0;

  for (i = 0; i < predio->A; i++) {
    for (j = 0; j < predio->H; j++) {
      for (k = 0; k < predio->W; k++) {
        if (predio->m[i][j][k] == 'J') {
          ini.x = k;
          ini.y = j;
          ini.z = i;
        }
        else if (predio->m[i][j][k] == 'S') {
          fim.x = k;
          fim.y = j;
          fim.z = i;
        }
      }
    }
  }

  sol_size = bfs(ini, fim, predio);
  get_solution(fim);
  resp->len = sol_size;
  resp->p = (Passo *) malloc(sizeof(Passo) * sol_size);

  for (i = 0; i < sol_size; i++) {
    if (solution[i+1].x > solution[i].x) {
      resp->p[i] = PASSO_DIREITA;
    }
    else if (solution[i+1].x < solution[i].x) {
      resp->p[i] = PASSO_ESQUERDA;
    }
    else if (solution[i+1].y < solution[i].y) {
      resp->p[i] = PASSO_CIMA;
    }
    else if (solution[i+1].y > solution[i].y) {
      resp->p[i] = PASSO_BAIXO;
    }
    else if (solution[i+1].z > solution[i].z) {
      resp->p[i] = PASSO_SOBE;
    }
    else if (solution[i+1].z < solution[i].z) {
      resp->p[i] = PASSO_DESCE;
    }
    else {
      resp->p[i] = PASSO_ENCHE;
    }
  }

  return resp;
}

void predio_free_passos(Passos *p) {
  free(p->p);
  free(p);
}
