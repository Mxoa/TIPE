#ifndef RECHERCHE_HPP
#define RECHERCHE_HPP

#include "dijkstra.hpp"
#include "dijkstra_memoire.hpp"
#include <climits>
#include "algorithmes.h"
#include "graphe.h"

/**
 * Fonction qui prend en entrée
 * - un graphe 
 * - deux sous ensembles disjoints de sommets A et B (sous forme de vecteur)
 * - un nombre de chemin nécessaire K
 * qui renvoie
 * - un tableau de K cases contenants les tableaux de pères de l'algo de dijksra */

std::vector<chemin> recherche(Graphe_m *g, std::vector<int>* points_ville, std::vector<int>* points_loti, int k, bool verbose = false);


std::vector<chemin> recherche_memoire(Graphe_m *g, std::vector<int>* points_ville, std::vector<int>* points_loti, int k, bool verbose = false);


// Fonction qui tente de trouver k chemins différents entre la ville et le lotissement avec comme condition un flot suffisant
// Ce sont les flots de le ville qui sont modifiés
// Hyp : le lotissment, intégré à g avec add_graphe, ne consomme rien, la route non plus
std::vector<chemin> recherche_memoire_avec_flot_suffisant(Graphe* g, int conso_loti, std::vector<int>* points_ville, std::vector<int>* points_loti, int k, bool verbose = false);


// Fonction qui tente de trouver k chemins différents entre la ville et le lotissement avec comme condition un flot suffisant
// Ce sont les flots de le ville qui sont modifiés et les chemins voulu qui sont renvoyés
// Hyp : la ville, le loti et la route st dissociés
std::vector<chemin> resolve(Graphe** ville_cpy, Graphe* ville, Graphe* route, Graphe* loti, std::vector<Arete>& ville_route, std::vector<Arete>& route_loti, int k, bool verbose = false);


/**
 * Fonction qui permet de trouver l'indice dans indices_autorises qui minimise dist
 * Hyp : les deux vecteurs sont de même dimension et non vides
 * - coût linéaire en focntion de la taille de indices_autorises*/

int find_indice_min_in_vect(std::vector<float>* dist, std::vector<int>* indices_autorises);

/**
 * Génère un chemin aléatoire entre les points de la ville et du lotissement
 * 
 * */
int rd(int min, int max);

chemin creer_chemin_aleatoire(Graphe_m* g, std::vector<int>* ville, std::vector<int>* lotissement);




#endif