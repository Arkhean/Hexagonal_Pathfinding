/*
 * auteur : Julien Miens
 * revue : 09/2019
 * but : mise en place d'une grille hexagonale (dessin, gestion)
 */


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "HexagonalGrid.h"
#include "pixel.h"
#include "pathfinding_hex.h"

///variable globale
static gridhex hexa;

void generate_grid(int w, int h, SDL_Renderer *renderer) {
    int n = nb_hex(w, h);
    hexa.x = malloc(n * sizeof(int));
    hexa.y = malloc(n * sizeof(int));
    hexa.color = calloc(n, sizeof(int));
    hexa.region = calloc(n, sizeof(int));
    hexa.acces = malloc(n * sizeof(int));
    hexa.test = calloc(n, sizeof(int));
    hexa.road = calloc(n, sizeof(int));

    init_hex(w, h, n);
    hexa.map = IMG_Load("fond.tga");
    dessin_hex(hexa.map, n);
    color_grid(n);
    tracer_frontiere(hexa.map, n);
    blit_map(renderer);
}

void regenerate_grid(int n, SDL_Renderer *renderer) {
    SDL_FreeSurface(hexa.map);
    hexa.map = IMG_Load("fond.tga");
    dessin_hex(hexa.map, n);
    color_grid(n);
    tracer_frontiere(hexa.map, n);
    blit_map(renderer);
}

void init_hex(int w, int h, int n) {
    w = w / 40;
    h = h / 30;
    int x = 0;
    int y = 0;

    for (int i = 0; i < n; i++) {
        hexa.x[i] = x;
        hexa.y[i] = y;
        x++;
        if (x == (w-1) && y%2 == 1) {
            x = 0;
            y++;
        }
        else if (x == w && y%2 == 0) {
            x = 0;
            y++;
        }
        hexa.acces[i] = 1;
    }
}

int nb_hex (int w, int h) {
    int n;
    w = w / 40;
    h = h / 30;

    if (h%2 == 0) {
        n = w * (h/2) + (w-1) * (h/2);
    }
    else {
        n = w * ((h+1)/2) + (w-1) * (h/2);
    }
    return n;
}

void free_grid(int n) {
    free(hexa.x);
    free(hexa.y);
    free(hexa.color);
    free(hexa.region);
    free(hexa.acces);
    free(hexa.test);
    free(hexa.road);
    SDL_FreeSurface(hexa.map);
    SDL_DestroyTexture(hexa.map_tex);
}

void dessin_hex(SDL_Surface *surface, int n) {
    int Xa, Xb, Xc, Xd, Xe, Xf, Ya, Yb, Yc, Yd, Ye, Yf, x, y;
    Uint32 noir = SDL_MapRGB(surface->format, 50, 152, 255);

    // on va dessiner tous les hexagones
    for (int i = 0; i < n; i++) {
        x = get_x_screen(i);
        y = get_y_screen(i);
        // on calcule les 6 sommets de l'hexagone à partir de son centre
        Xa = x;
        Ya = y - 20;
        Xb = x + 20;
        Yb = y - 10;
        Xc = x + 20;
        Yc = y + 10;
        Xd = x;
        Yd = y + 20;
        Xe = x - 20;
        Ye = y + 10;
        Xf = x - 20;
        Yf = y - 10;
        // on relie les sommets
        relier_pts(surface, Xa, Xb, Ya, Yb, noir);
        relier_pts(surface, Xb, Xc, Yb, Yc, noir);
        relier_pts(surface, Xc, Xd, Yc, Yd, noir);
        relier_pts(surface, Xd, Xe, Yd, Ye, noir);
        relier_pts(surface, Xe, Xf, Ye, Yf, noir);
        relier_pts(surface, Xf, Xa, Yf, Ya, noir);
    }
}

void blit_map(SDL_Renderer *renderer) {
    SDL_DestroyTexture(hexa.map_tex);
    hexa.map_tex = SDL_CreateTextureFromSurface(renderer, hexa.map);
    SDL_RenderCopy(renderer, hexa.map_tex, NULL, NULL);
}

void colorier(int x, int y) {
    // colorie l'hexagone qui contient le pixel (x,y)
    Uint32 bleu = SDL_MapRGB(hexa.map->format, 51, 102, 255);
    Uint32 jaune = SDL_MapRGB(hexa.map->format, 255, 255, 0);
    pot_de_peinture(x, y, bleu, jaune, hexa.map);
}

int localiser(int x, int y, int n) {
    int j, k, dx, dy, d;
    // on parcours tous les hexagones
    for (int i = 0; i < n; i++) {
        j = get_x_screen(i);
        k = get_y_screen(i);

        dx = j-x;
        dy = k-y;
        // si on est dans le cercle de rayon 21 alors on est dedans
        d = sqrt( dx*dx + dy*dy );
        if (d < 21) {
            return i;
        }
    }
    return -1; // non trouvé
}

int get_hexa (int x, int y, int n) {
    // parcours de tous les hexa à la recherche de celui dont les coordonnées
    // correspondent
    for (int i = 0; i < n; i++) {
        if (hexa.x[i] == x && hexa.y[i] == y) {
            // on renvoie son numéro
            return i;
        }
    }
    return -1;
}

