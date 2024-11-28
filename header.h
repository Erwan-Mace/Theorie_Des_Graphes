//
// Created by basti on 14/11/2024.
//

#ifndef CODE_BASE_PROJET_HEADER_H
#define CODE_BASE_PROJET_HEADER_H

#define INF 10000
//struct d'un arc
struct arc
{
    int sommet; // numéro de sommet d'une arc adjacente au sommet initial
    float valeur;
    struct arc* arc_suivant;
};
typedef struct arc* parc;

//struct du sommet
struct Sommet
{
    struct arc* arc;
    int valeur;
    char *nom;

};
typedef struct Sommet* pSommet;


typedef struct Graphe
{
    int ordre;
    pSommet* pSommet;
    float** capacite;
} Graphe;


Graphe * lireFichier(char * nomFichier);
void liberer_graphe(Graphe* graphe);
//int bfs(Graphe* g, int source, int puits, int* parent);
void liaisons(Graphe* graphe);


#endif //CODE_BASE_PROJET_HEADER_H
