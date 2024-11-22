//
// Created by basti on 14/11/2024.
//
#include <stdio.h>
#include "header.h"
#include <stdlib.h>
#include <string.h>


Graphe* lireFichier(char* nomFichier) {
    Graphe* graphe;
    FILE* ifs = fopen(nomFichier, "r");
    int ordre;
    float poids;
    char **sommet;//tab temporaire des sommets
    if (!ifs) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    //ordre
    fscanf(ifs, "%d", &ordre);
    graphe = (Graphe*)malloc(sizeof(struct Graphe));
    graphe->ordre = ordre;
    //allocation sommet
    graphe->pSommet = (pSommet*)malloc(ordre * sizeof(pSommet));
    //allocation capacite
    graphe->capacite = (float**)malloc(ordre * sizeof(float*));
    for (int i = 0; i < ordre; i++) {
        graphe->capacite[i] = (float*)calloc(ordre, sizeof(float));
    }
    //allocation sommet nom
    sommet = (char**)malloc(ordre * sizeof(char*));

    //info du sommet
    for (int i = 0; i < ordre; i++) {
        sommet[i] = (char*)malloc(100 * sizeof(char));//alloc pour les noms sommets
        fscanf(ifs, " %s", sommet[i]);
        graphe->pSommet[i] = (pSommet)malloc(sizeof(struct Sommet));
        graphe->pSommet[i]->arc = NULL;
        graphe->pSommet[i]->valeur = i; //indice
        graphe->pSommet[i]->nom = (char*)malloc(strlen(sommet[i]) + 1);
        strcpy(graphe->pSommet[i]->nom, sommet[i]); //nom
    }

    for (int i = 0; i < ordre; i++) {
        for (int j = 0; j < ordre; j++) {
            fscanf(ifs, "%f", &poids);
            graphe->capacite[i][j] = poids;

            if (poids > 0) {
                //condition si l'arc existe
                parc nouvelle_arc = (parc)malloc(sizeof(struct arc));
                nouvelle_arc->sommet = j; //sommet d'arrivée
                nouvelle_arc->valeur = poids; //poids de l'arete
                nouvelle_arc->arc_suivant = graphe->pSommet[i]->arc;
                graphe->pSommet[i]->arc = nouvelle_arc; //on lie l'arc au sommet

                //verif
                printf("arc %s -> %s = %f\n", sommet[i], sommet[j], poids);
            }
        }
    }
    fclose(ifs);
    return graphe;
}




void liberer_graphe(Graphe* graphe) {
    for (int i = 0; i < graphe->ordre; i++) {
        parc arc = graphe->pSommet[i]->arc;
        while (arc != NULL) {
            parc temp = arc;
            arc = arc->arc_suivant;
            free(temp);
        }
        free(graphe->pSommet[i]);
    }
    free(graphe->pSommet);
    free(graphe);
}




