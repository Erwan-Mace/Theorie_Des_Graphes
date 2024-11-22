#include <stdio.h>
#include "header.h"
#include <stdlib.h>
#include <string.h>

int main() {
    Graphe* g;
    char nom_fichier[50];
    int choix;

    printf("\n-menu-\n");
    printf("-ecris un numero pour choisir ton graphe-\n");
    printf("-1 : Mer -\n");
    printf("-2 : Foret -\n");
    printf("-3 : Savane -\n");

    scanf("%d", &choix);

    switch (choix) {
        case 1:
            printf("\necosysteme choisis : Mer \n");
            strcpy(nom_fichier, "matrice");
            break;
        case 2:
            printf("\necosysteme choisis : Foret \n");
            strcpy(nom_fichier, "matrice");
            break;
        case 3:
            printf("\necosysteme choisis : Savane \n");
            strcpy(nom_fichier, "matrice");
            break;
        default:
            printf("Nous n avons pas d ecosysteme pour un tel numero");
    }

    g = lireFichier(nom_fichier);//lecture de la matrice
    printf("\nVous voulez :\n");
    printf("1 : lancer la simulation\n");
    printf("2 : annalyser le graphe\n");
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            printf("-Simulation-\n");
            printf("Choisissez le nombre d annee de votre simulation :\n");
            break;
        case 2:
            printf("-annalyse du graphe-\n");
            printf("Choisissez une option d analyse :\n");
            break;
        default:
            printf("Cette option n existe pas");
    }
    //les free memoire
    liberer_graphe(g);

    return 0;
}
