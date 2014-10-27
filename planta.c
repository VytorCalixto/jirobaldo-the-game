//Serve para marcar os vértices em que já passamos. 0 caso não tenha passado por lá.
typedef struct{
    int *** planta;
}Planta;

//Aloca a planta com o mesmo tamanho do prédio e define todas as posições como 0
void iniciaPlanta(Planta *planta, Predio *predio){
    int i, j, k;
    planta->planta = (int ***) malloc(sizeof(int **) * predio->h);

    for(i = 0; i < predio->h; i++){
        planta->planta[i] = (int **) malloc(sizeof(int *) * predio->w);
        for(j = 0; j < predio->w; j++){
            planta->planta[i][j] = (int *) malloc(sizeof(int) * predio->altura);
            for(k = 0; k < predio->altura; k++){
                planta->planta[i][j][k] = 0;
            }
        }
    }
}

//Marca que um vértice foi enfileirado
void marcaVertice(Planta *planta, Vertice v){
    planta->planta[v.x][v.y][v.z] = v.tempo;
}

//Retorna true se o vértice já tiver sido marcado, false caso contrário
bool isVerticeMarcado(Planta *planta, Vertice v){
    return (planta->planta[v.x][v.y][v.z]) && (v.tempo >= planta->planta[v.x][v.y][v.z]); 
}

void imprimePlanta(Planta planta, Predio *predio){
    int i, j, k;
    for(i = 0; i < predio->altura; i++){
        for(j = 0; j < predio->h; j++){
            for(k = 0; k<predio->w; k++){
                printf("%d \t", planta.planta[j][k][i]); //Imprime 0s e 1s
            }
            printf("\n");
        }
        printf("\n");
    }
}
