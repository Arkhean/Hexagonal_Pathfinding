#ifndef PATHFINDING_HEX_H_INCLUDED
#define PATHFINDING_HEX_H_INCLUDED

typedef struct noeud noeud; //structure de case pour le chemin
struct noeud {
    // on conserve toutes ces informations car l'algo est amené à faire
    // machine arrière de temps à autres s'il suit une fausse piste
    int *distance;      // liste des distances euclidiennes jusqu'à l'arrivé
    int *noeud_avant;   // liste origine de la case
    int *hex_actuel;    // liste numero de la case actuelle
    int *check;         // liste si la case à été traitée
    int *inlist;        // liste si dans la liste
    int *pas;           // liste nombre de cases traversées
};

// colorie la route du départ vers l'arrivée s'il en trouve une
int pathfinding(int depart, int arrivee, int n, int a[]);

// ajoute les hex voisins de la position actuelle à la liste s'ils n'y sont pas
// et s'ils sont accessibles
int add_voisin (int hex, noeud *liste, int n, int arrivee, int parent, int last, int a[]);

// retourne le numero de l'hex le plus proche du point d'arrivée parmi ceux dans
// tab
int noeud_optimal(noeud tab, int n);

// retourne le numero du ième voisin de hex, où -1 si dans le vide (bordure)
int get_voisin (int hex, int i, int n);


#endif // PATHFINDING_HEX_H_INCLUDED
