//
// Created by sarah on 22/11/2023.
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


#define NB_CASES 6;
#define GRAINES_DEPART 4;
#define GRAINES_CAPTUREES 25;


typedef struct {
    int id;
    char pseudo[50];
    int score;
}Joueur;

typedef struct {
    int camp1[6];
    int camp2[6];
    Joueur joueur1;
    Joueur joueur2;
    int tour;
    int finDePartie;
}Plateau;

/*typedef struct{
    Plateau plateau;
    Joueur joueurs[2];
    Joueur observateur;
}Partie;*/

Joueur  * catalogueJoueur;
Plateau plateau;


Joueur inscrireJoueur(char* pseudo){
    Joueur j;
    j.id = rand();
    strcpy(j.pseudo, pseudo);
    j.score = 0;
    
    return j;
}


//afficher dans le terminal le plateau
char* afficherPlateau() {
    char* affichage = (char*)malloc(500);

    sprintf(affichage, "Joueur 2 %s (camp du haut), score : %d\n", plateau.joueur2.pseudo, plateau.joueur2.score);

    int i;
    for (i = 0; i < 6; i ++) {
        sprintf(affichage + strlen(affichage), "| %d ", plateau.camp2[i]);
    }

    sprintf(affichage + strlen(affichage), "|\n---------------------\n");

    for (i = 0; i < 6; i++) {
        sprintf(affichage + strlen(affichage), "| %d ", plateau.camp1[i]);

    }

    sprintf(affichage + strlen(affichage), "|\n---------------------\n");

    sprintf(affichage + strlen(affichage), "Joueur 1 %s (camp du bas), score : %d\n", plateau.joueur1.pseudo, plateau.joueur1.score);

    return affichage; 
}

//met 4 graines dans chaques cases de chaque camp
char* initialiserPlateau(char* pseudo1, char* pseudo2){
     
    int j;
    for(j = 0; j < 6 ; j++){
        plateau.camp1[j] = GRAINES_DEPART;
        plateau.camp2[j] = GRAINES_DEPART;
    }
    plateau.joueur1 = inscrireJoueur(pseudo1);
    plateau.joueur2 = inscrireJoueur(pseudo2);
    plateau.tour = 1;
    plateau.finDePartie = 0;

    char *premierAffichage = afficherPlateau(); 
    return premierAffichage; 
}

