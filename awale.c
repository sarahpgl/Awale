//
// Created by sarah on 22/11/2023.
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
    Joueur *joueurs;
}CatalogueJoueur;

CatalogueJoueur  *catalogueJoueur;

Joueur inscrireJoueur(char pseudo[]){
    Joueur j;
    j.id = rand();
    strcpy(j.pseudo, pseudo);
    j.score = 0;
    return j;
}

//met 4 graines dans chaques cases de chaque camp
void initialiserPlateau(Plateau *plateau, Joueur j1, Joueur j2){
    int j;
    for( j = 0; j < 6 ; j++){
        plateau->camp1[j] =GRAINES_DEPART;
        plateau->camp2[j] =GRAINES_DEPART;
    }
    plateau -> joueur1 = j1;
    plateau -> joueur2 = j2;
    plateau -> tour = 1;
    plateau -> finDePartie = 0;
}

//afficher dans le terminal le plateau
void afficherPlateau(Plateau *plateau) {
    int i;

    printf("Joueur 2 (camp du haut) : ");
    printf("%s ", plateau->joueur2.pseudo);
    printf("       score : %d\n", plateau->joueur2.score);

    for (i = 0; i < 6; i ++) {
        printf("| %d ", plateau->camp2[i]);
    }

    printf("|\n---------------------\n");

    for (i = 0; i < 6; i++) {
        printf("| %d ", plateau->camp1[i]);

    }

    printf("|\n---------------------\n");

    printf("Joueur 1 (camp du bas) : ");
    printf("%s ", plateau->joueur1.pseudo);
    printf("       score : %d\n", plateau->joueur1.score);
    printf("\n");

}


// Jouer les graines
void jouerCoup(Plateau *plateau, int indice) {
    int i, graines, caseCourante, tourComplet;

    if (plateau->tour == 1) {
        graines = plateau->camp1[indice];
        plateau->camp1[indice] = 0;
        if(indice==5){
            printf("je suis passe ici 1 fois\n");
            caseCourante = indice;
            tourComplet = 2;
        }else{
            caseCourante = (indice + 1);
            tourComplet = 1;
        }
    } else {
        graines = plateau->camp2[indice];
        plateau->camp2[indice] = 0;
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
        if (plateau->tour == 1) {
            //on seme les graines du joueur 1
            if( tourComplet == 1 && caseCourante < 5 && caseCourante >= 0){
                plateau->camp1[caseCourante]++;
                caseCourante ++;
            }else if (tourComplet == 1 && caseCourante == 5){
                plateau->camp1[caseCourante]++;
                tourComplet = 2;
            }else if(tourComplet == 2 && caseCourante <= 5 && caseCourante > 0){
                printf("je seme la premiere graine dans la case d'indice 5 du camp2\n");
                printf("%d\n", caseCourante);
                plateau->camp2[caseCourante]++;
                printf("nb de graines dans la case %d\n", plateau->camp2[caseCourante]);
                if(plateau->camp2[caseCourante] == 2){
                    plateau->camp2[caseCourante] = 0;
                    plateau->joueur1.score += 2;
                }
                if(plateau->camp2[caseCourante] == 3){
                    plateau->camp2[caseCourante] = 0;
                    plateau->joueur1.score += 3;
                }
                caseCourante --;
            }else if( tourComplet == 2 && caseCourante == 0){
                plateau->camp2[caseCourante]++;
                if(plateau->camp2[caseCourante] == 2){
                    plateau->camp2[caseCourante] = 0;
                    plateau->joueur1.score += 2;
                }
                if(plateau->camp2[caseCourante] == 3){
                    plateau->camp2[caseCourante] = 0;
                    plateau->joueur1.score += 3;
                }
                tourComplet = 1;
            }

        } else {
            //on seme les graines du joueur 2 et on capture les graines du camp adverse s'il gagne
            if( tourComplet == 1 && caseCourante < 5 && caseCourante >= 0){
                plateau->camp1[caseCourante]++;
                if(plateau->camp1[caseCourante] == 2){
                    plateau->camp1[caseCourante] = 0;
                    plateau->joueur2.score += 2;
                }
                if(plateau->camp1[caseCourante] == 3){
                    plateau->camp1[caseCourante] = 0;
                    plateau->joueur2.score += 3;
                }
                caseCourante ++;
            }else if (tourComplet == 1 && caseCourante == 5){
                plateau->camp1[caseCourante]++;
                if(plateau->camp1[caseCourante] == 2){
                    plateau->camp1[caseCourante] = 0;
                    plateau->joueur2.score += 2;
                }
                if(plateau->camp1[caseCourante] == 3){
                    plateau->camp1[caseCourante] = 0;
                    plateau->joueur2.score += 3;
                }
                tourComplet = 2;
            }else if(tourComplet == 2 && caseCourante <= 5 && caseCourante > 0){
                plateau->camp2[caseCourante]++;
                caseCourante --;
            }else if( tourComplet == 2 && caseCourante == 0) {
                plateau->camp2[caseCourante]++;
                tourComplet = 1;
            }
        }

    }

    // Conditions de fin de partie
    if (plateau->joueur1.score >= 25 || plateau->joueur2.score >= 25) {
        plateau->finDePartie = 1;
    }

    // Changer de tour
    if (plateau->tour == 1) {
        plateau->tour = 2;
    } else {
        plateau->tour = 1;
    }
}

void afficherGagnant (Plateau *plateau){
    printf("La partie est finie ! Le grand gagnant est  ");
    if(plateau->joueur1.score > plateau->joueur2.score){
        printf("le joueur 1 : %s avec un score de : %d points.\n", plateau->joueur1.pseudo, plateau->joueur1.score);
    }else{
        printf("le joueur 2 : %s avec un score de : %d points.\n", plateau->joueur2.pseudo, plateau->joueur2.score);
    }
}


int main() {

    int choix;
    char pseudo1[50];
    char pseudo2[50];
    Joueur j1, j2;
    Plateau plateau;
    int indice;

    printf("Hello, World!\n");
    do{
        printf("\nMenu : \n");
        printf("1 : Inscription\n");
        printf("2 : Commencer a jouer\n");
        printf("3 : Quitter\n");

        printf("Choisissez votre option :");
        scanf("%d",&choix);

        switch (choix) {
            case 1 :
                printf("\nEntrez votre pseudo du joueur 1 :");
                scanf("%s",&pseudo1);
                j1 = inscrireJoueur(pseudo1);
                printf("\nEntrez votre pseudo du joueur 2 :");
                scanf("%s",&pseudo2);
                j2 = inscrireJoueur(pseudo2);
                break;
            case 2 :
                int k;
                initialiserPlateau(&plateau, j1, j2);


                while (!plateau.finDePartie){
                    afficherPlateau(&plateau);
                    printf("Joueur %d, c'est votre tour.\n", plateau.tour);
                    printf("Choisissez une case (de 0 a 5) : ");
                    scanf("%d", &indice);
                    printf(("hello"));

                    jouerCoup(&plateau, indice);
                }

                afficherPlateau(&plateau);
                afficherGagnant(&plateau);
                break;

            case 3 :
                printf("Au revoir !\n");
                return 0;
                break;

            default:
                printf("Option invalide");

        }

    }while(choix!=3);



    return 0;
}
