#include "dijkstra_memoire.hpp"

int main(int argc, char const *argv[])
{
	srand((unsigned int)time(0));

	Graphe g(6);
	g.affiche();
	g.add_edge(0, 2, 1.);
	g.add_edge(2, 0, 2.);
	g.add_edge(1, 0, 3.);
	g.add_edge(0, 1, 2.);
	g.add_edge(0, 3, 12.);
	g.add_edge(3, 4, 1.);
	g.add_edge(3, 1, 1.);
	g.add_edge(2, 4, 1.);
	g.add_edge(4, 2, 1.);
	g.add_edge(4, 5, 2.);
	g.add_edge(0, 4, 1.9);
	g.affiche();

	std::vector<int> ville = {0, 1};
	std::vector<int> loti = {4, 5};

	Dijkstra_memoire dm(&g, &ville, &loti);

	chemin ch = dm.trouve_chemin(true);

	print_list<int>(&ch);

	return 0;
}