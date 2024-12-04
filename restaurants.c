#include "restaurants.h"

int lireRestaurants(const char *nomFichier, Restaurant restaurants[]) {
    FILE *fichier = fopen(nomFichier, "r");
    if (strlen(nomFichier) < 4 || strcmp(nomFichier + strlen(nomFichier) - 4, ".txt") != 0) {
        return -1;  // Retourne -1 si le fichier n'est pas un fichier .txt
    }

    char ligne[512]; // Un tampon pour lire chaque ligne
    int i = 0;

    // Ignorer la première ligne (les en-têtes)
    fgets(ligne, sizeof(ligne), fichier);

    // Lire chaque ligne du fichier
    while (i < MAX_RESTAURANTS && fgets(ligne, sizeof(ligne), fichier) != NULL) {
        // Utiliser sscanf pour extraire les données de la ligne
        if (sscanf(ligne, "%99[^;]; %199[^;]; (x=%lf, y=%lf);  %49[^;]",
                   restaurants[i].nom,
                   restaurants[i].adresse,
                   &restaurants[i].Coordonnees.x,
                   &restaurants[i].Coordonnees.y,
                   restaurants[i].specialite) == 5) {
            i++;
        }

    }
    if (i < MAX_RESTAURANTS) {
    strcpy(restaurants[i].nom, "rien");// Marquer la fin avec une chaîne vide ou utiliser NULL si c'est un pointeur
}

    fclose(fichier);
    return i; // Retourner le nombre de restaurants lus
}

int inserer_restaurant(char *nomFichier, Restaurant nouveauRestaurant) {
    FILE* fichier = fopen(nomFichier, "a");
    if (fichier) {
        fprintf(fichier, "%s; %s; (x=%f, y=%f); {%s}\n",
            nouveauRestaurant.nom,
            nouveauRestaurant.adresse,
            nouveauRestaurant.Coordonnees.x,
            nouveauRestaurant.Coordonnees.y,
            nouveauRestaurant.specialite);
        fclose(fichier);
        return 1;
    }
    return 0;
}

void cherche_restaurant(position actuel, double rayon, Restaurant results[], int nombreRestaurants, Restaurant restaurants[]) {
    int a = 0;
    int found = 0;  // Variable pour vérifier si au moins un restaurant est trouvé
    
    for (int i = 0; i < nombreRestaurants; i++) {
        // Si le restaurant est dans le rayon
        if (distance(restaurants[i].Coordonnees, actuel) < rayon) {
            if (a == 0) {
                printf("Voici les restaurants trouvés dans le rayon de recherche : \n\n");
            }
            printf("Restaurant" GREEN ": %s\n" WHITE, restaurants[i].nom);
            strcpy(results[a].nom, restaurants[i].nom);
            strcpy(results[a].adresse, restaurants[i].adresse);
            results[a].Coordonnees.x = restaurants[i].Coordonnees.x;
            results[a].Coordonnees.y = restaurants[i].Coordonnees.y;
            strcpy(results[a].specialite, restaurants[i].specialite);
            a++;
            found = 1;  // Un restaurant a été trouvé, donc on met `found` à 1
        }
    }

    if (found == 0) {
        printf(RED "Il semble qu'il n'y a pas de restaurants par ici..\n" WHITE);
    }
}


void tri_restaurant(Restaurant* result, position actuel, int a) { //Trier les restaurants par les coordonées rentrées.
    for (int i = 0; i < a - 1; i++) {
        for (int j = i + 1; j < a; j++) {
            if (distance(result[i].Coordonnees, actuel) > distance(result[j].Coordonnees, actuel)) {
                echanger_restaurant(&result[i], &result[j]); //Switch deux restaurants
            }
        }
    }
}

void cherche_par_specialite(char** specialite, int nombre_specialites, Restaurant* restaurant, Restaurant* result, position actuel, int* a) {
    for (int i = 0; i < MAX_RESTAURANTS; i++) {
        for (int j = 0; j < nombre_specialites; j++) {
            if (strstr(restaurant[i].specialite, specialite[j]) != NULL) {
                result[*a] = restaurant[i];
                (*a)++;
                break;
            }
        }
    }
}

