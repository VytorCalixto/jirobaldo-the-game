typedef struct{
    char **pontos;
}Pavimento;

typedef struct{
    int x,y,z,baldes;
}Jirobaldo;

typedef struct{
    Pavimento *pisos;
    int altura, h, w;
    Jirobaldo jirobaldo;
}Predio;

SDL_Rect chaoRect, paredeRect, fogoRect, escadaAcimaRect, escadaAbaixoRect, torneiraRect, saidaRect, jirobaldoRect;

bool novoPredio(Predio *predio, char *filePath);
//Aloca o prédio
void alocaPredio(Predio *predio);
void imprimePredio(Predio *predio);
//Lê a situação do prédio (mapa)
void lePredio(Predio *predio, FILE *file);
//Retorna true caso o ponto (x, y, z) esteja no prédio, false caso contrário
bool isPontoNoPredio(Predio *predio, int x, int y, int z);
//Carrega as texturas/imagens
void carregarTexturasPredio();
//Mostra graficamente um andar do prédio
void renderAndarPredio(Predio *predio, int andar);
void renderParedes(Pavimento *pavimento, int x, int y, int w, int h);

bool novoPredio(Predio *predio, char *filePath){
    FILE *file;
    if( (file = fopen(filePath, "r")) != NULL){
        fscanf(file," %d %d %d %d", &predio->altura, &predio->w, &predio->h, &predio->jirobaldo.baldes);
        lePredio(predio, file);
        return true;
    }
    return false;
}

//Lê a planta do prédio
void lePredio(Predio *predio, FILE *file){
    int i, j, k;
    alocaPredio(predio);

    for(i = 0; i < predio->altura; i++){
        for(j = 0; j < predio->h; j++){
            for(k = 0; k < predio->w; k++){
                fscanf(file, " %c", &predio->pisos[i].pontos[j][k]);
                if(predio->pisos[i].pontos[j][k] == 'J'){
                    predio->jirobaldo.x = j;
                    predio->jirobaldo.y = k;
                    predio->jirobaldo.z = i;
                }
            }
        }
    }
}

//Aloca em memória o espaço necessário para a planta do prédio
void alocaPredio(Predio *predio){
    int i, j;
    predio->pisos = (Pavimento *) malloc(sizeof(Pavimento) * predio->altura);
    if(predio->pisos == NULL){
        puts("Erro na alocação");
        exit(1);
    }

    for(i = 0; i < predio->altura; i++){
        predio->pisos[i].pontos = (char **) malloc(sizeof(char*) * predio->h);
        if(predio->pisos[i].pontos == NULL){
            puts("Erro na alocação");
            exit(1);
        }
        for(j = 0; j < predio->h; j++){
            predio->pisos[i].pontos[j] = (char *) malloc(sizeof(char) * predio->w);
            if(predio->pisos[i].pontos[j] == NULL){
                puts("Erro na alocação");
                exit(1);
            }
        }
    }
}