// Jouer les graines
void jouerCoup(int indice) {
    int i, graines, caseCourante, tourComplet;

    if (plateau.tour == 1) {
        graines = plateau.camp1[indice];
        plateau.camp1[indice] = 0;
        if(indice==5){
            caseCourante = indice;
            tourComplet = 2;
        }else{
            caseCourante = (indice + 1);
            tourComplet = 1;
        }
    } else {
        graines = plateau.camp2[indice];
        plateau.camp2[indice] = 0;
        if(indice == 0) {
            caseCourante = indice;
            tourComplet = 1;
        }else{
            caseCourante = (indice - 1);
            tourComplet = 2;
        }
    }

    // Jouer les graines

    for (i = 0; i < graines; i++) {
        // Semez une graine dans la case courante du joueur courant et on capture les graines du camp adverse s'il gagne
        if (plateau.tour == 1) {
            //on seme les graines du joueur 1
            if( tourComplet == 1 && caseCourante < 5 && caseCourante >= 0){
                plateau.camp1[caseCourante]++;
                caseCourante ++;
            }else if (tourComplet == 1 && caseCourante == 5){
                plateau.camp1[caseCourante]++;
                tourComplet = 2;
            }else if(tourComplet == 2 && caseCourante <= 5 && caseCourante > 0){
                plateau.camp2[caseCourante]++;
                if(plateau.camp2[caseCourante] == 2){
                    plateau.camp2[caseCourante] = 0;
                    plateau.joueur1.score += 2;
                }
                if(plateau.camp2[caseCourante] == 3){
                    plateau.camp2[caseCourante] = 0;
                    plateau.joueur1.score += 3;
                }
                caseCourante --;
            }else if( tourComplet == 2 && caseCourante == 0){
                plateau.camp2[caseCourante]++;
                if(plateau.camp2[caseCourante] == 2){
                    plateau.camp2[caseCourante] = 0;
                    plateau.joueur1.score += 2;
                }
                if(plateau.camp2[caseCourante] == 3){
                    plateau.camp2[caseCourante] = 0;
                    plateau.joueur1.score += 3;
                }
                tourComplet = 1;
            }

        } else {
            //on seme les graines du joueur 2 et on capture les graines du camp adverse s'il gagne
            if( tourComplet == 1 && caseCourante < 5 && caseCourante >= 0){
                plateau.camp1[caseCourante]++;
                if(plateau.camp1[caseCourante] == 2){
                    plateau.camp1[caseCourante] = 0;
                    plateau.joueur2.score += 2;
                }
                if(plateau.camp1[caseCourante] == 3){
                    plateau.camp1[caseCourante] = 0;
                    plateau.joueur2.score += 3;
                }
                caseCourante ++;
            }else if (tourComplet == 1 && caseCourante == 5){
                plateau.camp1[caseCourante]++;
                if(plateau.camp1[caseCourante] == 2){
                    plateau.camp1[caseCourante] = 0;
                    plateau.joueur2.score += 2;
                }
                if(plateau.camp1[caseCourante] == 3){
                    plateau.camp1[caseCourante] = 0;
                    plateau.joueur2.score += 3;
                }
                tourComplet = 2;
            }else if(tourComplet == 2 && caseCourante <= 5 && caseCourante > 0){
                plateau.camp2[caseCourante]++;
                caseCourante --;
            }else if( tourComplet == 2 && caseCourante == 0) {
                plateau.camp2[caseCourante]++;
                tourComplet = 1;
            }
        }

    }

    // Conditions de fin de partie
    if (plateau.joueur1.score >= 25 || plateau.joueur2.score >= 25) {
        plateau.finDePartie = 1;
    }

    // Changer de tour
    if (plateau.tour == 1) {
        plateau.tour = 2;
    } else {
        plateau.tour = 1;
    }
}

char* afficherGagnant (Plateau *plateau){
    char* resultat = (char*)malloc(100); 
    sprintf(resultat, "La partie est finie ! Le grand gagnant est  ");
    if(plateau->joueur1.score > plateau->joueur2.score){
        sprintf(resultat + strlen(resultat), "le joueur 1 : %s avec un score de : %d points.\n", plateau->joueur1.pseudo, plateau->joueur1.score);
    }else{
        sprintf(resultat + strlen(resultat), "le joueur 2 : %s avec un score de : %d points.\n", plateau->joueur2.pseudo, plateau->joueur2.score);
    }

    return resultat; 
}

int finDePartie(){
    printf("Je rentre dans fin de partie\n");
    if(plateau.finDePartie == 1){
        return 1;
    }else{
        return -1; 
    }
}

int aQuiLeTour(){
    printf("Je rentre dans aQuiLeTour\n");
    if(plateau.tour == 1){
        return 1; 
    }else{
        return 2; 
    }
}



void jeu (char * pseudo1, char * pseudo2) {

    //int choix;
    //Joueur j1;
    /*Joueur j2;
    int indice;
  
    
    j1 = inscrireJoueur(pseudo1);
    j2 = inscrireJoueur(pseudo2);
    initialiserPlateau(plateau, j1, j2);

    while (!plateau->finDePartie){
        afficherPlateau(plateau);
        printf("Joueur %d, c'est votre tour.\n", plateau->tour);
        printf("Choisissez une case (de 0 a 5) : ");
        scanf("%d", &indice);
        printf(("hello"));

        jouerCoup(plateau, indice);
    }

    afficherPlateau(plateau);
    afficherGagnant(plateau);*/

}
