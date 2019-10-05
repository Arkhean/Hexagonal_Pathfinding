#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "grid_interactions.h"
#include "HexagonalGrid.h"


void grid_click (int x, int y, int w, int h, SDL_Renderer * renderer) {
    // on retrouve le numéro de l'hex touché
    int l = localiser(x, y, nb_hex(w, h));
    // s'il est sur la grille
    if (x < w && y < h && l != -1) {
        bloc_acces(l);
        // par exemple entre les cases 56 et 412
        chemin(56, 412, nb_hex(w,h));
        // redessiner
        regenerate_grid(nb_hex(w,h), renderer);
        // actualiser l'écran
        SDL_RenderCopy(renderer,get_map(),NULL,NULL);
        SDL_RenderPresent(renderer);
    }
}

void grid_motion (int x, int y, int w, int h, SDL_Renderer * renderer) {
    static int case_precedente = 0;
    // on retrouve le numéro de l'hexagone touché
    int l = localiser(x, y, nb_hex(w, h));
    // s'il est sur la grille
    if (x < w && y < h && l != -1) {
        set_region(case_precedente, 0);
        case_precedente = l;
        set_region(l, 1);
        regenerate_grid(nb_hex(w,h), renderer);
        SDL_RenderCopy(renderer, get_map(), NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}
