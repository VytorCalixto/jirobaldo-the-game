CFLAGS=-Wall -O2 `sdl2-config --cflags` `pkg-config --cflags SDL2_image SDL2_ttf` 
LDLIBS=`sdl2-config --libs` `pkg-config --libs SDL2_image SDL2_ttf`

all: main
