CFLAGS= -Wall -O2 `sdl2-config --cflags --libs` `pkg-config --cflags SDL2_image SDL2_ttf SDL2_mixer` 
LDLIBS=`sdl2-config --libs` `pkg-config --libs SDL2_image SDL2_ttf SDL2_mixer`

all: main.o libpredio.o predio.o jirobaldo.o
	gcc main.o libpredio.o predio.o jirobaldo.o $(CFLAGS) $(LDLIBS) -o fogo2

main.o: main.c
	gcc -c main.c $(CFLAGS) $(LDLIBS) -o main.o

libpredio.o:
	gcc -c libpredio.c $(CFLAGS) $(LDLIBS)

predio.o: predio.c
	gcc -c predio.c $(CFLAGS) $(LDLIBS)

jirobaldo.o: jirobaldo.c
	gcc -c jirobaldo.c $(CFLAGS) $(LDLIBS)

clean:
	rm -rf *.o