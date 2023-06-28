#ifndef TD
#define TD

#include "outils.hpp"

class Tas
{
	/**
	 * Implémente un tas min
	 * */
public:
	Tas(int n); // Ordre de grandeur de la taille du tas
	void add_elem(int elem, float prio);
	int get_min() const;
	int extract_min();
	void remove_min();
	void show();
private:
	bool est_plein() const;
	int pere(int noeud);
	int fils_droit(int noeud);
	int fils_gauche(int noeud);
	void echanger(int i, int j);


	std::vector<float> m_tas;
	int m_nb_elem;
	
};

#endif