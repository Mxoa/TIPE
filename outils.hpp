#ifndef OUTILS_HPP
#define OUTILS_HPP

#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>	
#include <cassert>

#define INFINITY 1000000

typedef std::pair<int, float> doublet;

template<class T>
void print_vector(std::vector<T>* v) {
	// Affiche le vecteur v
	std::cout << "[|";
	for (unsigned int i = 0; i < v->size(); ++i)
	{
		std::cout << (*v)[i] << "; ";
	}
	std::cout << "|]" << std::endl;
}

template<class T>
void list_rev(std::list<T>* v) {
	std::list<T> res;

	for (auto it = std::begin(*v); it != std::end(*v); ++it)
	{
		res.push_front(*it);

	}
	
	*v = res;

}

template<class T>
void print_list(std::list<T>* v) {
	// Affiche le vecteur v
	std::cout << "[";
	for (auto it = std::begin(*v); it != std::end(*v); ++it)
	{
		std::cout << *it << "; ";
	}
	std::cout << "]" << std::endl;
}

#endif