void mainInsererResto(char *nomFichier) { //Récupérer les données du restaurant
    Restaurant nouveauRestaurant;
    
    printf("Veuillez entrer le nom du restaurant : ");
    scanf(" %[^\n]", nouveauRestaurant.nom);
    printf("Veuillez entrer l'adresse du restaurant : ");
    scanf(" %[^\n]", nouveauRestaurant.adresse);
    printf("Veuillez entrer les coordonnées du restaurant (x puis y) : ");
    while (1) {
        if (scanf("%lf %lf", &nouveauRestaurant.Coordonnees.x,
    &nouveauRestaurant.Coordonnees.y) == 2 && nouveauRestaurant.Coordonnees.x >= 0 && nouveauRestaurant.Coordonnees.y >= 0) {
            break;  // Si les entrées sont valides, sortir de la boucle
        } else {

            while (getchar() != '\n'); 
            clearTerminal();
            printf(RED "Erreur : Veuillez entrer deux nombres valides positifs pour x, y. \n" WHITE);
            printf("Donnez votre position x y\n");
        }
    }
    printf("Veuillez entrer la spécialité du restaurant : ");
    scanf(" %[^\n]", nouveauRestaurant.specialite);

    int fichier_existant = initiation_program(nomFichier);
    if (fichier_existant == 0) {
        int insere_fichier = inserer_restaurant(nomFichier, nouveauRestaurant);
        if(insere_fichier) {
    		printf(GREEN"Il n'y a pas de fichier source, le restaurant a donc été ajouté dans le fichier 'restau.txt' \n"WHITE);
    	}
    	else {
    	    	perror(RED"Echec de l'ajout du restaurant. \n"WHITE); }
    	}
    else { 
         int insere_fichier = inserer_restaurant(nomFichier, nouveauRestaurant);
    	 if(insere_fichier) {
    	         printf(GREEN"Le restaurant a été ajouté avec succès. \n"WHITE); }
    	 
    	 else{
    	     	 perror(RED"Echec de l'ajout du restaurant. \n"WHITE); }
    	 }
}


int mainChercherRestaurant(char *nomFichier) { //Récupérer les filtres pour la recherche de restaurant.
    int verif = verif_file(nomFichier);
    if (!verif) {
    	printf(RED"Impossible : Aucun fichier source séléctionné"WHITE);
    	return 0;
    }
    Restaurant results[MAX_RESTAURANTS];
    Restaurant restaurants[MAX_RESTAURANTS]; //
    
    int nombreRestaurants = lireRestaurants(nomFichier, restaurants); //Obtenir le nombre de restaurants.
    position actuel;
    double rayon;
    
    printf("Donnez votre position x, y puis le rayon de recherche\n");
    
    while (1) {
        if (scanf("%lf %lf %lf", &actuel.x, &actuel.y, &rayon) == 3 && actuel.x >= 0 && actuel.y >= 0 && rayon >= 0) {
            break;  // Si les entrées sont valides, sortir de la boucle
        } else {

            while (getchar() != '\n'); 
            clearTerminal();
            printf(RED "Erreur : Veuillez entrer trois nombres valides et positifs pour x, y, et rayon.\n" WHITE);
            printf("Donnez votre position x, y puis le rayon de recherche\n");
        }
    }

    cherche_restaurant(actuel,rayon,results,nombreRestaurants,restaurants);
}

void mainChercherParSpecialite(char *nomFichier) {
    int verif = verif_file(nomFichier);
    if (!verif) {
    	printf(RED"Impossible : Aucun fichier source séléctionné"WHITE);
    	return;
    }
    int a = 0;
    Restaurant* result = malloc(20 * sizeof(Restaurant));
    Restaurant restaurant[200];
    position actuel;
    int nombre_specialites;

    printf("Veuillez entrer votre position (coordonnées x et y, séparées par un espace) :\n");
    
    while (1) {
        if (scanf("%lf %lf", &actuel.x, &actuel.y) == 2 ) {
            break;  // Si les entrées sont valides, sortir de la boucle
        } else {

            while (getchar() != '\n'); 
            clearTerminal();
            printf(RED "Erreur : Veuillez entrer deux nombres valides positifs pour x, y\n" WHITE);
            printf("Donnez votre position x y\n");
        }
    }
    printf("Combien de spécialités voulez-vous entrer ? ");
    while (1) {
	    if (scanf("%d", &nombre_specialites) == 1 && nombre_specialites > 0) {
		break; 
	    } else {
		while (getchar() != '\n');
		clearTerminal();
		printf(RED "Erreur : Veuillez entrer un nombre entier positif.\n" WHITE);
		printf("Combien de spécialités voulez-vous entrer ? ");
    }
}

    char **specialite = malloc(nombre_specialites * sizeof(char *));
    for (int i = 0; i < nombre_specialites; i++) {
        specialite[i] = malloc(50 * sizeof(char));
        printf("Entrez la spécialité %d : ", i + 1);
        scanf(" %[^\n]", specialite[i]);
    }

    int nombreResto = lireRestaurants(nomFichier, restaurant);
    cherche_par_specialite(specialite, nombre_specialites, restaurant, result, actuel, &a);
    tri_restaurant(result, actuel, a);


    if (a > 0) { //On affiche le nom des restaurants
    printf(GREEN "%d" WHITE " restaurants ont été trouvés sur un total de %d \n\n", a, nombreResto);
        for (int i = 0; i < a; i++) {
            printf("Restaurant %d: " GREEN"%s\n"WHITE, i + 1, result[i].nom);
        }
    } else {
        printf(RED "Il semble qu'il n'y a pas cette spécialité ici..\n" WHITE);
    }

    for (int i = 0; i < nombre_specialites; i++) {
        free(specialite[i]);
    }
    free(specialite);
    free(result);
}