int ligne (int depart, int arrivee) {
    // distance euclidienne
    return sqrt( pow (get_x_screen(arrivee)-get_x_screen(depart), 2)
                        + pow (get_y_screen(arrivee)-get_y_screen(depart), 2));
}

void tracer_frontiere (SDL_Surface *surface, int n){///si ya un probleme, là tu depasses du tableau mais ça marche (bord de la grille)
    Uint32 color = SDL_MapRGBA(surface->format, 0, 0, 0, 255);

    int  x, y;
    int tab[4] = {0};
    //pour tous les hexagones
    for (int i = 0; i < n; i++) {
        if (hexa.region[i] != 0){
            // 1 : definir x et y
            x = get_x_screen(i);
            y = get_y_screen(i);
            if (hexa.y[i]%2 == 0) {
                tab[0] = -1; tab[1] = 0; tab[2] = -1; tab[3] = 0;
            }
            else {
                tab[0] = 0; tab[1] = 1; tab[2] = 0; tab[3] = 1;
            }
            // 2 : tracer les bons traits
            if (hexa.region[i-1] != hexa.region[i]){ //si la case d'avant c'est de l'eau
                relier_pts(surface, x-20, x-20, y-10, y+10, color);
                relier_pts(surface, x-19, x-19, y-10, y+10, color);
            }
            if (hexa.region[i+1] != hexa.region[i]){ //si la case d'après c'est de l'eau
                relier_pts(surface, x+20, x+20, y-10, y+10, color);
                relier_pts(surface, x+19, x+19, y-10, y+10, color);
            }
            if (hexa.region[get_hexa(hexa.x[i]+tab[0], hexa.y[i]-1, n)] != hexa.region[i]) {
                relier_pts(surface, x, x-20, y-20, y-10, color);
                relier_pts(surface, x, x-20, y-19, y-9, color);
            }
            if (hexa.region[get_hexa(hexa.x[i]+tab[1], hexa.y[i]-1, n)] != hexa.region[i]) {
                relier_pts(surface, x, x+20, y-20, y-10, color);
                relier_pts(surface, x, x+20, y-19, y-9, color);
            }
            if (hexa.region[get_hexa(hexa.x[i]+tab[2], hexa.y[i]+1, n)] != hexa.region[i]) {
                relier_pts(surface, x-20, x, y+10, y+20, color);
                relier_pts(surface, x-20, x, y+9, y+19, color);
            }
            if (hexa.region[get_hexa(hexa.x[i]+tab[3], hexa.y[i]+1, n)] != hexa.region[i]) {
                relier_pts(surface, x, x+20, y+20, y+10, color);
                relier_pts(surface, x, x+20, y+19, y+9, color);
            }
        }
    }
}

void set_region(int i, int c){
    hexa.region[i] = c;
}

void set_road(int i){
    hexa.road[i] = 1;
}

void bloc_acces(int i){
    hexa.acces[i] = (hexa.acces[i]+1)%2;
}

int get_x(int i){
    return hexa.x[i];
}

int get_y(int i){
    return hexa.y[i];
}

void chemin(int depart, int arrivee, int n){
    // si les deux points sont réellement accessibles
    if (hexa.acces[depart] == 1 && hexa.acces[arrivee] == 1) {
        // alors on cherche une route
        pathfinding(depart, arrivee, n, hexa.acces);
    }
    else {
        printf("inaccessible !\n");
    }
}

int get_x_screen(int i) {
    if (hexa.y[i]%2 == 0) {
        return hexa.x[i]*40 + 20;
    }
    return hexa.x[i]*40 + 40;
}

int get_y_screen(int i) {
    return hexa.y[i]*30 +20;
}

void color_grid(int n) {
    Uint32 bleu = SDL_MapRGB(hexa.map->format, 51, 102, 255);
    Uint32 acces_bloc = SDL_MapRGB(hexa.map->format, 255, 0, 0);
    Uint32 road = SDL_MapRGB(hexa.map->format, 0, 255, 0);
    Uint32 test = SDL_MapRGB(hexa.map->format, 255, 255, 0);
    Uint32 land = SDL_MapRGB(hexa.map->format, 0, 255, 0);

    for (int i = 0; i < n; i++) {
        if (hexa.acces[i] == 0) {
            pot_de_peinture(get_x_screen(i), get_y_screen(i), bleu, acces_bloc, hexa.map);
        }
        if (hexa.road[i] == 1) {
            pot_de_peinture(get_x_screen(i), get_y_screen(i), bleu, road, hexa.map);
        }
        if (hexa.test[i] == 1) {
            pot_de_peinture(get_x_screen(i), get_y_screen(i), bleu, test, hexa.map);
        }
        if (hexa.color[i] != 0) {
            pot_de_peinture(get_x_screen(i), get_y_screen(i), bleu, land, hexa.map);
        }
    }
}

void reset_hex(int n) {
    for(int i = 0; i < n; i++) {
        hexa.road[i] = 0;
        hexa.test[i] = 0;
        hexa.region[i] = 0;
    }
}

int get_test(int i) {
    return hexa.test[i];
}

int get_acces(int i) {
    return hexa.acces[i];
}

void set_test(int i, int c) {
    hexa.test[i] = c;
}

SDL_Texture *get_map(){
    return hexa.map_tex;
}
