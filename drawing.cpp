#include "drawing.h"
#include <cmath>
#include <iostream>



// Constantes pour l'affichage du graphe.
#define ARROW_HEAD 15.0f
#define SOMMET_SIZE 20.0f
#define FONT_SIZE 25.0f



// Dessine une ligne de couleur color entre (x1,y1) et (x2,y2).
void drawLine(sf::RenderWindow& window, sf::Color color, float x1,
	float y1, float x2, float y2) {

	sf::VertexArray line(sf::LinesStrip, 2);
	line[0].position = sf::Vector2f(x1, y1);
	line[1].position = sf::Vector2f(x2, y2);
	line[0].color = color;
	line[1].color = color;
	window.draw(line);

}

// Dessine une flèche de couleur color de (x1,y1) à (x2,y2).
void drawArrow(sf::RenderWindow& window, sf::Color color, float x1,
	float y1, float x2, float y2) {

	// Angle pour la rotation de la tête.
	float theta = std::atan((y2-y1)/(x2-x1));
	if (x2<x1) theta += M_PI;
	else if (y2<y1) theta += 2.0*M_PI;
	theta *= 180.0/M_PI;

	// Tête.
	sf::ConvexShape head;
	head.setPointCount(3);
	head.setPoint(0,{0.f,0.f});
	head.setPoint(1,{ARROW_HEAD,ARROW_HEAD/2.f});
	head.setPoint(2,{0.f,ARROW_HEAD});
	head.setOrigin(ARROW_HEAD,ARROW_HEAD/2.f);

	// Dessine la tête et la ligne de la flèche.
	head.setPosition(sf::Vector2f(x2,y2));
	head.setRotation(theta);
	drawLine(window, color, x1, y1, x2, y2);
	window.draw(head);

}

// Dessine une arète de couleur color de (x1,y1) à (x2,y2) titrée par str.
void drawArete(sf::RenderWindow& window, sf::Font& font, sf::Color color, 
	float x1, float y1, float x2, float y2, std::string str, bool oriented) {

	// Calcule le décalage de position du début et de la fin de l'arète
	// en fonction de la taille d'affichage des noeuds.
	float d[2] = {x2-x1,y2-y1};
	float length = sqrt(d[0]*d[0]+d[1]*d[1]);
	d[0] *= SOMMET_SIZE/length;
	d[1] *= SOMMET_SIZE/length;

	// Dessine l'arète.
	x1 += d[0];
	y1 += d[1];
	x2 -= d[0];
	y2 -= d[1];
	if (oriented) drawArrow(window, color, x1, y1, x2, y2);
	else drawLine(window, color, x1, y1, x2, y2);

	// Dessine le titre.
	if (str != "") {

		sf::Text text;
		text.setFont(font);
		text.setString(str);
		text.setCharacterSize(FONT_SIZE); 
		text.setPosition((x1+x2)/2.0f, (y1+y2)/2.0f);
		text.setOrigin(text.getGlobalBounds().width/2.0f, text.getGlobalBounds().height/2.0f);
		text.setOutlineThickness(2.0f);
		
		window.draw(text);
	
	}

}


// Dessine une arète double de couleur color entre (x1,y1) et (x2,y2) titrée par 
// str1 de (x1,y1) à (x2,y2) et str2 de (x2,y2) à (x1,y1).
void drawDoubleArete(sf::RenderWindow& window, sf::Font& font, sf::Color color, 
	float x1, float y1, float x2, float y2, std::string str1, std::string str2) {

	float d[2] = {y2-y1, -x2+x1};
	float length = sqrt(d[0]*d[0]+d[1]*d[1]);
	d[0] *= SOMMET_SIZE/(2.0*length);
	d[1] *= SOMMET_SIZE/(2.0*length);

	drawArete(window, font, color, x1+d[0], y1+d[1], x2+d[0], y2+d[1], "");
	drawArete(window, font, color, x2-d[0], y2-d[1], x1-d[0], y1-d[1], "");

	d[0] *= 2.0/SOMMET_SIZE;
	d[1] *= 2.0/SOMMET_SIZE;
	if (str1 != "") {

		sf::Text text;
		text.setFont(font);
		text.setString(str1);
		text.setCharacterSize(FONT_SIZE); 
		length = sqrt(
			text.getGlobalBounds().width * text.getGlobalBounds().width
			+ text.getGlobalBounds().height * text.getGlobalBounds().height
		);
		text.setPosition((x1+x2+d[0]*length)/2.0f, (y1+y2+d[1]*length)/2.0f);
		text.setOrigin(text.getGlobalBounds().width/2.0f, text.getGlobalBounds().height/2.0f);
		text.setOutlineThickness(2.0f);
		
		window.draw(text);
	
	}

	if (str2 != "") {

		sf::Text text;
		text.setFont(font);
		text.setString(str2);
		text.setCharacterSize(FONT_SIZE); 
		text.setPosition((x1+x2-d[0]*length)/2.0f, (y1+y2-d[1]*length)/2.0f);
		text.setOrigin(text.getGlobalBounds().width/2.0f, text.getGlobalBounds().height/2.0f);
		text.setOutlineThickness(2.0f);
	
		window.draw(text);
	
	}

}

void drawSommet(sf::RenderWindow& window, sf::Font& font, sf::Color color,
	float c, float x, float y) {

	sf::CircleShape circle;
	circle.setRadius(SOMMET_SIZE);
	circle.setFillColor(color);

	circle.setPosition(x-SOMMET_SIZE, y-SOMMET_SIZE);
	window.draw(circle);

	if (c > 0) {
		sf::Text text;
		text.setFont(font);
		text.setString(std::to_string(static_cast<int>(c)));
		text.setCharacterSize(30);
		text.setPosition(x, y);
		text.setOrigin(text.getGlobalBounds().width/2.0f, text.getGlobalBounds().height/2.0f);
		text.setOutlineThickness(2.0f);
		window.draw(text);
	}

}


void drawHeavyLine(sf::RenderWindow& window, sf::Color color, float x1, float y1, float x2, float y2, float width) {
	// 


	float longueur = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

	sf::RectangleShape line(sf::Vector2f(longueur, width));

	line.setPosition(sf::Vector2f(x1, y1 - width / 2));

	

	// Angle pour la rotation de la tête.
	float theta = std::atan((y2-y1)/(x2-x1));
	if (x2<x1) theta += M_PI;
	else if (y2<y1) theta += 2.0*M_PI;
	theta *= 180.0/M_PI;

	line.setRotation(theta);

	line.setFillColor(color);
	window.draw(line);
}