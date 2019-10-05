#ifndef PIXEL_H_INCLUDED
#define PIXEL_H_INCLUDED

// retourne la couleur d'un pixel sur une surface sous forme de Uint32
Uint32 obtenirPixel (SDL_Surface * surface, int x, int y);

// colore le pixel (x, y) sur la surface avec la couleur fournie Uint32
void definirPixel (SDL_Surface * surface, int x, int y, Uint32 pixel);

// dessine sur la surface une ligne reliant les points (xa, ya) et (xb, yb)
// en utilisant la couleur fournie
void relier_pts (SDL_Surface *surface, int xa, int xb, int ya, int yb, Uint32 color);

// colorie une surface reccursivement depuis (x, y) en remplacement chaque pixel
// de couleur colcible par un autre de couleur colrep, se propage de maniere
// contigue
void pot_de_peinture (int x, int y, Uint32 colcible, Uint32 colrep, SDL_Surface * surface);


#endif // PIXEL_H_INCLUDED
