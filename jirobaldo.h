#ifndef JIROBALDO_H_INLCUDED
#define JIROBALDO_H_INLCUDED

//Direções para onde o Jirobaldo está olhando
#define FACE_NORTH 0
#define FACE_WEST 1
#define FACE_SOUTH 2
#define FACE_EAST 3

typedef struct{
	int x, y, z; //Posição atuais
	int baldes; //Baldes atuais
	int MAX_BALDES; //Qtd máxima de baldes
	int frame, face; //Animação
	bool isAnimating;
	SDL_Texture *texture;
	SDL_Rect jirobaldoRect[4][9];
}Jirobaldo;

void carregarTexturasJirobaldo(SDL_Renderer *screen, Jirobaldo *jirobaldo);
void renderJirobaldo(SDL_Renderer *screen, Jirobaldo *jirobaldo, SDL_Rect aux);

#endif