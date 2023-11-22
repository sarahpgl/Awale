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
    int role;
    char pseudo[50];
    int score;
}Joueur;

typedef struct {
    int camp1[6];
    int camp2[6];
    int nbGrainesJeu;
}Plateau;

typedef struct{
    Plateau plateau;
    Joueur joueurs[2];
    Joueur observateur;
}Partie;

typedef struct {
    Joueur *joueurs;
}CatalogueJoueur;

CatalogueJoueur  *catalogueJoueur;

/*void definirRole(int role)
{
    if(role ==1){
        j1->role= role;
    }else if (role == 2){
        j2->role=role;
    }else{
        observateur.role=role;
    }
}*/

Joueur inscrireJoueur(char pseudo[]){
    Joueur j;
    j.id = rand();
    strcpy(j.pseudo, pseudo);
    j.score =0;
    j.role =0;
    catalogueJoueur->joueurs[j.id]= j;
}

Joueur trouverJoueur(char pseudo[]){
    for(int i = 0 ; i< sizeof (catalogueJoueur->joueurs); i++){
        if (strcmp(catalogueJoueur->joueurs[i].pseudo, pseudo)){
            return catalogueJoueur->joueurs[i];
        }
    }
    Joueur j;
    return j ;
}

//met 4 graines dans chaques cases de chaque camp
Plateau initialiserPlateau(){
    Plateau plateau;
    int j;
    for( j = 0; j < 6 ; j++){
        plateau.camp1[j] =GRAINES_DEPART;
        plateau.camp2[j] =GRAINES_DEPART;
    }
    plateau.nbGrainesJeu = 48;
}


int main() {
    Plateau plateau;

    int choix;
    char pseudo[50];

    printf("Hello, World!\n");
    do{
        printf("\nMenu : \n");
        printf("1 : Inscription\n");
        printf("2 : Commencer à jouer\n");
        printf("3 : Quitter\n");
        printf("Choisissez votre option :");
        scanf("%d",&choix);
        switch (choix) {
            case 1 :
                printf("\nEntrez votre pseudo :");
                scanf("%s",&pseudo);
                Joueur j1 = inscrireJoueur(pseudo);
                break;
            case 2 :
                printf("\nVous voulez jouer : entrez votre pseudo :");
                scanf("%s",&pseudo);
                 = (pseudo);
                break;

            default:
                printf("Option invalide");

        }

    }while(choix!=3);



    return 0;
}
