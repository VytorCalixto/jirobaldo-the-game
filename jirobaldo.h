#ifndef JIROBALDO_H_INLCUDED
#define JIROBALDO_H_INLCUDED

//Direções para onde o Jirobaldo está olhando
#define FACE_NORTH 0
#define FACE_WEST 1
#define FACE_SOUTH 2
#define FACE_EAST 3

typedef struct{
	int sx, sy, sz; //Posição inicial
	int x, y, z; //Posição atuais
	int baldes; //Baldes atuais
	int MAX_BALDES; //Qtd máxima de baldes
	int frame, face; //Animação
	bool isAnimating;
	SDL_Texture *texture;
	SDL_Rect jirobaldoRect[4][9];
	SDL_Rect danceRect[4][7];
}Jirobaldo;

//Carrega as texturas do Jirobaldo e define os SDL_Rect's necessários
void carregarTexturasJirobaldo(SDL_Renderer *screen, Jirobaldo *jirobaldo);
//Renderiza o Jirobaldo
void renderJirobaldo(SDL_Renderer *screen, Jirobaldo *jirobaldo, SDL_Rect aux);
void renderDanceJirobaldo(SDL_Renderer *screen, Jirobaldo *jirobaldo, SDL_Rect aux);

#endif