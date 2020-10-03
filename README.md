# Hexagonal_Pathfinding

implémentation d'une grille hexagonale, gestion de l'affichage, position des cases sur l'écran ...

implémentation d'un algorithme de recherche du plus court chemin utilisant A*

utiliser la commande make pour générer l'executable

nécessite SDL2
apt-get install libsdl2-dev
mkdir build
make
./hexa

Le programme de test affiche une grille sur une petite fenêtre, cliquer sur une case va la rendre inaccessible.
Le chemin proposé par A* est recalculé à chaque changement sur la grille, les couleurs représentent :
- jaune: cases visitées
- vert: cases du chemin
- rouge: cases inaccessibles
- bleu: default
