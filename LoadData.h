#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// En-têtes spécifiques à Windows
#ifdef _WIN32
#include <windows.h>
#include <fileapi.h>
#else
#include <dirent.h>
#endif

#include "restaurants.h" 
#include "struct.h"

#define GREEN   "\033[32m"
#define WHITE   "\033[0m"
#define RED     "\033[31m"
#define MAX_RESTAURANTS 100

// Fonction qui permet d'afficher tous les fichiers du répertoire courant.
int afficherRep() {
#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("*", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Erreur d'ouverture du répertoire.\n");
        return 0;
    } else {
        printf("Voici tous les fichiers du repertoire actuel, si vous souhaitez changer de fichier source entrez '2' \n\n");
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                printf(WHITE"Fichier: ");
                printf(GREEN"%s\n"WHITE"", findFileData.cFileName);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
#else
    DIR *d;
    struct dirent *dir;
    
    d = opendir(".");
    if (d) {
        printf("Voici tous les fichiers du répertoire actuel, si vous souhaitez changer de fichier source entrez '2' \n\n");
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                printf(WHITE"Fichier: ");
                printf(GREEN"%s\n"WHITE"", dir->d_name);
            }
        }
        closedir(d);
    }
#endif
}

// Fonction qui calcule la distance en fonction d'une position
double distance(position cherche, position actuel) {
    return sqrt(pow(cherche.x - actuel.x, 2) + pow(cherche.y - actuel.y, 2));
}

void echanger_restaurant(Restaurant* a,Restaurant* b) {
    Restaurant c;
    c = *a;
    *a = *b;
    *b = c;
}

// Fonction qui permet de "nettoyer" le texte affiché
void clearTerminal() {
#ifdef _WIN32
    system("cls"); // Commande pour Windows
#else
    system("clear"); // Commande pour Linux/Mac 
#endif
} 

void consigne() {
    printf("\n\nVeuillez entrer une commande, '0' pour afficher le menu : ");
}

// Fonction qui vérifie la présence du fichier source dans le répertoire au lancement du programme.
int initiation_program(char *nomFichier) {
#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(nomFichier, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf(RED"\nErreur lors du chargement du répertoire\n\n"WHITE);
        return 0;
    }
    FindClose(hFind);
#else
    DIR *d;
    struct dirent *dir;
    
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG && strcmp(dir->d_name, nomFichier) == 0) {
                closedir(d);
                return 1;
            }
        }
        closedir(d);
        return 0; // Le fichier n'a pas été trouvé
    } else {
        printf(RED"\nErreur lors du chargement du répertoire\n\n"WHITE);
        return 0; // En cas d'erreur dans le répertoire
    }
#endif
}

void QuitterProgramme() {
    printf(GREEN "Merci d'avoir lancé notre programme ! (Badyss et Yanis) \n"WHITE);
    exit(0);
}

// Fonction qui permet de charger le fichier source depuis le répertoire courant.
void changerFichierSource(char *nomFichier) {
    printf("Veuillez entrer le nom du nouveau fichier source : ");
    scanf(" %[^\n]", nomFichier); // Modifie directement le contenu

    // Allocation dynamique de restaurants
    Restaurant *restaurants = malloc(MAX_RESTAURANTS * sizeof(Restaurant));
    if (!restaurants) {
        printf(RED "Erreur : Impossible d'allouer de la mémoire pour les restaurants.\n" WHITE);
        return;
    }

    int nombreRestaurants = lireRestaurants(nomFichier, restaurants);
    if (nombreRestaurants == -1) {
        printf(RED "Erreur : Impossible d'ouvrir le fichier %s.\n" WHITE, nomFichier);
    } else {
        printf(GREEN "%d restaurants chargés depuis le fichier %s.\n" WHITE, nombreRestaurants, nomFichier);
    }

    free(restaurants);
}

// Fonction qui vérifie la présence du fichier source au lancement des fonctions du programme.
int verif_file(const char *nomFichier) {
#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(nomFichier, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0; // Fichier non trouvé
    }
    FindClose(hFind);
    return 1; // Fichier trouvé
#else
    DIR *d;
    struct dirent *dir;

    d = opendir(".");
    if (d == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 0; // Erreur d'ouverture du répertoire
    }

    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_REG && strcmp(dir->d_name, nomFichier) == 0) {
            closedir(d);
            return 1; // Fichier trouvé
        }
    }

    closedir(d);
    return 0;
#endif
}
