#ifndef RESTAURANTS_H
#define RESTAURANTS_H
#include <stdio.h>
#include "struct.h"
#include <string.h>
#include <stdlib.h>
#define MAX_RESTAURANTS 100

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define WHITE "\033[0m"

int lireRestaurants(const char *nomFichier, Restaurant restaurants[]);
int inserer_restaurant(char *nomFichier, Restaurant nouveauRestaurant);
void cherche_restaurant(position actuel, double rayon, Restaurant results[], int nombreRestaurants, Restaurant restaurants[]);
void tri_restaurant(Restaurant* result, position actuel, int a);
void cherche_par_specialite(char** specialite, int nombre_specialites, Restaurant* restaurant, Restaurant* result, position actuel, int* a);
void echanger_restaurant(Restaurant* a,Restaurant* b);
double distance(position cherche, position actuel);
void mainInsererResto(char *nomFichier);
int mainChercherRestaurant(char *nomFichier);
void mainChercherParSpecialite(char *nomFichier);
int initiation_program(char *nomFichier);
void clearTerminal();
int verif_file(const char *nomFichier);
#endif // RESTAURANTS_H
