#include "ville_carree.hpp"

Graphe gen_graphe_grille(int n, int inter, int c, int x0, int y0) {
	Graphe res({});
	srand(time(0));

	int x = x0;
	int y = y0;

	int k = 0;

	for (int i = 0; i < n; ++i)
	{
		x = x0;
		for (int j = 0; j < n; ++j)
		{
			res.add_sommet(x, y, k);
			x += inter;
			k++;
		}
		y += inter;
	}

	// On connecte toutes les arêtes à un deux ou trois sommets

	k = n+1;

	for (int i = 1; i < n - 1; ++i)
	{
		for (int j = 1; j < n - 1; ++j)
		{
			if (rand()%2 == 0) {
				res.add_arete(k, k-n, c, c);
			}
			if (rand()%2 == 0) {
				res.add_arete(k, k-1, c, c);
			}
			if (rand()%2 == 0) {
				res.add_arete(k, k+1, c, c);
			} 
			if (rand()%2 == 0) {
				res.add_arete(k, k+n, c, c);
			}
			if (rand()%4 == 0) {
				res.add_arete(k, k+n+1, c, c);
			}
			if (rand()%4 == 0) {
				res.add_arete(k, k+n-1, c, c);
			}
			if (rand()%4 == 0) {
				res.add_arete(k, k-n+1, c, c);
			}
			if (rand()%4 == 0) {
				res.add_arete(k, k-n-1, c, c);
			} 
			k++;
		}
		k+=2;
	}

	for (int i = 0; i < n - 1; ++i)
	{
		res.add_arete(n*i, n*(i+1), c, c);

		if (rand()%3 == 0) {
			res.add_arete(n*i, n*i + 1, c, c);
		}
	}
	for (int i = 0; i < n - 1; ++i)
	{
		res.add_arete(n-1 + n*(i+1), n - 1 + n*i, c, c);

		if (rand()%3 == 0) {
			res.add_arete(n-1 + n*(i+1), n-1 + n*(i+1) - 1, c, c);
		}
	}
	for (int i = 0; i < n - 1; ++i)
	{
		res.add_arete(i + 1, i, c, c);

		if (rand()%3 == 0) {
			res.add_arete(i + 1, i+1+n, c, c);
		}
	}
	for (int i = 0; i < n - 1; ++i)
	{
		res.add_arete(n*(n-1) + i, n*(n-1) + 1 + i, c, c);

		if (rand()%3 == 0) {
			res.add_arete(n*(n-1) + i, n*(n-1) + i - 1, c, c);
		}
	}


	return res;
}

void consommations_coherentes(Graphe* g, float proba_maison, int conso, int dispertion) {
	srand(time(0));

	for (int i = 0; i < g->get_nb_sommets(); i++) {
		if (((float)rand()) / INT_MAX < proba_maison) {

			int c = std::max(1, conso + rand()%(2*dispertion) - dispertion);
			g->get_sommet(i)->c = c;

		} else g->get_sommet(i)->c = 0;
	}

}


void ajuste_flot(Graphe* g, int largeur) {

	srand(time(0));

	assert(demande_satisfiable(g, true) && "On ne peut pas trouver des flot qui fonctionnent");

	for (int i = 0; i < g->get_nb_aretes(); ++i)
	{
		g->get_arete(i)->c = g->get_arete(i)->f + rand() % largeur;
	}

}