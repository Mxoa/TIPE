#include "recherche.hpp"

int main(int argc, char const *argv[])
{
	srand((unsigned int)time(0));

	Graphe_m g(6);
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
	g.add_edge(0, 4, 3.);
	g.affiche();

	assert(g.get_cout(0, 2) == 1.);
	assert(g.get_cout(2, 0) == 2.);

	for (int i = 0; i < 30; ++i)
	{
		std::cout << rd(0, 3) << std::endl;
	}

	doublet_dist_chem v = dijkstra_d_p(&g, 4);
	print_vector(&(v.first));
	print_vector(&(v.second));

	std::vector<int> p = dijkstra(&g, 4);
	print_vector(&(p));

	std::vector<int> ville = {0, 1};
	std::vector<int> lotissement = {4, 3};

	std::vector<chemin> w = recherche(&g, &ville, &lotissement, 2);

	print_list(&w[0]);
	print_list(&w[1]);

	std::cout << "_____________" << std::endl;

	// Test chemin aléatoire
	chemin ch = creer_chemin_aleatoire(&g, &ville, &lotissement);
	
	chemin ch2 = creer_chemin_aleatoire(&g, &ville, &lotissement);
	print_list(&ch);
	print_list(&ch2);


	

	assert(g.contient_chemin(&ch));
	assert(g.contient_chemin(&ch2));

	std::cout << "fin" << std::endl;
	return 0;
}