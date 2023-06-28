#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "graphe.h"
#include "algorithmes.h"

#include "prim.hpp"


#include "quartier.h"

#include "recherche.hpp"
#include "graphe_m.hpp"

#include "ville_carree.hpp"



int main() {

	sf::RenderWindow window(sf::VideoMode(1600, 900), "Window");

	sf::Font font;
	if (font.loadFromFile("arial.ttf")) {



		std::vector<Sommet*> sommets = {
			new Sommet(0,0, -1000000),	//0
			new Sommet(0,1, 0),
			new Sommet(0,2, 6),
			new Sommet(1,1, 6),
			new Sommet(1,2, 6),
			new Sommet(2,1, 0),
			new Sommet(3,1, 0),
		};

		Graphe g = Graphe(sommets);
		g.add_arete(0,1,   2, 2);
		g.add_arete(1,0,   2, 2);
		g.add_arete(1,2,   2, 2);
		g.add_arete(2,4,   3, 3);
		g.add_arete(4,3,   4, 4);
		g.add_arete(4,1,   5, 5);
		g.add_arete(5,0,   7, 7);
		g.add_arete(5,3,   9, 9);
		g.add_arete(5,6,   9, 9);


		Graphe* acpd = g.copy();


		prim_v(&g, acpd);


		

		while (window.isOpen()) {

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) window.close();
			}

			window.clear();
			acpd->draw(window, font, false, true);
			window.display();
		
		}

		delete acpd;

	}

	return 0;

}