//
// Created by basti on 14/11/2024.
//
#include <Windows.h>
#include <stdio.h>
#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <math.h>

Graphe* lireFichier(char* nomFichier) {
    Graphe* graphe;
    FILE* ifs = fopen(nomFichier, "r");
    int ordre;
    float poids;
    char **sommet;//tab temporaire des sommets
    int N_temp;
    float coef_temp;
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
    for (int i = 0; i < ordre; i++) {
        fscanf(ifs, " %f", &coef_temp);
        graphe->pSommet[i]->coef= coef_temp; //Nb population
    }
    for (int i = 0; i < ordre; i++) {
        fscanf(ifs, " %d", &N_temp);
        graphe->pSommet[i]->N = N_temp; //Nb population
    }
    fclose(ifs);
    return graphe;
}

// Fonction pour afficher les liaisons et les poids
void liaisons(Graphe* graphe) {
    printf("\nVerification des liaisons entre les noeuds :\n");

    for (int i = 0; i < graphe->ordre; i++) {
        printf("Liaisons pour %s : ", graphe->pSommet[i]->nom);
        bool liaison_existe = false;

        // Parcours des arcs sortants
        for (parc arc = graphe->pSommet[i]->arc; arc != NULL; arc = arc->arc_suivant) {
            if (liaison_existe) {
                printf(", ");
            }
            printf("%s (poids : %.2f)", graphe->pSommet[arc->sommet]->nom, arc->valeur);
            liaison_existe = true;
        }

        if (!liaison_existe) {
            printf("aucune liaison");
        }

        printf("\n");
    }

    printf("\nAnalyse terminee : Tous les noeuds sont connectes et affiches.\n");
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
void Simu(Graphe* graphe) {
    int semaine = 0;
    int touche;
    bool simulation = true;
    Graphe* graphe_actualiser = Copie_graphe(graphe); // Initialiser avec une copie du graphe original

    printf("Affichage graphe semaine %d\n", semaine);
    printf("\033[0m");
    Afficher_N(graphe_actualiser);
    printf("\033[0;32m");
    printf("fleche gauche : graphe semaine prochaine\n"
           "fleche droite : graphe semaine avant\n"
           "fleche haut : quitter la simulation\n"
           "fleche droite : sauvegarde le graphe actuel\n");
    printf("\033[0m");
    while (simulation) {
        _sleep(400);
        if (_kbhit()) { // Vérifie si une touche est appuyée
            touche = _getch();
            if (touche == 224) {
                touche = _getch();
                if (touche == 72) {
                    simulation = false; // Fin
                    printf("\nFin de la simulation\n");
                } else if (touche == 75) {
                    // Flèche gauche
                    if (semaine <= 0) {
                        printf("\nOn ne peut pas avoir de graphe avant semaine 0\n");
                    } else {
                        semaine -= 1;
                        printf("\033[0;35m");
                        printf("\nAffichage graphe semaine %d\n", semaine);
                        printf("\033[0m");
                        liberer_graphe(graphe_actualiser); // Libérer l'ancien graphe
                        graphe_actualiser = Graphe_semaine(graphe, semaine); // Créer un nouveau graphe
                        Afficher_N(graphe_actualiser);
                        ecrireFichier(graphe_actualiser, "grapheActuel");
                        const char *fichier_dot = "graphe.dot";
                        creer_dot("grapheActuel", fichier_dot);
                        afficher_graphe_png(fichier_dot);
                        printf("\033[0;32m");
                        printf("\nfleche gauche : graphe semaine prochaine\n"
                               "fleche droite : graphe semaine avant\n"
                               "fleche haut : quitter la simulation\n"
                               "fleche droite : sauvegarde le graphe actuel\n");
                        printf("\033[0m");
                    }
                } else if (touche == 77) {
                    // Flèche droite
                    semaine += 1;
                    printf("\033[0;35m");
                    printf("\nAffichage graphe semaine %d\n", semaine);
                    printf("\033[0m");
                    liberer_graphe(graphe_actualiser); // Libérer l'ancien graphe
                    graphe_actualiser = Graphe_semaine(graphe, semaine); // Créer un nouveau graphe
                    Afficher_N(graphe_actualiser);
                    ecrireFichier(graphe_actualiser, "grapheActuel");
                    const char *fichier_dot = "graphe.dot";
                    creer_dot("grapheActuel", fichier_dot);
                    afficher_graphe_png(fichier_dot);
                    printf("\033[0;32m");
                    printf("\nfleche gauche : graphe semaine prochaine\n"
                           "fleche droite : graphe semaine avant\n"
                           "fleche haut : quitter la simulation\n"
                           "fleche droite : sauvegarde le graphe actuel\n");
                    printf("\033[0m");
                } else if (touche == 80) {
                    // Flèche bas
                    char nom[50];
                    printf("\033[0;35m");
                    printf("\nDonnez le noms de la sauvegarde du graphe :\n");
                    printf("\033[0m");
                    scanf("%s",nom);
                    printf("\033[0;35m");
                    ecrireFichier(graphe_actualiser,nom);
                    printf("graphe %s sauvegardee\n",nom);
                    printf("\033[0;32m");
                    printf("\nfleche gauche : graphe semaine prochaine\n"
                           "fleche droite : graphe semaine avant\n"
                           "fleche haut : quitter la simulation\n"
                           "fleche droite : sauvegarde le graphe actuel\n");
                    printf("\033[0m");
                }
            }
        }
    }
    liberer_graphe(graphe_actualiser); // Libérer la mémoire du graphe actualisé
}


Graphe* Graphe_semaine(Graphe* graphe, int semaine) {
    Graphe* Graphe_simu = Copie_graphe(graphe);
    float* populations_anterieures = malloc(graphe->ordre * sizeof(float));

    // Initialiser les populations antérieures à la population actuelle
    for (int i = 0; i < graphe->ordre; i++) {
        populations_anterieures[i] = (float)Graphe_simu->pSommet[i]->N;
    }

    // Appliquer les calculs pour chaque semaine
    for (int t = 0; t < semaine; t++) {
        for (int i = 0; i < graphe->ordre; i++) {
            int successeurs[graphe->ordre];
            int nbSuccesseurs = 0;
            int predesseurs[graphe->ordre];
            int nbPredesseurs = 0;

            // Remplir les listes de successeurs et prédécesseurs
            Successeur_Predecesseur(graphe, i, successeurs, &nbSuccesseurs, predesseurs, &nbPredesseurs);


            float N_i = populations_anterieures[i];  // Population initiale pour ce tour
            float r_i = Graphe_simu->pSommet[i]->coef; // Coefficient de croissance
            float K_i = 0;

            // Calcul de K_i à partir des prédécesseurs
            for (int j = 0; j < nbPredesseurs; j++) {
                int pred = predesseurs[j];
                parc arc_pred = Graphe_simu->pSommet[pred]->arc;
                while (arc_pred != NULL) {
                    if (arc_pred->sommet == i) {
                        K_i += (float)Graphe_simu->pSommet[pred]->N * arc_pred->valeur;
                        break;
                    }
                    arc_pred = arc_pred->arc_suivant;
                }
            }

            // Vérification des conditions de K_i
            if (K_i < Kmin) {
                K_i = fmax(Kmin, N_i * 0.1); // Eviter que K_i devienne trop petit et que cela fausse les calculs
            }
            if (N_i / K_i > 1) {
                K_i = fmax(K_i, N_i); // Ajuster K_i de façon à éviter que la population dépasse la capacité
            }



            if (nbSuccesseurs == 0) {
                N_i += r_i * N_i * (1 - (N_i / K_i)) - N_i * CoefMortalite;
            } else if (nbPredesseurs == 0) {
                N_i += r_i * N_i - SommePopMangee(successeurs, nbSuccesseurs, Graphe_simu, populations_anterieures,i);
            } else {
                N_i += r_i * N_i * (1 - (N_i / K_i)) - SommePopMangee(successeurs, nbSuccesseurs, Graphe_simu, populations_anterieures,i);
            }

            if (N_i < 0) N_i = 0;
            if (N_i >= Nmax) N_i = Nmax;
            Graphe_simu->pSommet[i]->N = (int)N_i;
        }

        // Mettre à jour les populations antérieures pour le prochain tour
        for (int i = 0; i < graphe->ordre; i++) {
            populations_anterieures[i] = (float)Graphe_simu->pSommet[i]->N;
        }
    }

    free(populations_anterieures);
    return Graphe_simu;
}




Graphe* Copie_graphe(Graphe* graphe_initial) {
    if (!graphe_initial) {
        return NULL; // Si le graphe initial est nul, retourne NULL.
    }

    Graphe* grapheCopie = (Graphe*)malloc(sizeof(Graphe));
    grapheCopie->ordre = graphe_initial->ordre;

    // alloc sommets
    grapheCopie->pSommet = (pSommet*)malloc(grapheCopie->ordre * sizeof(pSommet));

    // alloc capacités
    grapheCopie->capacite = (float**)malloc(grapheCopie->ordre * sizeof(float*));
    for (int i = 0; i < grapheCopie->ordre; i++) {
        grapheCopie->capacite[i] = (float*)calloc(grapheCopie->ordre, sizeof(float));
    }

    //copie
    for (int i = 0; i < grapheCopie->ordre; i++) {
        grapheCopie->pSommet[i] = (pSommet)malloc(sizeof(struct Sommet));
        grapheCopie->pSommet[i]->valeur = graphe_initial->pSommet[i]->valeur;

        //nom
        grapheCopie->pSommet[i]->nom = (char*)malloc(strlen(graphe_initial->pSommet[i]->nom) + 1);
        strcpy(grapheCopie->pSommet[i]->nom, graphe_initial->pSommet[i]->nom);

        //coef
        grapheCopie->pSommet[i]->coef = graphe_initial->pSommet[i]->coef;

        //nb pop
        grapheCopie->pSommet[i]->N = graphe_initial->pSommet[i]->N;

        //arcs
        grapheCopie->pSommet[i]->arc = NULL;
        parc arcCourantOriginal = graphe_initial->pSommet[i]->arc;
        parc* arcCopieCourant = &(grapheCopie->pSommet[i]->arc);
        while (arcCourantOriginal) {
            *arcCopieCourant = (parc)malloc(sizeof(struct arc));
            (*arcCopieCourant)->sommet = arcCourantOriginal->sommet;
            (*arcCopieCourant)->valeur = arcCourantOriginal->valeur;
            (*arcCopieCourant)->arc_suivant = NULL;

            arcCopieCourant = &((*arcCopieCourant)->arc_suivant);
            arcCourantOriginal = arcCourantOriginal->arc_suivant;
        }
    }

    //copie capacités
    for (int i = 0; i < grapheCopie->ordre; i++) {
        for (int j = 0; j < grapheCopie->ordre; j++) {
            grapheCopie->capacite[i][j] = graphe_initial->capacite[i][j];
        }
    }

    return grapheCopie;
}

void Successeur_Predecesseur(Graphe* graphe, int sommet, int* successeurs, int* nbSuccesseurs, int* predesseurs, int* nbPredesseurs) {
    *nbSuccesseurs = 0;
    *nbPredesseurs = 0;
    for (int i = 0; i < graphe->ordre; i++) {
        parc arc = graphe->pSommet[i]->arc;
        // Recherche des prédécesseurs
        while (arc != NULL) {
            if (arc->sommet == sommet) {
                predesseurs[*nbPredesseurs] = i;
                (*nbPredesseurs)++;
                break;
            }
            arc = arc->arc_suivant;
        }
    }
    // Recherche des successeurs
    parc arc = graphe->pSommet[sommet]->arc; // Partir des arcs du sommet donné
    while (arc != NULL) {
        successeurs[*nbSuccesseurs] = arc->sommet; // Ajouter le sommet destination comme successeur
        (*nbSuccesseurs)++;
        arc = arc->arc_suivant;
    }
}

float SommePopMangee(int* successeurs, int nbSuccesseurs, Graphe* graphe, float* populations,int Sommetactuel) {
    float somme = 0;
    parc arc = graphe->pSommet[Sommetactuel]->arc;
    for (int i = 0; i < nbSuccesseurs; i++) {
        if(arc!=NULL) {
            somme += arc->valeur * populations[arc->sommet];
            arc = arc->arc_suivant;
        }
    }
    return somme;
}






void Afficher_N(Graphe* graphe) {
    for (int i = 0; i < graphe->ordre; i++) {
        printf("%s : N = %d\n", graphe->pSommet[i]->nom, graphe->pSommet[i]->N);
    }
}

void ecrireFichier(const Graphe* graphe, const char* nomFichier) {
    FILE* ofs = fopen(nomFichier, "w");
    if (!ofs) {
        printf("Erreur : impossible de créer le fichier texte %s.\n", nomFichier);
        return;
    }

    // Écrire l'ordre du graphe
    fprintf(ofs, "%d\n", graphe->ordre);

    // Écrire les noms des sommets
    for (int i = 0; i < graphe->ordre; i++) {
        fprintf(ofs, "%s ", graphe->pSommet[i]->nom);
    }
    fprintf(ofs, "\n");

    // Écrire la matrice des capacités
    for (int i = 0; i < graphe->ordre; i++) {
        for (int j = 0; j < graphe->ordre; j++) {
            fprintf(ofs, "%d ", (int)graphe->capacite[i][j]);
        }
        fprintf(ofs, "\n");
    }

    // Écrire les coefficients de croissance
    for (int i = 0; i < graphe->ordre; i++) {
        fprintf(ofs, "%.4f ", graphe->pSommet[i]->coef);
    }
    fprintf(ofs, "\n");

    // Écrire les populations initiales
    for (int i = 0; i < graphe->ordre; i++) {
        fprintf(ofs, "%d ", graphe->pSommet[i]->N);
    }
    fprintf(ofs, "\n");

    fclose(ofs);
}


void creer_dot(const char *fichier_texte, const char *fichier_dot) {
    FILE *f_texte = fopen(fichier_texte, "r");
    if (!f_texte) {
        printf("Erreur : impossible d'ouvrir le fichier texte %s.\n", fichier_texte);
        return;
    }

    FILE *f_dot = fopen(fichier_dot, "w");
    if (!f_dot) {
        printf("Erreur : impossible de creer le fichier DOT %s.\n", fichier_dot);
        fclose(f_texte);
        return;
    }

    int nb_sommets;
    fscanf(f_texte, "%d", &nb_sommets);

    char noms[nb_sommets][50];
    for (int i = 0; i < nb_sommets; i++) {
        fscanf(f_texte, "%s", noms[i]);
    }

    fprintf(f_dot, "digraph G {\n");

    // Lire la matrice d'adjacence
    float poids;
    for (int i = 0; i < nb_sommets; i++) {
        for (int j = 0; j < nb_sommets; j++) {
            fscanf(f_texte, "%f", &poids);
            if (poids > 0) {
                fprintf(f_dot, "    %s -> %s [label=\"%.1f\"];\n", noms[i], noms[j], poids);
            }
        }
    }

    // Ignorer la ligne des taux de croissance
    for (int i = 0; i < nb_sommets; i++) {
        fscanf(f_texte, "%f", &poids);
    }

    // Lire la ligne des populations initiales et l'ajouter aux nœuds
    float population_initiale[nb_sommets];
    for (int i = 0; i < nb_sommets; i++) {
        fscanf(f_texte, "%f", &population_initiale[i]);
        fprintf(f_dot, "    %s [label=\"%s\nPop: %.0f\"];\n", noms[i], noms[i], population_initiale[i]);
    }

    fprintf(f_dot, "}\n");

    fclose(f_texte);
    fclose(f_dot);

    printf("Le fichier DOT est genere : %s\n", fichier_dot);
}
// Fonction pour afficher le graphe sous forme d'image PNG
void afficher_graphe_png(const char *fichier_dot) {
    char commande[200];
    char fichier_png[100];

    strcpy(fichier_png, fichier_dot);
    char *extension = strrchr(fichier_png, '.');
    if (extension) {
        strcpy(extension, ".png");
    } else {
        strcat(fichier_png, ".png");
    }

    sprintf(commande, "dot -Tpng %s -o %s", fichier_dot, fichier_png);
    int ret = system(commande);
    if (ret != 0) {
        printf("Erreur : impossible de générer l'image PNG. Vérifiez que Graphviz est installé.\n");
        return;
    }

    sprintf(commande, "start %s", fichier_png); // Windows
    // Pour Linux, utilisez : sprintf(commande, "xdg-open %s", fichier_png);
    // Pour macOS, utilisez : sprintf(commande, "open %s", fichier_png);
    system(commande);

    printf("Le graphe PNG est affiche : %s\n", fichier_png);
}

void afficher_centre(const char* texte, int largeur_console) {
    int longueur = strlen(texte);
    int espaces = (largeur_console - longueur) / 2;
    for (int i = 0; i < espaces; i++) {
        printf(" ");
    }
    printf("%s\n", texte);
}

// Fonction pour dessiner une ligne horizontale
void dessiner_ligne(int largeur_console, char caractere) {
    for (int i = 0; i < largeur_console; i++) {
        printf("%c", caractere);
    }
    printf("\n");
}


void afficher_titre_3D() {
    SetConsoleOutputCP(65001);
    printf("\n");
    printf("████████╗ ██████╗    ██████╗  ██████╗  ██╗  ██╗ ██╗  ██████╗     ███╗   ██╗ ███████╗ ████████╗ ██╗    ██╗  ██████╗  ██████╗  ██╗  ██╗ ███████╗\n");
    printf("╚══██╔══╝ ██╔══██╗  ██╔═══██╗ ██╔══██╗ ██║  ██║ ██║ ██╔═══██╗    ████╗  ██║ ██╔════╝ ╚══██╔══╝ ██║    ██║ ██╔═══██╗ ██╔══██╗ ██║ ██╔╝ ██╔════╝\n");
    printf("   ██║    ██████╔╝  ██║   ██║ ██████╔╝ ███████║ ██║ ██║          ██╔██╗ ██║ █████╗      ██║    ██║ █╗ ██║ ██║   ██║ ██████╔╝ █████╔╝  ███████╗\n");
    printf("   ██║    ██╔══██╗  ██║   ██║ ██╔═══╝  ██╔══██║ ██║ ██║          ██║╚██╗██║ ██╔══╝      ██║    ██║███╗██║ ██║   ██║ ██╔══██╗ ██╔═██╗  ╚════██║\n");
    printf("   ██║    ██║   ██╗ ╚██████╔╝ ██║      ██║  ██║ ██║ ╚██████╔╝    ██║ ╚████║ ███████╗    ██║    ╚███╔███╔╝ ╚██████╔╝ ██║   ██╗██║  ██╗ ███████║\n");
    printf("   ╚═╝    ╚═╝   ╚═╝  ╚═════╝  ╚═╝      ╚═╝  ╚═╝ ╚═╝  ╚═════╝     ╚═╝  ╚═══╝ ╚══════╝    ╚═╝     ╚══╝╚══╝   ╚═════╝  ╚═╝   ╚═╝╚═╝  ╚═╝ ╚══════╝\n");
    printf("\n");
}

void Analyse(Graphe* graphe) {
    int choix;
    do {

        printf("\n1. Verifier si le graphe est connexe\n");
        printf("2. Afficher les derniers maillons (sans predateurs)\n");
        printf("3. Afficher les especes ayant une seule source de nourriture\n");
        printf("4. Afficher le reseau trophique\n");
        printf("5. Simuler la disparition d une espece\n");
        printf("6. Estimer l importance d une espece (centralite)\n");
        printf("7. Retourner en arriere\n");
        printf("Votre choix : ");
        printf("\033[0m");
        scanf("%d", &choix);
        printf("\033[0;35m");

        switch (choix) {
            case 1:
                system("cls");
                printf("\nVerification de la connexite...\n");
                printf("\033[0m");
                connexite(graphe);
                printf("\033[0;35m");
                break;
            case 2:
                system("cls");
                printf("\nAffichage des derniers maillons...\n");
                printf("\033[0m");
                spredateurs(graphe);
                printf("\033[0;35m");
                break;
            case 3:
                system("cls");
                printf("\nAffichage des especes avec une seule source de nourriture...\n");
                printf("\033[0m");
                unesource(graphe);
                printf("\033[0;35m");
                break;
            case 4:
                system("cls");
                printf("\nAffichage du reseau trophique...\n");
                printf("\033[0m");
                liaisonniv(graphe);
                printf("\033[0;35m");
                break;
            case 5:
                system("cls");
                printf("\nSimulation de la disparition d une espece...\n");
                int reponse;
                for(int i=0;i<graphe->ordre;i++){
                    printf("%d : %s\n",i,graphe->pSommet[i]->nom);
                }
                printf("Voici la liste des especes, Laquel voulez vous faire disparaitre :\n");
                printf("\033[0m");
                scanf("%d",&reponse);
                printf("\033[0;35m");
                if(reponse>=0 && reponse<graphe->ordre){
                    Graphe* Graphe_simu = Copie_graphe(graphe);
                    Graphe_simu->pSommet[reponse]->N=0;
                    Simu(Graphe_simu);
                }
                else{
                    printf("ce sommet n existe pas \n");
                }
                system("cls");
                break;
            case 6:
                system("cls");
                printf("\nEstimation de l importance d une espece...\n");
                printf("\033[0m");
                centralite(graphe);
                printf("\033[0;35m");
                break;
            case 7:
                system("cls");
                printf("\nRetour au menu principal...\n");
                break;
            default:
                printf("\nChoix invalide. Veuillez reessayer.\n");
        }
    } while (choix != 7);
}
void liaisonniv(Graphe* graphe) {
    int ordre = graphe->ordre;
    int niveaux[ordre];
    for (int i = 0; i < ordre; i++) {
        niveaux[i] = 0;
    }
    for (int i = 0; i < ordre; i++) {
        calculer_niveau(graphe, niveaux, i);
    }
    printf("\nLiaisons entre les noeuds (avec niveaux trophiques) :\n");
    for (int i = 0; i < ordre; i++) {
        printf(" %s (Niveau trophique : %d) : ", graphe->pSommet[i]->nom, niveaux[i]);
        parc arc = graphe->pSommet[i]->arc;
        if (!arc) {
            printf("aucune liaison\n");
            continue;
        }
        while (arc) {
            printf("%s (poids : %.2f)", graphe->pSommet[arc->sommet]->nom, arc->valeur);
            if (arc->arc_suivant) {
                printf(", ");
            }
            arc = arc->arc_suivant;
        }
        printf("\n");
    }
}

int calculer_niveau(Graphe* graphe, int* niveaux, int index) {
    if (niveaux[index] > 0) {
        return niveaux[index]; // si deja calcule retourner le niveau
    }
    int unpredateur = 0;
    int uneproie = 0;
    // verifier les predateurs
    for (int i = 0; i < graphe->ordre; i++) {
        parc pred = graphe->pSommet[i]->arc;
        while (pred) {
            if (pred->sommet == index) {
                unpredateur = 1;
                break;
            }
            pred = pred->arc_suivant;
        }
        if (unpredateur) break;
    }
    parc arc = graphe->pSommet[index]->arc;
    if (arc) {
        uneproie = 1;
    }
    if (unpredateur && !uneproie) {
        niveaux[index] = 3;
    } else if (unpredateur && uneproie) {
        niveaux[index] = 2;
    } else if (!unpredateur && uneproie) {
        niveaux[index] = 1;
    } else {
        niveaux[index] = -1; //sommet isole
    }
    return niveaux[index];
}

void unesource(Graphe* graphe) {
    printf("\nEspeces ayant une seule source de nourriture :\n");

    // verifier chaque sommet pour ses arcs entrants
    for (int i = 0; i < graphe->ordre; i++) {
        int nb_sources = 0; // compteur pour les arcs entrants

        // parcours des arcs entrants
        for (int j = 0; j < graphe->ordre; j++) {
            parc arc = graphe->pSommet[j]->arc;
            while (arc != NULL) {
                if (arc->sommet == i) { // si l'arc pointe vers le sommet i
                    nb_sources++;
                    break;
                }
                arc = arc->arc_suivant;
            }
        }
        // si une espèce a exactement une source de nourriture
        if (nb_sources == 1) {
            printf("- %s\n", graphe->pSommet[i]->nom);
        }
    }
}

void spredateurs(Graphe* graphe) {
    printf("\nAnimaux sans predateurs (super-predateurs) :\n");
    bool trouve = false;
    // parcours de chaque sommet
    for (int i = 0; i < graphe->ordre; i++) {
        bool a_une_proie = false;
        // verification des arcs sortants
        for (int j = 0; j < graphe->ordre; j++) {
            if (graphe->capacite[i][j] > 0) { // verifie les arcs sortants pour chaque sommet
                a_une_proie = true;
                break;
            }
        }

        // Si aucune proie trouvée (ligne entièrement 0), c'est un super-predateur
        if (!a_une_proie) {
            printf("- %s\n", graphe->pSommet[i]->nom);
            trouve = true;
        }
    }

    if (!trouve) {
        printf("Aucun super-predateur trouve.\n");
    }
    printf("\n");
}

void lireFichier2(Graphe* graphe) {
    if (!graphe) {
        printf("Erreur : graphe inexistant.\n");
        return;
    }
    for (int i = 0; i < graphe->ordre; i++) {
        for (int j = 0; j < graphe->ordre; j++) {
            if (graphe->capacite[i][j] > 0) {
                // Assurer que la capacité est symétrique
                graphe->capacite[j][i] = graphe->capacite[i][j];
                // Ajouter l'arc opposé dans la liste des arcs
                parc arc_courant = graphe->pSommet[j]->arc;
                bool arc_existe = false;
                // Vérifier si l'arc opposé existe déjà
                while (arc_courant != NULL) {
                    if (arc_courant->sommet == i) {
                        arc_existe = true;
                        break;
                    }
                    arc_courant = arc_courant->arc_suivant;
                }
                // Ajouter l'arc si inexistant
                if (!arc_existe) {
                    parc nouvel_arc = (parc)malloc(sizeof(struct arc));
                    nouvel_arc->sommet = i;
                    nouvel_arc->valeur = graphe->capacite[j][i];
                    nouvel_arc->arc_suivant = graphe->pSommet[j]->arc;
                    graphe->pSommet[j]->arc = nouvel_arc;
                }
            }
        }
    }
}

void connexite(Graphe* graphe) {
    if (graphe == NULL) {
        printf("Le graphe est vide.\n");
        return;
    }
    // Convertir le graphe en non orienté
    Graphe* Graphe_copie = Copie_graphe(graphe);

    lireFichier2(Graphe_copie);
    bool* visites = (bool*)calloc(Graphe_copie->ordre, sizeof(bool));
    int* file = (int*)malloc(Graphe_copie->ordre * sizeof(int));
    int debut = 0, fin = 0;
    // Initialiser BFS à partir du premier sommet
    visites[0] = true;
    file[fin++] = 0;
    while (debut < fin) {
        int sommet = file[debut++];
        // Parcourir les voisins du sommet actuel
        for (parc arc = Graphe_copie->pSommet[sommet]->arc; arc != NULL; arc = arc->arc_suivant) {
            if (!visites[arc->sommet]) {
                visites[arc->sommet] = true;
                file[fin++] = arc->sommet;
            }
        }
    }
    // Vérifier si tous les sommets ont été visités
    bool connexe = true;
    for (int i = 0; i < Graphe_copie->ordre; i++) {
        if (!visites[i]) {
            connexe = false;
            break;
        }
    }
    if (connexe) {
        printf("Le graphe est connexe.\n");
    } else {
        printf("Le graphe n'est pas connexe.\n");
    }
    free(visites);
    free(file);
    liberer_graphe(Graphe_copie);
}

void centralite(Graphe* graphe) {
    // Tableau pour stocker le nombre de liaisons (degré) pour chaque espèce
    int* degres = (int*)calloc(graphe->ordre, sizeof(int));
    // Calculer le degré de chaque sommet (espèce)
    for (int i = 0; i < graphe->ordre; i++) {
        parc arc = graphe->pSommet[i]->arc;
        while (arc != NULL) {
            degres[i]++; // Chaque arc entrant compte comme un lien pour cette espèce
            arc = arc->arc_suivant;
        }
    }
    // Tri des sommets par leur nombre de liaisons, de manière décroissante
    for (int i = 0; i < graphe->ordre - 1; i++) {
        for (int j = i + 1; j < graphe->ordre; j++) {
            if (degres[i] < degres[j]) {
                // Échange les degrés
                int temp = degres[i];
                degres[i] = degres[j];
                degres[j] = temp;
                // Échange les noms des sommets (espèces)
                char* temp_nom = graphe->pSommet[i]->nom;
                graphe->pSommet[i]->nom = graphe->pSommet[j]->nom;
                graphe->pSommet[j]->nom = temp_nom;
            }
        }
    }
    // Afficher les espèces avec le plus grand nombre de liaisons
    printf("Centralite de degre (especes les plus connectees) :\n");
    for (int i = 0; i < graphe->ordre; i++) {
        printf("- %s : %d liaisons\n", graphe->pSommet[i]->nom, degres[i]);
    }
    free(degres);
}
