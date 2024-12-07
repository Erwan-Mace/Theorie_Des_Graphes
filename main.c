

#include <Windows.h>

#include <stdio.h>
#include "header.h"
#include <stdlib.h>
#include <string.h>
int main() {
    const int largeur_console = 148;
    Graphe* g = NULL;
    char nom_fichier[50];
    int choix;

    while (1) {
        printf("\033[0;35m");
        // Afficher le titre en "3D"
        dessiner_ligne(largeur_console, '=');
        afficher_titre_3D();
        dessiner_ligne(largeur_console, '=');

        // Menu principal
        afficher_centre("Ecris un numero pour choisir ton graphe :", largeur_console);
        printf("\n");
        afficher_centre("1 : Mer", largeur_console);
        afficher_centre("2 : Foret", largeur_console);
        afficher_centre("3 : Savane", largeur_console);
        afficher_centre("4 : Quitter", largeur_console);
        printf("\n");

        // Saisie du choix
        printf("Entrez votre choix : ");
        printf("\033[0m");
        scanf("%d", &choix);
        printf("\033[0;35m");

        if (choix == 4) {
            break;
        }

        // Traitement du choix
        switch (choix) {
            case 1:
                printf("\nEcosysteme choisi : Mer\n");
                strcpy(nom_fichier, "Mer");
                break;
            case 2:
                printf("\nEcosysteme choisi : Foret\n");
                strcpy(nom_fichier, "matrice");
                break;
            case 3:
                printf("\nEcosysteme choisi : Savane\n");
                strcpy(nom_fichier, "Savane");
                break;
            default:
                printf("Nous n avons pas d ecosysteme pour un tel numero\n");
                continue;
        }

        // Lecture du graphe
        printf("\033[0m");
        g = lireFichier(nom_fichier);
        printf("\033[0;35m");
        if (!g) {
            printf("Erreur lors de la lecture du graphe.\n");
            continue;
        }

        while (1) {
            // Menu des actions
            printf("\033[0;35m");
            dessiner_ligne(largeur_console, '-');
            afficher_centre("Que voulez-vous faire ?", largeur_console);
            dessiner_ligne(largeur_console, '-');
            afficher_centre("1 : Lancer la simulation", largeur_console);
            afficher_centre("2 : Analyser le graphe", largeur_console);
            afficher_centre("3 : Revenir au menu principal", largeur_console);
            printf("\n");


            // Saisie du choix
            printf("Entrez votre choix : ");
            printf("\033[0m");
            scanf("%d", &choix);
            printf("\033[0;35m");
            system("cls");

            if (choix == 3) {
                liberer_graphe(g);
                g = NULL;
                break;
            }

            // Traitement du choix
            switch (choix) {
                case 1:
                    printf("\nSimulation\n");
                    Simu(g);
                    break;
                case 2:
                    printf("\nAnalyse du graphe\n");
                    Analyse(g);
                    break;
                case 3:
                    printf("\nSource\n");
                    //source
                default:
                    printf("Cette option n'existe pas\n");
            }
        }
    }

    // Libération de mémoire si nécessaire
    if (g) {
        liberer_graphe(g);
    }

    return 0;
}
