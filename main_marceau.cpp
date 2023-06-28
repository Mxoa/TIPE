#include "graphe_m.hpp"
#include "drawing.h"
#include "recherche.hpp"
#include "outils.hpp"

int main() {

	// Création d'un graphe 

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Window");

	sf::Font font;
	if (font.loadFromFile("arial.ttf")) {
    
		std::vector<Sommet*> sommets = {
			new Sommet(0,2,-8),
			new Sommet(2,0,0),
			new Sommet(2,4,0),
			new Sommet(4,2,6),
			new Sommet(4,4,2),
			new Sommet(4,5,2)
		};
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
		g.add_edge(1, 3, 3.);
		g.affiche();

		std::vector<int> ville = {0, 2, 1};
		std::vector<int> lotissement = {4, 3};

		std::vector<chemin> w = recherche_memoire(&g, &ville, &lotissement, 2);

		print_list<int>(&w[0]);

		Graphe g_d = g.to_graphe(sommets);


		while (window.isOpen()) {

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) window.close();
			}

			window.clear();
			g_d.drawPath(window, &(w[0]), 20);
			g_d.drawPath(window, &(w[1]), 20);
			g_d.draw(window, font, false, false);	

			window.display();
		
		}

	}

	return 0;

}