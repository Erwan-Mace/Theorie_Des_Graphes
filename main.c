

#include <Windows.h>

#include <stdio.h>
#include "header.h"
#include <stdlib.h>
#include <string.h>
int main() {

    Graphe* g = NULL;
    char nom_fichier[50];
    int choix;

    while (1) {
        printf("\033[0;35m");
        //affiche le titre en 3D
        dessiner_ligne('=');
        afficher_titre_3D();
        dessiner_ligne('=');

        afficher_centre("Ecris un numero pour choisir ton graphe :");
        printf("\n");
        afficher_centre("1 : Mer");
        afficher_centre("2 : Foret");
        afficher_centre("3 : Savane");
        afficher_centre("4 : ecosysteme sauvegarde");
        afficher_centre("5 : Quitter");
        printf("\n");

        printf("Entrez votre choix : ");
        printf("\033[0m");
        scanf("%d", &choix);
        printf("\033[0;35m");

        if (choix == 5) {
            break;
        }

        //swtich case des choix 
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
            case 4:
                printf("\nEcosysteme sauvegarde : veuillez rentrer son nom\n");
                scanf("%s",&*nom_fichier);

                break;
            default:
                printf("Nous n avons pas d ecosysteme pour un tel numero\n");
                continue;
        }

        printf("\033[0m");
        g = lireFichier(nom_fichier);
        printf("\033[0;35m");
        if (!g) {
            printf("Erreur lors de la lecture du graphe.\n");
            continue;
        }

        while (1) {
            //menu des choix
            printf("\033[0;35m");
            dessiner_ligne('-');
            afficher_centre("Que voulez-vous faire ?");
            dessiner_ligne('-');
            afficher_centre("1 : Lancer la simulation");
            afficher_centre("2 : Analyser le graphe");
            afficher_centre("3 : acceder aux Sources");
            afficher_centre("4 : Revenir au menu principal");
            printf("\n");

            //saisie du choix + couleur
            printf("Entrez votre choix : ");
            printf("\033[0m");
            scanf("%d", &choix);
            printf("\033[0;35m");
            system("cls");

            if (choix == 4) {
                liberer_graphe(g);
                g = NULL;
                break;
            }
            switch (choix) {
                case 1:
                    afficher_centre("Simulation");
                    dessiner_ligne('-');
                    Simu(g);
                    break;
                case 2:
                    afficher_centre("Analyse du graphe");
                    dessiner_ligne('-');
                    Analyse(g);
                    break;
                case 3:

                    Source();
                default:
                    printf("Cette option n'existe pas\n");
            }
        }
    }
    if (g) {
        liberer_graphe(g);
    }

    return 0;
}
