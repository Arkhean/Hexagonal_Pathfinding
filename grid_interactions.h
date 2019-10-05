#ifndef GRID_INTERACTIONS_H_INCLUDED
#define GRID_INTERACTIONS_H_INCLUDED

// réaction au clic de souris sur l'écran en (x, y)
void grid_click (int x, int y, int w, int h, SDL_Renderer * renderer);

// réaction au mouvement
void grid_motion (int x, int y, int w, int h, SDL_Renderer * renderer);

#endif // GRID_INTERACTIONS_H_INCLUDED
