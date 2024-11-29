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
    int N;
    float coef;
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
void creer_dot(const char *fichier_texte, const char *fichier_dot);
void afficher_graphe_png(const char *fichier_dot);
void liaisons(Graphe* graphe);
void Simu(Graphe* graphe);
Graphe* Graphe_semaine(Graphe* graphe,int semaine);
Graphe* Copie_graphe(Graphe* graphe_initial);
void Successeur_Predecesseur(Graphe* graphe, int sommet, int* successeurs, int* nbSuccesseurs, int* predesseurs, int* nbPredesseurs);
float SommePopMangee(int* predecesseurs, int nbPredesseurs, Graphe* graphe, float* populations);
void Afficher_N(Graphe* graphe);
void ecrireFichier(const Graphe* graphe, const char* nomFichier);
#endif //CODE_BASE_PROJET
