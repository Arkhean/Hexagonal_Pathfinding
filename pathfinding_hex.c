/*
 * auteur : Julien Miens
 * revue : 09/2019
 * objectif : calcul du plus court chemin en utilisant A*
 * optimisé pour les grilles hexagonales mais adapable en modifiant add_voisin
 * non optimisé en complexité : pas de tas...
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "HexagonalGrid.h"
#include "pathfinding_hex.h"

// chaque hexagone de la grille est "numerote", on peut ainsi se reperer
// juste en connaissant son numero
int pathfinding(int depart, int arrivee, int n, int a[]) {
    reset_hex(n);

    // initialisation
    int best = 0;
    int last = 0;
    noeud liste;

    liste.check = calloc(n, sizeof(int));
    liste.inlist = calloc(n, sizeof(int));
    liste.pas= calloc(n, sizeof(int));
    liste.distance = malloc(n * sizeof(int));
    liste.hex_actuel = malloc(n * sizeof(int));
    liste.noeud_avant = malloc(n * sizeof(int));

    liste.hex_actuel[0] = depart;
    liste.distance[0] = ligne(depart,arrivee);
    liste.inlist[depart] = 1;
    liste.check[depart] = 1;

    // recherche d'un chemin
    // tant qu'on n'est pas arrivé
    while (liste.hex_actuel[best] != arrivee) {
        // l'hexagone actuel est vérifié
        liste.check[liste.hex_actuel[best]] = 1;
        // on prend en considération ces voisins
        last = add_voisin(liste.hex_actuel[best], &liste, n, arrivee, best, last, a);
        // on cherche l'hex le plus proche de l'arrivée parmi tous ceux dans la liste
        best = noeud_optimal(liste, last);
        if (best == -1) {
            // plus aucune possibilité, on quitte
            return -1;
        }
        if (last > n){ printf("erreur !\n"); SDL_Delay(2000); }
    }
    ///si on est là, on a trouvé !
    int i = best;
    // on parcours les cases à l'envers pour remonter jusqu'au départ
    do {
        // au passage, on dessine la route
        set_road(liste.hex_actuel[i]);
        i = liste.noeud_avant[i];
    } while(liste.hex_actuel[i] != depart);
    set_road(liste.hex_actuel[i]);

    // nettoyage mémoire
    free(liste.check);
    free(liste.distance);
    free(liste.hex_actuel);
    free(liste.noeud_avant);
    free(liste.inlist);
    free(liste.pas);

    return 0;
}

int add_voisin (int hex, noeud *liste, int n, int arrivee, int parent, int last, int a[]) {
    // on parcours les 6 cases autour de hex
    for(int j = 0; j < 6; j++) {
        int i = get_voisin(hex, j, n);
        // si le voisin existe et s'il est accessible
        // et s'il n'est pas déjà ajouté
        if (i!= -1 && a[i] != 0 && liste->inlist[i] == 0) {
            last++; // on "agrandit" la liste
            liste->hex_actuel[last] = i;
            liste->noeud_avant[last] = parent;
            liste->inlist[i] = 1;
            // permet de conserver une trace du passage de l'algo
            set_test(liste->hex_actuel[last], 1);
            // on a avancé d'une case
            liste->pas[last] = liste->pas[liste->noeud_avant[last]] + 1;
            // on calcule la distance euclidienne jusqu'à l'arrivé
            // + une pondération par rapport au nombre de cases franchies
            liste->distance[last] = ligne(i,arrivee) + liste->pas[last]*40;
        }
    }
    return last;
}

int noeud_optimal(noeud tab, int n) {
    static int minp;
    int minc = -1;
    for(int i = 0; i < n + 1; i++) {
        if ( (minc == -1) && (tab.check[tab.hex_actuel[i]] == 0)) {
            minc = i;
        }
        else {
            if ( (tab.distance[i] < tab.distance[minc])
                    && (tab.check[tab.hex_actuel[i]] == 0) ) {
                minc = i;
            }
        }
    }

    if (minp == minc) {
        printf("erreur min ! %d\n", minc);
        SDL_Delay(300);
    }
    minp = minc;
    if (minc == -1) {
        // aucun chemin possible
        printf("no way\n");
        return -1;
    }
    return minc;
}

int get_voisin (int hex, int i, int n) {
    // les hexagones sont considérés comme sur un damier dont une ligne sur
    // deux seraient décalées d'une demi case
    // donc selon la parité la ligne, les dx et dy à faire ne sont pas les mêmes
    int dx_pair[6] = {-1, 0, 1, 0, -1, -1};
    int dx_impair[6] = {0, 1, 1, 1, 0, -1};
    int dy[6] = {-1, -1, 0, 1, 1, 0};
    int x = get_x(hex);
    int y = get_y(hex);

    if (y%2 == 0) {
        return get_hexa(x + dx_pair[i], y + dy[i], n);
    }
    else {
        return get_hexa(x + dx_impair[i], y + dy[i], n);
    }
    return -1;
}
