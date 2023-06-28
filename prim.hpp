#ifndef PRIM
#define PRIM

#include <iostream>
#include <cassert>

#include "graphe.h"
#include "union_find.h"

std::vector<Arete*> get_voisins_ensemble(Graphe* g, std::vector<Sommet*> s, bool* deja_voisins);
void prim(Graphe* g, Sommet* s, Graphe* acpd);
void prim_v(Graphe* g, Graphe* acpd);
void emonder(Graphe* acpd);
void post_traitement(Graphe* acpd);



#endif