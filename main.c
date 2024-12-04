#include "LoadData.h" //Toutes les fonctions intermédiaires y sont.
#include "restaurants.h" //Toutes les fonctions directes répondant au TP.

int afficher_menu() {
        printf("Voici la liste des commandes : \n\n");
        printf("1.Afficher le répertoire actuel \n");
        printf("2.Séléctionner un nouveau fichier Restaurant source \n");
        printf("3.Insérer un restaurant\n");
        printf("4.Chercher un restaurant\n");
        printf("5.Chercher un restaurant par spécialité\n");
        printf("6.Quitter le programme\n");
}

int utiliser_menu(char *nomFichier) {
    clearTerminal();
    if (initiation_program(nomFichier) == 0) {
        printf(RED"\nIl semble qu'aucun fichier n'est chargé :( \n\n"WHITE);
    }

    printf("Veuillez entrer une commande, appuyer sur '0' pour afficher le menu : ");
    char valeur_saisie;
    while (1) {
        valeur_saisie = getchar();
        if (valeur_saisie == '\n') {
            continue;
        }
        switch (valeur_saisie) {
            case '1':
                clearTerminal();
                afficherRep();
                consigne();
                break;
            case '2':
                clearTerminal();
                changerFichierSource(nomFichier);
                consigne();
                break;
            case '3':
                clearTerminal();
                mainInsererResto(nomFichier);
                consigne();
                break;
            case '4':
                clearTerminal();
                mainChercherRestaurant(nomFichier);
                consigne();
                break;
            case '5':
                clearTerminal();
                mainChercherParSpecialite(nomFichier);
                consigne();
                break;
            case '6':
                clearTerminal();
                QuitterProgramme();
                consigne();
                break;
            default:
                clearTerminal();
                afficher_menu();
                consigne();
                break;
        }
    }
}

int main() {
    char nomFichier[100] = "restau.txt";
    utiliser_menu(nomFichier);
    return 0;
}

