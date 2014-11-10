CFLAGS= -Wall -O2 `sdl2-config --cflags --libs` `pkg-config --cflags SDL2_image SDL2_ttf` 
LDLIBS=`sdl2-config --libs` `pkg-config --libs SDL2_image SDL2_ttf`

all: main.o buscaLargura.o planta.o fila.o predio.o jirobaldo.o
	gcc main.o buscaLargura.o planta.o fila.o predio.o jirobaldo.o $(CFLAGS) $(LDLIBS) -o fogo2

main.o: main.c
	gcc -c main.c $(CFLAGS) $(LDLIBS) -o main.o

buscaLargura.o: buscaLargura.c
	gcc -c buscaLargura.c $(CFLAGS) $(LDLIBS)

planta.o: planta.c
	gcc -c planta.c $(CFLAGS) $(LDLIBS)

fila.o: fila.c
	gcc -c fila.c $(CFLAGS) $(LDLIBS)

predio.o: predio.c
	gcc -c predio.c $(CFLAGS) $(LDLIBS)

jirobaldo.o: jirobaldo.c
	gcc -c jirobaldo.c $(CFLAGS) $(LDLIBS)

clean:
	rm -rf *.o