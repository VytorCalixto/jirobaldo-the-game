CFLAGS=-Wall -O2 `sdl2-config --cflags --libs` `pkg-config --cflags SDL2_image SDL2_ttf SDL2_mixer` 
LDLIBS=`sdl2-config --libs` `pkg-config --libs SDL2_image SDL2_ttf SDL2_mixer`

all: main