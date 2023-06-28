#ifndef Graphe_m_HPP
#define Graphe_m_HPP

#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <list>
#include "graphe.h"

typedef std::pair<unsigned int, float> paire;
typedef std::vector<int> ensemble_points;



class Graphe_m {
public:
	Graphe_m(int nb_sommets);
	void add_edge(unsigned int u, unsigned int v, float c);
	std::list<paire> get_successeurs(int i);
	float get_cout(unsigned int u, unsigned int v);
	void affiche() const;
	int get_nb_sommets() const;
	void supprime_chemin(chemin* ch);
	bool contient_chemin(chemin* ch);


	Graphe to_graphe(std::vector<Sommet*> sommets);

private:
	std::vector<std::list<paire>> m_voisins;
};

Graphe_m to_graphe_m(Graphe* g, bool capacity_as_weight = false);


#endif