#ifndef QUARTIER
#define QUARTIER



#include "graphe.h"
#include "algorithmes.h"

Graphe* generer_quartier(int nb_pts_route, int nb_maison, int dispertion, int x, int y, int conso_moyenne = 1, int dispertion_conso = 1);
Graphe* generer_routes(Graphe* ville, Graphe* quartier, int nb_sommets_inter, int dispertion);



#endif