# Makefile

GCC = g++
EXEC = test

# Pour la SFML
FLAGS = -W -Wall -Wextra
LIBS = -Llib -lsfml-graphics -lsfml-window -lsfml-system

tas.o : tas.cpp tas.hpp outils.hpp
	$(GCC) -c $< -Wall

graphe_m.o : graphe_m.cpp graphe_m.hpp graphe.cpp
	$(GCC) -c $< -Wall

recherche.o : recherche.cpp recherche.hpp
	$(GCC) -c $< -Wall

dijkstra.o : dijkstra.cpp dijkstra.hpp outils.hpp
	$(GCC) -c $< -Wall

dijkstra_memoire.o : dijkstra_memoire.cpp dijkstra.hpp outils.hpp
	$(GCC) -c $< -Wall

file_de_priorite.o : file_de_priorite.cpp file_de_priorite.hpp outils.hpp
	$(GCC) -c $< -Wall

grahe.o : graphe.cpp graphe.h
	$(GCC) -c $<

drawing.o : drawing.cpp drawing.h
	$(GCC) -c $<

prim.o : prim.cpp prim.hpp
	$(GCC) -c $<

union_find.o : union_find.cpp union_find.h
	$(GCC) -c $<

quartier.o :  quartier.cpp quartier.h
	$(GCC) -c $<

algorithmes.o : algorithmes.cpp algorithmes.h
	$(GCC) -c $<

ville_carree.o : ville_carree.cpp ville_carree.hpp
	$(GCC) -c $<

# Exécutables

test : main.cpp graphe_m.o dijkstra.o file_de_priorite.o recherche.o
	$(GCC) $^ -o $(EXEC) -Wall

test_dm : dijkstra_memoire_test.cpp dijkstra_memoire.o file_de_priorite.o graphe_m.o
	$(GCC) $^ -o $(EXEC) -Wall

test_tas : test_tas.cpp tas.o

test_quartier : main_quartier.cpp graphe.o drawing.o quartier.o algorithmes.o prim.o union_find.o
	$(GCC) -lm $^ -o $@ $(LIBS)

all : main.cpp graphe_m.o dijkstra.o
	$(GCC) $^ -o $(EXEC) -Wall

test_louis : main_louis.cpp graphe_m.o graphe.o drawing.o dijkstra.o algorithmes.o file_de_priorite.o tas.o dijkstra_memoire.o recherche.o ville_carree.o prim.o union_find.o quartier.o
	$(GCC) -lm $^ -o $@ $(LIBS)

test_prim : main_louis.cpp graphe.o drawing.o prim.o union_find.o
	$(GCC) -lm $^ -o $@ $(LIBS)

test_prim_em : test_prim.cpp graphe.o drawing.o prim.o union_find.o file_de_priorite.o
	$(GCC) -lm $^ -o $@ $(LIBS)

test_convertisseur : main_marceau.cpp graphe_m.o graphe.o drawing.o dijkstra.o algorithmes.o file_de_priorite.o tas.o dijkstra_memoire.o recherche.o
	$(GCC) -lm $^ -o $@ $(LIBS)

test_ville_loti_relies : test_ville_loti_relies.cpp graphe_m.o graphe.o drawing.o dijkstra.o algorithmes.o file_de_priorite.o tas.o dijkstra_memoire.o recherche.o ville_carree.o prim.o union_find.o
	$(GCC) -lm $^ -o $@ $(LIBS)

test_realiste : main_realiste.cpp graphe.o quartier.o algorithmes.o drawing.o
	$(GCC) -lm $^ -o $@ $(LIBS)




# Autre

clean :
	rm *.o