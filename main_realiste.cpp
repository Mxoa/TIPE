#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "graphe.h"
#include "algorithmes.h"


#include "quartier.hpp"


int main() {

	sf::RenderWindow window(sf::VideoMode(800, 800), "Window");

	sf::Font font;
	if (font.loadFromFile("arial.ttf")) {
  /*  
		std::vector<Sommet*> sommets = {
			new Sommet(0,2,-8),
			new Sommet(2,0,0),
			new Sommet(2,4,0),
			new Sommet(4,2,6)
		};
		Graphe g = Graphe(sommets);
		g.add_arete(0,1,5);
		g.add_arete(0,2,3);
		g.add_arete(1,2,3);
		g.add_arete(1,3,2);
		g.add_arete(2,3,6);

		Sommet s = Sommet(4,4,2);
		std::vector<Arete> aretes = {
			{2,-1,1,0},
			{3,-1,1,0}
		};



		if (ajout_possible(&g, s, aretes, true, &window, &font)) std::cout << "Ajout possible" << std::endl;
		if (demande_satisfiable(&g, true, &window, &font)) std::cout << "Satisfait" << std::endl;

*/



		std::vector<Sommet*> sommets = {
			new Sommet(0,0, -1000000),	//0
			new Sommet(2,3, 0),
			new Sommet(2,0, 6),
			new Sommet(3,2, 3),
			new Sommet(3,6, 0),
			new Sommet(2,7, 2),			//5
			new Sommet(6,4, 0),
			new Sommet(8,3, 0),
			new Sommet(10,2, 0),
			new Sommet(4,3, 5),
			new Sommet(8,1, 5),			//10
			new Sommet(9,-1, 0),
			new Sommet(8,-3, 0),
			new Sommet(10,-3, 3),
			new Sommet(6,-3, 3),
			new Sommet(7,0, 0),			//15
			new Sommet(5,1, 0),
			new Sommet(6,-1, 6),
			new Sommet(4,0, 3),
			new Sommet(11,-2, 0),
			new Sommet(11,0, 4),		//20
			new Sommet(13,-1, 0),
			new Sommet(15,-3, 7),
			new Sommet(16,0, 0),
			new Sommet(17,-1, 8),
			new Sommet(13,1, 0),		//25
			new Sommet(14,2, 10),
			new Sommet(11,4, 0),
			new Sommet(12,6, 0),
			new Sommet(10,7, 7),
			new Sommet(6,6, 4),			//30
			new Sommet(8,5, 5),
			new Sommet(4,9, 0),
			new Sommet(6,8, 0),
			new Sommet(7,9, 5),
			new Sommet(5,12, 0),		//35
			new Sommet(8,11, 0),		
			new Sommet(10,10, 0),		
			new Sommet(13,9, 0),
			new Sommet(15,5, 0),
			new Sommet(18,4, 0),		//40
			new Sommet(16,10, 0),
			new Sommet(20,6, 0),
			new Sommet(15,7, 8),
			new Sommet(17,3, 4),
			new Sommet(17,7, 3),		//45
			new Sommet(20,3, 3),
			new Sommet(15,12, 3),
			new Sommet(19,11, 4),
			new Sommet(3,13, 0),
			new Sommet(4,16, 0),			//50
			new Sommet(4,19, 0),
			new Sommet(6,19, 0),
			new Sommet(7,16, 0),
			new Sommet(2,16, 15),			
			new Sommet(3,20, 12),			//55			
			new Sommet(5,22, 10),			
			new Sommet(5,16, 20),
			new Sommet(8,19, 9),
			new Sommet(9,15, 7),
			new Sommet(10,12, 8)				//60
		};

		Graphe g = Graphe(sommets);
		g.add_arete(0,1,   300);
		g.add_arete(0,2,   10);
		g.add_arete(1,4,   300);
		g.add_arete(1,3,   10);
		g.add_arete(4,5,   10);
		g.add_arete(4,6,   60);
		g.add_arete(6,4,   60);
		g.add_arete(6,7,   60);
		g.add_arete(7,6,   60);
		g.add_arete(7,8,   60);
		g.add_arete(8,7,   60);
		g.add_arete(6,9,   10);
		g.add_arete(7,10,  10);
		g.add_arete(8,11,  60);
		g.add_arete(11,12, 60);
		g.add_arete(12,13, 10);
		g.add_arete(12,14, 10);
		g.add_arete(11,15, 60);
		g.add_arete(15,16, 60);
		g.add_arete(15,17, 10);
		g.add_arete(16,18, 10);
		g.add_arete(11,19, 60);
		g.add_arete(19,20, 10);
		g.add_arete(19,21, 60);
		g.add_arete(21,22, 10);
		g.add_arete(21,23, 60);
		g.add_arete(23,24, 10);
		g.add_arete(21,25, 20);
		g.add_arete(25,26, 20);
		g.add_arete(8,27,  60);
		g.add_arete(27,8,  60);
		g.add_arete(27,28, 60);
		g.add_arete(28,27, 60);
		g.add_arete(27,29, 10);
		g.add_arete(6,30,  10);
		g.add_arete(7,31,  10);
		g.add_arete(4,32,  240);
		g.add_arete(32,4,  240);
		g.add_arete(32,33, 10);
		g.add_arete(33,34, 10);
		g.add_arete(32,35, 240);
		g.add_arete(35,32, 240);
		g.add_arete(35,36, 100);
		g.add_arete(36,35, 100);
		g.add_arete(36,37, 100);
		g.add_arete(37,36, 100);
		g.add_arete(37,38, 100);
		g.add_arete(38,37, 100);
		g.add_arete(38,28, 60);
		g.add_arete(28,38, 60);
		g.add_arete(28,39, 60);
		g.add_arete(39,28, 60);
		g.add_arete(39,40, 60);
		g.add_arete(40,39, 60);
		g.add_arete(38,41, 60);
		g.add_arete(41,38, 60);
		g.add_arete(41,42, 60);
		g.add_arete(42,41, 60);
		g.add_arete(40,42, 60);
		g.add_arete(42,40, 60);
		g.add_arete(39,43, 10);
		g.add_arete(40,44, 10);
		g.add_arete(42,45, 10);
		g.add_arete(42,46, 10);
		g.add_arete(38,47, 10);
		g.add_arete(41,48, 10);
		g.add_arete(35,49, 100);
		g.add_arete(49,35, 100);
		g.add_arete(49,50, 100);
		g.add_arete(50,49, 100);
		g.add_arete(50,51, 100);
		g.add_arete(51,50, 100);
		g.add_arete(51,52, 100);
		g.add_arete(52,51, 100);
		g.add_arete(52,53, 100);
		g.add_arete(53,52, 100);
		g.add_arete(53,35, 100);
		g.add_arete(35,53, 100);
		g.add_arete(50,54, 20);
		g.add_arete(51,55, 20);
		g.add_arete(51,56, 20);
		g.add_arete(52,57, 20);
		g.add_arete(53,58, 20);
		g.add_arete(53,59, 20);
		g.add_arete(37,60, 20);



//		if (demande_satisfiable(&g, true, &window, &font)) std::cout << "Satisfait" << std::endl;



		Graphe *g2 = generer_quartier(3,6,8,30,25,5,3);
		Graphe *g3 = generer_routes(&g, g2, 3, 7);

		while (window.isOpen()) {

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) window.close();
			}

			window.clear();
			g3->draw(window, font, false);
			window.display();
		
		}

		delete g3;
		delete g2;

	}

	return 0;
}