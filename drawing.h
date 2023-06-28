#ifndef DRAWING_H
#define DRAWING_H



#include <string>
#include <SFML/Graphics.hpp>



// Fonctions d'affichage.
void drawLine(sf::RenderWindow& window, sf::Color color, float x1, float y1, float x2, float y2); 
void drawArrow(sf::RenderWindow& window, sf::Color color, float x1, float y1, float x2, float y2);
void drawArete(sf::RenderWindow& window, sf::Font& font, sf::Color color, float x1, float y1, float x2, float y2, std::string str, bool oriented = true);
void drawDoubleArete(sf::RenderWindow& window, sf::Font& font, sf::Color color, float x1, float y1, float x2, float y2, std::string str1, std::string str2);
void drawSommet(sf::RenderWindow& window, sf::Font& font, sf::Color color, float c, float x, float y);
void drawHeavyLine(sf::RenderWindow& window, sf::Color color, float x1, float y1, float x2, float y2, float width);


#endif
