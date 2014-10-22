#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int main(int argc, char **argv){
    int stepsPerSecond = 0;
    bool isModoResolvedor = false;
    int c;

    while( (c = getopt(argc, argv, "f:s")) != -1 ){
        switch(c){
            case 'f':
                stepsPerSecond = atoi(optarg);
                break;
            case 's':
                isModoResolvedor = true;
                break;
        }
    }

    printf("PPS: %d, MR: %d\n", stepsPerSecond, isModoResolvedor);
    
    int index;
    for (index = optind; index < argc; index++)
            printf ("Non-option argument %s\n", argv[index]);
    return 0;
}
