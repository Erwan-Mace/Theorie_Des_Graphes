//
// Created by basti on 14/11/2024.
//

#ifndef CODE_BASE_PROJET_HEADER_H
#define CODE_BASE_PROJET_HEADER_H

#define  CoefMortalite 0.3
#define Nmax 100000000
#define  Kmin 0.0001
//struct d'un arc
struct arc
{
    int sommet; // numéro de sommet d'un arc adjacent au sommet initial
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
void Simu(Graphe* graphe);
Graphe* Graphe_semaine(Graphe* graphe,int semaine);
Graphe* Copie_graphe(Graphe* graphe_initial);
void Successeur_Predecesseur(Graphe* graphe, int sommet, int* successeurs, int* nbSuccesseurs, int* predesseurs, int* nbPredesseurs);
float SommePopMangee(int* successeurs, int nbSuccesseurs, Graphe* graphe, float* populations,int Sommetactuel);
void Afficher_N(Graphe* graphe);
void ecrireFichier(const Graphe* graphe, const char* nomFichier);
void afficher_centre(const char* texte, int largeur_console);
void dessiner_ligne(int largeur_console, char caractere);
void afficher_titre_3D();
void Analyse(Graphe* graphe);
void spredateurs(Graphe* graphe);
void unesource(Graphe* graphe);
int calculer_niveau(Graphe* graphe, int* niveaux, int index);
void liaisonniv(Graphe* graphe);
void centralite(Graphe* graphe);
void connexite(Graphe* graphe);
void lireFichier2(Graphe* graphe);
#endif //CODE_BASE_PROJET_HEADER_H
