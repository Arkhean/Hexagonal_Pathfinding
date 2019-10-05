#ifndef HEXAGON_H_INCLUDED
#define HEXAGON_H_INCLUDED

typedef struct gridhex gridhex;
// structure qui contient toute la grille
struct gridhex {
    // x et y sont les coordonnées dans la grille
    int *x;                 // ensemble des positions x des hex
    int *y;                 // ensemble des positions y
    int *color;
    SDL_Surface *map;
    SDL_Texture *map_tex;
    int *region;
    int *acces;             // 1 si une case est accessible, -1 bloquée
    int *test;              // sorte de mémoire pour visualisation
    int *road;              // 1 si la case fait partie de la route dessinée
};

///constructeurs
int nb_hex (int w, int h);
void generate_grid(int w, int h, SDL_Renderer *renderer);
void regenerate_grid(int n, SDL_Renderer *renderer);
void free_grid(int n);
///dessin
void dessin_hex(SDL_Surface *surface, int n);
void init_hex(int w, int h, int n);
void blit_map(SDL_Renderer *renderer);
void colorier(int x, int y);
void tracer_frontiere (SDL_Surface *surface, int n);
void color_grid(int n);

///se reperer
int localiser(int x, int y, int n);//donne le numero de l'hexagone contenant le pixel (x;y)
int get_hexa (int x, int y, int n);//donne le numero de l'hexagone dont le centre est en (x;y)
int ligne (int depart, int arrivee);
void chemin(int depart, int arrivee, int n);

///accesseurs
void set_region(int i, int c);
void set_road(int i);
int get_x(int i);
int get_y(int i);
int get_y_screen(int i);
int get_x_screen(int i);
void bloc_acces(int i);
void reset_hex(int n);
int get_test(int i);
int get_acces(int i);
void set_test(int i, int c);
SDL_Texture *get_map();

#endif // HEXAGON_H_INCLUDED
