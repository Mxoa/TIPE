#ifndef VILLE_CARREE_HPP
#define VILLE_CARREE_HPP

#include "graphe.h"
#include "algorithmes.h"
#include "file_de_priorite.hpp"
#include <climits>

// Construit le graphe d'une ville carrée et d'un lotissement carré de coté n * dimension
Graphe gen_graphe_grille(int n, int dimension, int conso, int x0, int y0);

// ok
void consommations_coherentes(Graphe* g, float proba_maison, int conso, int dispertion);

// Ajuste les flots pour qu'ils respectent la loi des noeuds, il faut que ce soit possible par contre
// Il faut qu'il n'y ait qu'une seule source
// Le graphe g est modifié (ses flots et ses capacités)
void ajuste_flot(Graphe* g, int surplus = 3);

#endif