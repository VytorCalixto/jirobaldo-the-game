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

SDL_Texture *chao, *parede, *fogo, *escadaCima, *escadaBaixo, *torneira, *jirobaldo;
SDL_Rect chaoRect, paredeRect, fogoRect, escadaCimaRect, escadaBaixoRect, torneiraRect, jirobaldoRect;

bool novoPredio(Predio *predio, char *filePath);
//Aloca o prédio
void alocaPredio(Predio *predio);
void imprimePredio(Predio *predio);
//Lê a situação do prédio (mapa)
void lePredio(Predio *predio, FILE *file);
//Retorna true caso o ponto (x, y, z) esteja no prédio, false caso contrário
bool isPontoNoPredio(Predio *predio, int x, int y, int z);
//Carrega as texturas/imagens
void carregarTexturasPredio(SDL_Renderer *screen);
//Mostra graficamente um andar do prédio
void renderAndarPredio(Predio *predio, int andar, SDL_Renderer *screen);

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

void carregarTexturasPredio(SDL_Renderer *screen){
	SDL_Surface *tmp;

	tmp = IMG_Load("data/skorpios-pack/Interior-Furniture.png");
	chao = SDL_CreateTextureFromSurface(screen, tmp);
	chaoRect.x = 6*32;
	chaoRect.y = 2*32;
	chaoRect.w = 32;
	chaoRect.h = 32;
	SDL_FreeSurface(tmp);

	tmp = IMG_Load("data/skorpios-pack/Interior-Walls-Beige.png");
	parede = SDL_CreateTextureFromSurface(screen, tmp);
	paredeRect.x = 0;
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

}

void renderAndarPredio(Predio *predio, int andar, SDL_Renderer *screen){
	SDL_Rect aux;
	aux.w = 512/predio->w;
	aux.h = 512/predio->h;
	int i, j;
	for(i = 0; i < predio->h; i++){
		for(j = 0; j < predio->w; j++){
			aux.y = i * 32;
			aux.x = j * 32;
			char c = predio->pisos[andar].pontos[i][j];
			switch(c){
				case '.':
					SDL_RenderCopy(screen, chao, &chaoRect, &aux);
					break;
				case '#':
					SDL_RenderCopy(screen, parede, &paredeRect, &aux);
					break;
				case 'T':
					SDL_RenderCopy(screen, chao, &chaoRect, &aux);
					SDL_RenderCopy(screen, torneira, &torneiraRect, &aux);
					break;
				case 'F':
					SDL_RenderCopy(screen, chao, &chaoRect, &aux);
					SDL_RenderCopy(screen, fogo, &fogoRect, &aux);
					break;
				case 'J':
					SDL_RenderCopy(screen, chao, &chaoRect, &aux);
					SDL_RenderCopy(screen, jirobaldo, &jirobaldoRect, &aux);
					break;
			}
			// printf("%c", predio->pisos[andar].pontos[i][j]);
		}
		// puts("");
	}
}

bool isPontoNoPredio(Predio *predio, int x, int y, int z){
	return ((x >= 0 && x < predio->h) && (y >= 0 && y < predio->w) && (z >= 0 && z <predio->altura));
}