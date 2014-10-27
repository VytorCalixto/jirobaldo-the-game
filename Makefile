CFLAGS= -Wall -O2 `sdl2-config --cflags --libs` `pkg-config --cflags SDL2_image SDL2_ttf` 
LDLIBS=`sdl2-config --libs` `pkg-config --libs SDL2_image SDL2_ttf`

all: fogo2.o predio.o jirobaldo.o
	gcc fogo2.o predio.o jirobaldo.o $(CFLAGS) $(LDLIBS) -o fogo2

fogo2.o: fogo2.c
	gcc -c fogo2.c $(CFLAGS) $(LDLIBS)

predio.o: predio.c
	gcc -c predio.c $(CFLAGS) $(LDLIBS)

jirobaldo.o: jirobaldo.c
	gcc -c jirobaldo.c $(CFLAGS) $(LDLIBS)

clean:
	rm -rf *.o