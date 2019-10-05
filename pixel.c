#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Uint32 obtenirPixel (SDL_Surface * surface, int x, int y){
    int nbopp = surface->format->BitsPerPixel/8;
    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * nbopp;

    switch (nbopp) {
        case 1 :
            return *p;
        case 2 :
            return *(Uint16*)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0]<<16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4 :
            return *(Uint32*)p;
        default :
            return 0;
    }
}

void definirPixel (SDL_Surface * surface, int x, int y, Uint32 pixel){
    int nbopp = surface->format->BitsPerPixel/8;
    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * nbopp;

    switch (nbopp) {
        case 1 :
            *p = pixel;
            break;
        case 2 :
            *(Uint16*)p = pixel;
            break;
        case 3 :
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN){
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4 :
            *(Uint32*)p = pixel;
            break;
    }
}


void relier_pts(SDL_Surface *surface, int xa, int xb, int ya, int yb, Uint32 color){
    int x, y, dx, dy, xi, yi, e;
    dx = abs(xb-xa);
    dy = abs(yb-ya);

    if (xa < xb) {
        xi = 1;
    }
    else {
        xi = -1;
    }
    if (ya < yb) {
        yi = 1;
    }
    else {
        yi = -1;
    }

    x = xa;
    y = ya;

    if (dx > dy) {
        e = dx/2;
        for (int i = 0; i < dx; i++) {
            x = x + xi;
            e = e + dy;
            if (e > dx) {
                e = e - dx;
                y = y + yi;
            }
            definirPixel(surface, x, y, color);
        }
    }
    else {
        e = dy/2;
        for (int i = 0; i < dy; i++) {
            y = y + yi;
            e = e + dx;
            if (e > dy) {
                e = e - dy;
                x = x + xi;
            }
            definirPixel(surface, x, y, color);
        }
    }
}

void pot_de_peinture(int x, int y, Uint32 colcible, Uint32 colrep, SDL_Surface * surface)
{
    if (obtenirPixel(surface, x, y) == colcible) {
        definirPixel(surface, x, y, colrep);
        pot_de_peinture(x, y-1, colcible, colrep, surface);
        pot_de_peinture(x, y+1, colcible, colrep, surface);
        pot_de_peinture(x-1, y, colcible, colrep, surface);
        pot_de_peinture(x+1, y, colcible, colrep, surface);
    }
}
