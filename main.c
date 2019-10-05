/*
 * auteur : Julien Miens
 * revue : 09/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "HexagonalGrid.h"
#include "pathfinding_hex.h"
#include "grid_interactions.h"


int main(int argc, char *argv[]){

    ///initialisation SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1){
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    int w = 900;    // largeur fenetre
    int h = 700;    // hauteur fenetre

    SDL_Window *screen = SDL_CreateWindow("Hexa", SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            w, h, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1,
                        SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);


    ///variables principales du main
    int continuer = 1;
    SDL_Event event;

    // initialiser grille
    generate_grid(w, h, renderer);

    SDL_RenderCopy(renderer, get_map(), NULL, NULL);
    SDL_RenderPresent(renderer);

    ///boucle de jeu
    while (continuer){
        // gestion des evenements
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT :
                    continuer = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN : // clic souris
                    grid_click(event.button.x, event.button.y, w, h, renderer);
                    break;
                case SDL_MOUSEMOTION : // mouvement souris
                    grid_motion(event.button.x, event.button.y, w, h, renderer);
                    break;
            }
        }
        SDL_Delay(50);
    }

    // liberation memoire
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    free_grid(nb_hex(w,h));
    SDL_Quit();
    return 0;
}
