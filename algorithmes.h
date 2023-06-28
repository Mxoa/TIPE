#ifndef ALGORITHMES_H
#define ALGORITHMES_H



#include <SFML/Graphics.hpp>
#include <vector>
#include <list>

#include "graphe.h"



// Algorithme de Ford-Fulkerson.
Chemin chemin_ameliorant(Graphe* g, int s, int p);
void ford_fulkerson(Graphe* g, int s, int p, sf::RenderWindow* window = nullptr, sf::Font* font = nullptr);

// Foncitions de vérification de l'intégrité du réseau.
bool demande_satisfiable(Graphe* g, bool edit = false, sf::RenderWindow* window = nullptr, sf::Font* font = nullptr);
bool ajout_possible(Graphe* g, Sommet s, std::vector<Arete>& aretes, bool edit = false, sf::RenderWindow* window = nullptr, sf::Font* font = nullptr);
bool ajout_chemin_possible(Graphe* g, Graphe* routes, std::vector<chemin>* chemins, bool edit = false, sf::RenderWindow* window = nullptr, sf::Font* font = nullptr);

// Fonctions de calcul des sommets directement accessibles depuis un point.
bool intersection(Sommet* a1, Sommet* a2, Sommet* b1, Sommet* b2);
std::vector<int> sommets_accessibles(Graphe* g, Sommet s);



#endif
