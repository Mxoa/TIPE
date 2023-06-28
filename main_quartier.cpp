#include "quartier.h"

#include "prim.hpp"

int main() {

	// Création d'un graphe 

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Window");

	sf::Font font;
	if (font.loadFromFile("arial.ttf")) {
    
		Graphe* q = generer_quartier(10, 10, 20, 0, 0, 10, 5);
		//Graphe* v = generer_quartier(1, 1, 2, 20, 20, 10, 5);
		//Graphe* r = generer_routes(q, v, 1, 1);

		std::cout << "Consommation du lotissement = " << q->get_consommation() << std::endl;

		while (window.isOpen()) {

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) window.close();
			}

			window.clear();
			q->draw(window, font, false, false);
			window.display();
		
		}

		delete q;
		//delete v;
		//delete r;

	}

	return 0;

}