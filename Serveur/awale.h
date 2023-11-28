// awale.h

#ifndef AWALE_H
#define AWALE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define NB_CASES 6
#define GRAINES_DEPART 4
#define GRAINES_CAPTUREES 25

typedef struct {
    int id;
    char pseudo[50];
    int score;
} Joueur;

typedef struct {
    int camp1[6];
    int camp2[6];
    Joueur joueur1;
    Joueur joueur2;
    int tour;
    int finDePartie;
} Plateau;



typedef struct{
    Joueur * catalogueJoueur;
    Plateau plateau;
}AwaleGameData;
//extern struct AwaleGameData awaleGameData;

Joueur inscrireJoueur(char pseudo[]);
char* initialiserPlateau(char *pseudo1, char *pseudo2);
char* afficherPlateau();
void jouerCoup(int indice);
char* afficherGagnant(Plateau *plateau);
void jeu (char * pseudo1, char * pseudo2);
int finDePartie(); 
int aQuiLeTour(); 

#endif // AWALE_H