void imprimePredio(Predio *predio){
    int i, j, k;
    for(i = 0; i < predio->altura; i ++){
        for(j = 0; j < predio->h; j++){
            for(k = 0; k < predio->w; k++){
                printf("%c", predio->pisos[i].pontos[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void carregarTexturasPredio(){
    SDL_Surface *tmp;

    tmp = IMG_Load("data/skorpios-pack/Interior-Furniture.png");
    chao = SDL_CreateTextureFromSurface(screen, tmp);
    chaoRect.x = 6*32;
    chaoRect.y = 2*32;
    chaoRect.w = 32;
    chaoRect.h = 32;
    
    saida = SDL_CreateTextureFromSurface(screen, tmp);
    saidaRect.x = 7*32;
    saidaRect.y = 5*32;
    saidaRect.w = 32;
    saidaRect.h = 32;
    SDL_FreeSurface(tmp);

    tmp = IMG_Load("data/skorpios-pack/Interior-Walls-Beige.png");
    parede = SDL_CreateTextureFromSurface(screen, tmp);
    paredeRect.x = 32;
    paredeRect.y = 0;
    paredeRect.w = 32;
    paredeRect.h = 32;
    SDL_FreeSurface(tmp);

    tmp = IMG_Load("data/skorpios-pack/CampFireFinished.png");
    fogo = SDL_CreateTextureFromSurface(screen, tmp);
    fogoRect.x = 0;
    fogoRect.y = 0;
    fogoRect.w = 64;
    fogoRect.h = 64;
    SDL_FreeSurface(tmp);

    tmp = IMG_Load("data/skorpios-pack/Lamp_alternative.png");
    torneira = SDL_CreateTextureFromSurface(screen, tmp);
    torneiraRect.x = 0;
    torneiraRect.y = 0;
    torneiraRect.w = 32;
    torneiraRect.h = 32;
    SDL_FreeSurface(tmp);

    tmp = IMG_Load("data/skorpios-pack/Characters/Character 1/Walk/Character1Walk.png");
    jirobaldo = SDL_CreateTextureFromSurface(screen, tmp);
    jirobaldoRect.x = 0;
    jirobaldoRect.y = 2*64;
    jirobaldoRect.w = 64;
    jirobaldoRect.h = 64;
    SDL_FreeSurface(tmp);

    tmp = IMG_Load("data/skorpios-pack/Street.png");
    escadaAcima = SDL_CreateTextureFromSurface(screen, tmp);
    escadaAcimaRect.x = 16*32;
    escadaAcimaRect.y = 16*32;
    escadaAcimaRect.w = 64;
    escadaAcimaRect.h = 32;

    escadaAbaixo = SDL_CreateTextureFromSurface(screen, tmp);
    escadaAbaixoRect.x = 16*32;
    escadaAbaixoRect.y = 16*32;
    escadaAbaixoRect.w = 64;
    escadaAbaixoRect.h = 32;
    SDL_FreeSurface(tmp);
}

void renderAndarPredio(Predio *predio, int andar){
    SDL_Rect aux;
    aux.h = (SCREEN_HEIGHT - 20)/predio->h;
    aux.w = aux.h;
    int i, j;
    for(i = 0; i < predio->h; i++){
        for(j = 0; j < predio->w; j++){
            aux.y = i * aux.h;
            aux.x = j * aux.w;
            char c = predio->pisos[andar].pontos[i][j];
            //Coloca chão em tudo
            SDL_RenderCopy(screen, chao, &chaoRect, &aux);
            
            //Coisas que ficam atrás do Jirobaldo
            if(c == '#'){
                /*if(j == 0 || 
                        (isPontoNoPredio(predio, i, j-1, andar)
                         && predio->pisos[andar].pontos[i][j-1] != '#'
                         && predio->pisos[andar].pontos[i][j+1] == '#') ){
                    paredeRect.x = 4*32;
                    paredeRect.y = 4*32;
                }else if(j > 0 && (j+1) < predio->w){
                    char esq = predio->pisos[andar].pontos[i][j-1];
                    char dir = predio->pisos[andar].pontos[i][j+1];
                    if(esq != '#' && dir !='#'){
                        if(i > 0 
                            && predio->pisos[andar].pontos[i-1][j-1] == '#'
                            && predio->pisos[andar].pontos[i-1][j] == '#'
                            && predio->pisos[andar].pontos[i-1][j+1] == '#'){
                            paredeRect.x = 7*32;
                            paredeRect.y = 32;
                        }else{
                            paredeRect.x = 7*32;
                            paredeRect.y = 2*32;
                        }
                    }else{
                        paredeRect.x = 32;
                        paredeRect.y = 0;
                    }
                }else{
                    paredeRect.x = 32;
                    paredeRect.y = 0;
                }
                SDL_RenderCopy(screen, parede, &paredeRect, &aux);*/
                renderParedes(&predio->pisos[andar], i, j, predio->w, predio->h);
            }else if(c == 'U'){
                SDL_RenderCopy(screen, escadaAcima, &escadaAcimaRect, &aux);
            }else if(c == 'D'){
                SDL_RenderCopy(screen, escadaAbaixo, &escadaAbaixoRect, &aux);
            }else if(c == 'E'){
                SDL_RenderCopy(screen, escadaAbaixo, &escadaAbaixoRect, &aux);
                SDL_RenderCopy(screen, escadaAcima, &escadaAcimaRect, &aux);
            }

            //Jirobaldo
            if(predio->jirobaldo.x == i && predio->jirobaldo.y == j){
                SDL_RenderCopy(screen, jirobaldo, &jirobaldoRect, &aux);
            }
            
            //Coisas que vão na frente do Jirobaldo
            if(c == 'T'){
                SDL_RenderCopy(screen, torneira, &torneiraRect, &aux);
            }else if(c == 'F'){
                SDL_RenderCopy(screen, fogo, &fogoRect, &aux);
            }else if(c == 'S'){
                SDL_RenderCopy(screen, saida, &saidaRect, &aux);
            }
        }
    }
}

void renderParedes(Pavimento *pavimento, int x, int y, int w, int h){
    SDL_Rect aux;
    aux.h = (SCREEN_HEIGHT - 20)/h; //-100px da barra de status
    aux.w = aux.h;
    aux.x = y * aux.w;
    aux.y = x * aux.h;

    if(y == 0 && pavimento->pontos[x][y+1] == '#'){
        paredeRect.x = 4*32;
        paredeRect.y = 4*32;
    }else if(y == w - 1 && pavimento->pontos[x][y-1] == '#'){
        paredeRect.x = 6*32;
        paredeRect.y = 4*32;
    }else if(y > 0 && y + 1 < w){
        if(pavimento->pontos[x][y-1] == '#' && pavimento->pontos[x][y+1] == '#'){
            paredeRect.x = 32;
            paredeRect.y = 0;
        }else if(pavimento->pontos[x][y+1] == '#'){
            paredeRect.x = 4*32;
            paredeRect.y = 4*32;
        }else if(pavimento->pontos[x][y-1] == '#'){
            paredeRect.x = 6*32;
            paredeRect.y = 4*32;
        }
    }else{
        paredeRect.x = 32;
        paredeRect.y = 0;
    }
    SDL_RenderCopy(screen, parede, &paredeRect, &aux);

    if(x == 0 && pavimento->pontos[x+1][y] == '#'){
        paredeRect.x = 7*32;
        paredeRect.y = 0;
    }else if(x == h - 1 && pavimento->pontos[x-1][y] == '#'){
        paredeRect.x = 5*32;
        paredeRect.y = 32;
    }else if(x > 0 && x + 1 < h){
        if(pavimento->pontos[x+1][y] == '#'){
            paredeRect.x = 7*32;
            paredeRect.y = 0;
        }else if(pavimento->pontos[x+1][y] != '#' && pavimento->pontos[x-1][y] == '#'){
            paredeRect.x = 7*32;
            paredeRect.y = 32;
        }
    }else{
        paredeRect.x = 7*32;
        paredeRect.y = 0;
    }
    SDL_RenderCopy(screen, parede, &paredeRect, &aux);
}

bool isPontoNoPredio(Predio *predio, int x, int y, int z){
    return ((x >= 0 && x < predio->h) && (y >= 0 && y < predio->w) && (z >= 0 && z <predio->altura));
}
