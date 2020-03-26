#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>

/*-----------------------------------------------------------------------------------*/
/* Auteur : Régis Evrard                                                             */
/* Groupe : 2201                                                                     */
/* Application : LC20_Evrard_Régis_2.c                                               */
/* Date de la dernière maj de l'exercice : 06-03-2019                                */
/*-----------------------------------------------------------------------------------*/

#define MAX_LIGNES 6
#define MAX_COL 7
#define MAX_PION MAX_LIGNES*MAX_COL
#define MAX_NOM 15

void AfficheTableau(char *); /* <-- Affichage du tableau */
void InitialiserTableau(char *); /* <-- Initialisation du tableau */
void AjoutPionTableau (char *, char *, short); /* <-- Ajout d'un pion dans le tableau */
short LireChaine (char *); /* <-- Simple lirechaine */
void ModifChaine(char *); /* <-- Modifier les maj/min des noms encodés */
short VerificationJeu(char *); /* <-- Verification si il y a un alignement de 4 pions */

int main()
{
    char Tableau[MAX_LIGNES][MAX_COL], Joueur1[MAX_NOM], Ordinateur[11];
    short controle=1, CompteurPion, Resultat=0, ControleJeu=1;
    srand(time(NULL));

    printf("**************************************\n") ;
    printf("*                                    *\n") ;
    printf("* Bienvenue sur le jeu Puissance 4 ! *\n") ;
    printf("*                                    *\n") ;
    printf("**************************************\n\n\n") ;

    do
    {
        do
        {
            printf ("Veuillez entrer votre nom de joueur : ");
            memset (Joueur1, 0, sizeof (Joueur1));
            controle = LireChaine(&Joueur1[0]);
            ModifChaine(&Joueur1[0]);
        } while (controle == 0);

        strcpy(Ordinateur,"ORDINATEUR");
        system("cls");
        InitialiserTableau(&Tableau[0][0]);
        AfficheTableau(&Tableau[0][0]);
        CompteurPion = 0;
        Resultat = 0;
        do
        {
            AjoutPionTableau(&Joueur1[0], &Tableau[0][0], CompteurPion);
            system("cls");
            AfficheTableau(&Tableau[0][0]);
            CompteurPion++;
            if(CompteurPion >= 7)
            {
                Resultat = VerificationJeu(&Tableau[0][0]);
                if(Resultat==1)
                    printf("\n\nBien joue %s ! Vous remportez la partie!\n\n",Joueur1);
            }
            if(Resultat != 1)
            {
                AjoutPionTableau(&Ordinateur[0], &Tableau[0][0], CompteurPion);
                system("cls");
                AfficheTableau(&Tableau[0][0]);
                CompteurPion++;
                if(CompteurPion >= 8)
                {
                    Resultat = VerificationJeu(&Tableau[0][0]);
                    if(Resultat == 1)
                        printf("\n\nBien joue mais malheureusement %s a remporte la partie!\n\n",Ordinateur);
                }
            }
        } while(Resultat == 0);

        if(CompteurPion==MAX_PION)
            printf("\nMatch Nul!\n");

        printf("\nVoulez-vous rejouer la partie? (1 = oui ou 0 = non) : ");
        fflush(stdin);
        scanf("%hd",&ControleJeu);
    } while(ControleJeu == 1);
}

/* Process : Cette fonction permet d'ajouter un pion dans le tableau en fonction du joueur passé en paramètre */
/* Input : pNom : Pointeur sur le vecteur du joueur / pTab : adresse du tableau / pNomJoueur1/2 : pointeur sur les deux tableaux noms pour pouvoir comparer qui est entrain d'ajouter un pion  */
/* Output : void */
void AjoutPionTableau (char *pNom, char *pTab, short CompteurPion)
{
    short PionColonne=0,i,j, booleen, compteur;
    char *pTmp;
    pTmp = pTab;
    if (strcmp(pNom,"ORDINATEUR") != 0)
    {
        do
        {
            printf("C'est a %s de jouer! Choisissez une colonne! => ", pNom);
            scanf ("%hd", &PionColonne);
        } while (PionColonne <=0 || PionColonne > MAX_COL);

        /*Tant que la colonne choisie est pleine, le joueur
        doit en choisir une autre*/
        while((*(pTab + PionColonne -1) != ' '))
        {
            printf("La colonne est pleine!\n\n");
            do
            {
                printf("Choisissez une autre colonne! => ");
                scanf ("%hd", &PionColonne);
            } while (PionColonne <=0 || PionColonne > 7);
        }
        PionColonne--;
        for(i = MAX_LIGNES-1; i >= 0; i--)
        {
            if(*(pTab + (i*MAX_COL) + PionColonne) == ' ')
            {
                *(pTab + (i*MAX_COL) + PionColonne) = 'X';
                i=-1;
            }
        }
    }
    else
    {
        printf ("\nL'ordinateur choisit son emplacement..!\n\n");
        Sleep(2000);
        pTmp = pTab;
        if (CompteurPion == 1)
        {
            if (*(pTab+((MAX_LIGNES-1)*MAX_COL)+3) == ' ') /* Empeche la formation d'un alignement adverse sur la ligne du bas */
                PionColonne = 4;
            else
            {
                if (*(pTab+((MAX_LIGNES-1)*MAX_COL)+1) == ' ') /* Empeche la formation d'un alignement adverse sur la ligne du bas */
                    PionColonne = 2;
            }
        }
        else
        {
            if (CompteurPion == 3)
            {
                if (*(pTab+((MAX_LIGNES-1)*MAX_COL)+3) == 'O')
                {
                    PionColonne = rand() % 7 +1;
                }
                else
                {
                    if (*(pTab+((MAX_LIGNES-1)*MAX_COL)+5) == ' ') /* Empeche la formation d'un alignement adverse sur la ligne du bas */
                        PionColonne = 6;
                    else
                    {
                        PionColonne = 5; /* Empeche la formation d'un alignement adverse sur la ligne du bas */
                    }
                }
            }
            else
            {
                PionColonne = -1;
                for (compteur=0;compteur<2;compteur++) /* Parcours 2x le tableau pour analyser les pions adverses et les siens */
                {
                    for (i=MAX_LIGNES-1;i>=0;i--) /* Commence  la derniere ligne */
                    {
                        pTmp = pTab + (i*MAX_COL-1);
                        for (j=0;j<MAX_COL;j++,pTmp++) /* Commence à la premiere colonne */
                        {
                            if(compteur == 0) /* Compteur a 0 donc analyse des pions alliés */
                            {
                                if (*pTmp == 'O') /* Place un pion si la possibilité de terminer la partie est possible pour l'ordinateur */
                                {
                                    if (*(pTmp-MAX_COL) == 'O' && *(pTmp-(2*MAX_COL)) == 'O' && *(pTmp-(3*MAX_COL)) == ' ') /* Place un pion si 3 pions alliés sont déja verticales */
                                    {
                                        /* printf ("Alternative 1 attaque"); */
                                        PionColonne = j;
                                        i = 0;
                                        j = MAX_COL;
                                        compteur = 2;
                                    }
                                    else
                                    {
                                        if (j<=3 && *(pTmp+1) == 'O' && *(pTmp+2) == 'O' && *(pTmp+3) == ' ' && (*(pTmp+ MAX_COL + 3) != ' ' || i == MAX_LIGNES)) /* Place un pion si 3 pions alliés sont déja horizontales sur la barre */
                                        {
                                            /* printf ("Alternative 2 attaque"); */
                                            PionColonne = j+3;
                                            i = 0;
                                            j = MAX_COL;
                                            compteur = 2;
                                        }
                                        else
                                        {
                                            if (j <= 4 && *(pTmp-1) == ' ' && *(pTmp+1) == 'O' && *(pTmp+2) == 'O' && (*(pTmp+MAX_COL-1) != ' ' || i == MAX_LIGNES)) /* Place un pions si 3 pions alignés et case à gauche libre */
                                            {
                                                /* printf ("Alternative 2.1 attaque"); */
                                                PionColonne = j-1;
                                                i = 0;
                                                j = MAX_COL;
                                                compteur = 2;
                                            }
                                            else
                                            {
                                                if (j <= 4 && *(pTmp-MAX_COL+1) == 'O' && *(pTmp-(2*MAX_COL)+2) == 'O' && *(pTmp+MAX_COL-1) == ' ' && (*(pTmp+(2*MAX_COL) -1) != ' ' || i == MAX_LIGNES)) /* Attaque la diagonale droite */
                                                {
                                                    /* printf ("Alternative 3 attaque"); */
                                                    PionColonne = j-1;
                                                    i = 0;
                                                    j = MAX_COL;
                                                    compteur = 2;
                                                }
                                                else
                                                {
                                                    if (j >=2 && *(pTmp-MAX_COL-1) == 'O' && *(pTmp-(2*MAX_COL)-2) == 'O' && *(pTmp+MAX_COL+1) == ' ' && (*(pTmp+(2*MAX_COL) +1) != ' ' || i == MAX_LIGNES)) /* Attaque la diagonale gauche */
                                                    {
                                                        /* printf ("Alternative 4 attaque"); */
                                                        PionColonne = j+1;
                                                        i = 0;
                                                        j = MAX_COL;
                                                        compteur = 2;
                                                    }
                                                    else
                                                    {
                                                        if (j <= 5 && *(pTmp+MAX_COL-1)=='O' && *(pTmp-(2*MAX_COL)-2) == 'O' && *(pTmp-MAX_COL+1) == ' ' && *(pTmp+1) != ' ') /* Place un pion si 3 pions en diagonale droite avec un pion support à droite */
                                                        {
                                                            /* printf ("Alternative 5 attaque"); */
                                                            PionColonne = j+3;
                                                            i = 0;
                                                            j = MAX_COL;
                                                            compteur = 2;
                                                        }
                                                        else
                                                        {
                                                            if (j<=3 && *(pTmp-MAX_COL+1)=='O' && *(pTmp-(2*MAX_COL)+2) == ' ' && *(pTmp-(3*MAX_COL)+3) == 'O' && *(pTmp-MAX_COL+2) != ' ') /* Place un pion si il y a des pions en diagonale avec un trou */
                                                            {
                                                                /* printf ("Alternative 6 attaque"); */
                                                                PionColonne = j+2;
                                                                i = 0;
                                                                j = MAX_COL;
                                                                compteur = 2;
                                                            }
                                                            else
                                                            {
                                                                if (j>=3 && *(pTmp-1) != ' ' && *(pTmp-MAX_COL-1) == ' ' && *(pTmp-(2*MAX_COL)-2) == 'O' && *(pTmp-(3*MAX_COL)-3) == 'O') /* Place un pion si il y a des pions en diagonale avec un trou */
                                                                {
                                                                    /* printf ("Alternative 7 attaque"); */
                                                                    PionColonne = j-1;
                                                                    i = 0;
                                                                    j = MAX_COL;
                                                                    compteur = 2;
                                                                }
                                                                else
                                                                {
                                                                    if (j<=3 && *(pTmp+1) != ' ' && *(pTmp-MAX_COL+1) == ' ' && *(pTmp-(2*MAX_COL)+2) == 'O' && *(pTmp-(3*MAX_COL)+3) == 'O') /* Place un pion si il y a des pions en diagonale avec un trou */
                                                                    {
                                                                        /* printf ("Alternative 8 attaque"); */
                                                                        PionColonne = j+1;
                                                                        i = 0;
                                                                        j = MAX_COL;
                                                                        compteur = 2;
                                                                    }
                                                                    else
                                                                    {
                                                                        if (j>=3 && *(pTmp-MAX_COL-1) == 'O' && *(pTmp-(2*MAX_COL)-2) == 'O' && *(pTmp-(3*MAX_COL)-3) == ' ' && *(pTmp-(2*MAX_COL)-3) != ' ') /* Place un pion si il y a des pions en diagonale avec un trou */
                                                                        {
                                                                            /* printf ("Alternative 9 attaque"); */
                                                                            PionColonne = j-3;
                                                                            i = 0;
                                                                            j = MAX_COL;
                                                                            compteur = 2;
                                                                        }
                                                                        else
                                                                        {
                                                                            if (j>=3 && *(pTmp-MAX_COL-1) == 'O' && *(pTmp-(2*MAX_COL)-2) == ' ' && *(pTmp-(3*MAX_COL)-3) == 'O' && *(pTmp-MAX_COL-2) != ' ') /* Place un pion si il y a des pions en diagonale avec un trou */
                                                                            {
                                                                                /* printf ("Alternative 10 attaque"); */
                                                                                PionColonne = j-2;
                                                                                i = 0;
                                                                                j = MAX_COL;
                                                                                compteur = 2;
                                                                            }
                                                                            else
                                                                            {
                                                                                if (j<=3 && *(pTmp+1) == 'O' && *(pTmp+2) == ' ' && *(pTmp+3) == 'O' && *(pTmp+MAX_COL+2) != ' ') /* Place un pion si 3 pions alignés avec trou au milieu */
                                                                                {
                                                                                    /* printf ("Alternative 11 attaque"); */
                                                                                    PionColonne = j+2;
                                                                                    i = 0;
                                                                                    j = MAX_COL;
                                                                                    compteur = 2;
                                                                                }
                                                                                else
                                                                                {
                                                                                    if (j<=3 && *(pTmp+1) == ' ' && *(pTmp+2) == 'O' && *(pTmp+3) == 'O' && *(pTmp+MAX_COL+1) != ' ') /* Place un pion si 3 pions alignés avec trou au milieu */
                                                                                    {
                                                                                        /* printf ("Alternative 12 attaque"); */
                                                                                        PionColonne = j+1;
                                                                                        i = 0;
                                                                                        j = MAX_COL;
                                                                                        compteur = 2;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        if (j>=2 && *(pTmp+MAX_COL+1)==' ' && *(pTmp-MAX_COL-1)=='O' && *(pTmp-(2*MAX_COL)-2) == 'O' && *(pTmp + MAX_COL + 1 ) != ' ') /* Diagonale avec trou */
                                                                                        {
                                                                                            /* printf ("Alternative 13 attaque"); */
                                                                                            PionColonne = j+1;
                                                                                            i = 0;
                                                                                            j = MAX_COL;
                                                                                            compteur = 2;
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            if (j >=2 && *(pTmp+MAX_COL-1)==' ' && *(pTmp-MAX_COL+1)=='O' && *(pTmp-(2*MAX_COL)+2) == 'O') /* Diagonale avec trou */
                                                                                            {
                                                                                                /* printf ("Alternative 14 attaque"); */
                                                                                                PionColonne = j-1;
                                                                                                i = 0;
                                                                                                j = MAX_COL;
                                                                                                compteur = 2;
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                if (j >=3 && j<=5 && *(pTmp+MAX_COL-1)==' ' && *(pTmp-MAX_COL+1)=='O' && *(pTmp-(2*MAX_COL)+2) == 'O' && *(pTmp+(2*MAX_COL)+2) != ' ') /* Diagonale avec toru */
                                                                                                {
                                                                                                    /* printf ("Alternative 15 attaque"); */
                                                                                                    PionColonne = j-1;
                                                                                                    i = 0;
                                                                                                    j = MAX_COL;
                                                                                                    compteur = 2;
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    if (j >=3 && j<=5 && *(pTmp+MAX_COL+1)==' ' && *(pTmp-MAX_COL-1)=='O' && *(pTmp-(2*MAX_COL)-2) == 'O' && *(pTmp+(2*MAX_COL)+2) != ' ') /* Diagonale avec trou */
                                                                                                    {
                                                                                                        /* printf ("Alternative 16 attaque"); */
                                                                                                        PionColonne = j+1;
                                                                                                        i = 0;
                                                                                                        j = MAX_COL;
                                                                                                        compteur = 2;
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (*pTmp == 'X')
                                {
                                    if (*(pTmp-MAX_COL) == 'X' && *(pTmp-(2*MAX_COL)) == 'X' && *(pTmp-(3*MAX_COL)) == ' ')
                                    {
                                        /* printf ("Alternative 1 defense"); */
                                        PionColonne = j;
                                        i = 0;
                                        j = MAX_COL;
                                        compteur = 2;
                                    }
                                    else
                                    {
                                        if (j<=3 && *(pTmp+1) == 'X' && *(pTmp+2) == 'X' && *(pTmp+3) == ' ' && *(pTmp+ MAX_COL + 3) != ' ')
                                        {
                                            /* printf ("Alternative 2 defense"); */
                                            PionColonne = j+3;
                                            i = 0;
                                            j = MAX_COL;
                                            compteur = 2;
                                        }
                                        else
                                        {
                                            if (j >1 && j <= 4 && *(pTmp-1) == ' ' && *(pTmp+1) == 'X' && *(pTmp+2) == 'X' && *(pTmp+MAX_COL-1) != ' ')
                                            {
                                                /* printf ("Alternative 2.1 defense"); */
                                                PionColonne = j-1;
                                                i = 0;
                                                j = MAX_COL;
                                                compteur = 2;
                                            }
                                            else
                                            {
                                                if (j <= 4 && *(pTmp-MAX_COL+1) == 'X' && *(pTmp-(2*MAX_COL)+2) == 'X' && *(pTmp+MAX_COL-1) == ' ' && *(pTmp+(2*MAX_COL) -1) != ' ')
                                                {
                                                    /* printf ("Alternative 3 defense"); */
                                                    PionColonne = j-1;
                                                    i = 0;
                                                    j = MAX_COL;
                                                    compteur = 2;
                                                }
                                                else
                                                {
                                                    if (j >=2 && *(pTmp-MAX_COL-1) == 'X' && *(pTmp-(2*MAX_COL)-2) == 'X' && *(pTmp+MAX_COL+1) == ' ' && *(pTmp+(2*MAX_COL) +1) != ' ')
                                                    {
                                                        /* printf ("Alternative 4 defense"); */
                                                        PionColonne = j+1;
                                                        i = 0;
                                                        j = MAX_COL;
                                                        compteur = 2;
                                                    }
                                                    else
                                                    {
                                                        if (j <= 5 && *(pTmp+MAX_COL-1)=='X' && *(pTmp-(2*MAX_COL)-2) == 'X' && *(pTmp-MAX_COL+1) == ' ' && *(pTmp+1) != ' ')
                                                        {
                                                            /* printf ("Alternative 5 defense"); */
                                                            PionColonne = j+3;
                                                            i = 0;
                                                            j = MAX_COL;
                                                            compteur = 2;
                                                        }
                                                        else
                                                        {
                                                            if (j<=3 && *(pTmp-MAX_COL+1)=='X' && *(pTmp-(2*MAX_COL)+2) == ' ' && *(pTmp-(3*MAX_COL)+3) == 'X' && *(pTmp-MAX_COL+2) != ' ')
                                                            {
                                                                /* printf ("Alternative 6 defense"); */
                                                                PionColonne = j+2;
                                                                i = 0;
                                                                j = MAX_COL;
                                                                compteur = 2;
                                                            }
                                                            else
                                                            {
                                                                if (j>=3 && *(pTmp-1) != ' ' && *(pTmp-MAX_COL-1) == ' ' && *(pTmp-(2*MAX_COL)-2) == 'X' && *(pTmp-(3*MAX_COL)-3) == 'X')
                                                                {
                                                                    /* printf ("Alternative 7 defense"); */
                                                                    PionColonne = j-1;
                                                                    i = 0;
                                                                    j = MAX_COL;
                                                                    compteur = 2;
                                                                }
                                                                else
                                                                {
                                                                    if (j<=3 && *(pTmp+1) != ' ' && *(pTmp-MAX_COL+1) == ' ' && *(pTmp-(2*MAX_COL)+2) == 'X' && *(pTmp-(3*MAX_COL)+3) == 'X')
                                                                    {
                                                                        /* printf ("Alternative 8 defense"); */
                                                                        PionColonne = j+1;
                                                                        i = 0;
                                                                        j = MAX_COL;
                                                                        compteur = 2;
                                                                    }
                                                                    else
                                                                    {
                                                                        if (j>=3 && *(pTmp-MAX_COL-1) == ' ' && *(pTmp-(2*MAX_COL)-2) == 'X' && *(pTmp-(3*MAX_COL)-3) == 'X' && *(pTmp-(2*MAX_COL)-3) != ' ')
                                                                        {
                                                                            /* printf ("Alternative 9 defense"); */
                                                                            PionColonne = j-3;
                                                                            i = 0;
                                                                            j = MAX_COL;
                                                                            compteur = 2;
                                                                        }
                                                                        else
                                                                        {
                                                                            if (j>=3 && *(pTmp-MAX_COL-1) == 'X' && *(pTmp-(2*MAX_COL)-2) == ' ' && *(pTmp-(3*MAX_COL)-3) == 'X' && *(pTmp-MAX_COL-2) != ' ')
                                                                            {
                                                                                /* printf ("Alternative 10 defense"); */
                                                                                PionColonne = j-2;
                                                                                i = 0;
                                                                                j = MAX_COL;
                                                                                compteur = 2;
                                                                            }
                                                                            else
                                                                            {
                                                                                if (j<=3 && *(pTmp+1) == 'X' && *(pTmp+2) == ' ' && *(pTmp+3) == 'X' && *(pTmp+MAX_COL+2) != ' ')
                                                                                {
                                                                                    /* printf ("Alternative 11 defense");*/
                                                                                    PionColonne = j+2;
                                                                                    i = 0;
                                                                                    j = MAX_COL;
                                                                                    compteur = 2;
                                                                                }
                                                                                else
                                                                                {
                                                                                    if (j<=3 && *(pTmp+1) == ' ' && *(pTmp+2) == 'X' && *(pTmp+3) == 'X' && *(pTmp+MAX_COL+1) != ' ')
                                                                                    {
                                                                                        /* printf ("Alternative 12 defense"); */
                                                                                        PionColonne = j+1;
                                                                                        i = 0;
                                                                                        j = MAX_COL;
                                                                                        compteur = 2;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        if (j >=2 && *(pTmp+MAX_COL+1)==' ' && *(pTmp-MAX_COL-1)=='X' && *(pTmp-(2*MAX_COL)-2) == 'X')
                                                                                        {
                                                                                            /* printf ("Alternative 13 defense"); */
                                                                                            PionColonne = j+1;
                                                                                            i = 0;
                                                                                            j = MAX_COL;
                                                                                            compteur = 2;
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            if (j >=2 && *(pTmp+MAX_COL-1)==' ' && *(pTmp-MAX_COL+1)=='X' && *(pTmp-(2*MAX_COL)+2) == 'X')
                                                                                            {
                                                                                                /* printf ("Alternative 14 defense"); */
                                                                                                PionColonne = j-1;
                                                                                                i = 0;
                                                                                                j = MAX_COL;
                                                                                                compteur = 2;
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                if (j >=3 && j<=5 && *(pTmp+MAX_COL-1)==' ' && *(pTmp-MAX_COL+1)=='X' && *(pTmp-(2*MAX_COL)+2) == 'X' && *(pTmp+(2*MAX_COL)+2) != ' ')
                                                                                                {
                                                                                                    /* printf ("Alternative 15 defense"); */
                                                                                                    PionColonne = j-1;
                                                                                                    i = 0;
                                                                                                    j = MAX_COL;
                                                                                                    compteur = 2;
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    if (j >=3 && j<=5 && *(pTmp+MAX_COL+1)==' ' && *(pTmp-MAX_COL-1)=='X' && *(pTmp-(2*MAX_COL)-2) == 'X' && *(pTmp+(2*MAX_COL)-2) != ' ')
                                                                                                    {
                                                                                                        /* printf ("Alternative 16 defense"); */
                                                                                                        PionColonne = j-1;
                                                                                                        i = 0;
                                                                                                        j = MAX_COL;
                                                                                                        compteur = 2;
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (PionColonne == -1) /* Si il n'y a aucune façon d'attaquer ou de contrer une attaque adverse */
        {
            for (compteur=0;compteur<2;compteur++)
            {
                for (i=MAX_LIGNES-1;i>=0;i--)
                {
                    pTmp = pTab + (i*MAX_COL-1);
                    for (j=0;j<MAX_COL;j++,pTmp++)
                    {
                        if (compteur == 0)
                        {
                            if (*pTmp == 'O')
                            {
                                if (*(pTmp-MAX_COL) == 'O' && *(pTmp-(2*MAX_COL)) == ' ') /*Si déja 2 pions verticaux */
                                {
                                    PionColonne = j;
                                    i = 0;
                                    j = MAX_COL;
                                    compteur = 2;
                                }
                                else
                                {
                                    if (*(pTmp+1) == 'O' && *(pTmp+2) == ' ') /* Si déja 2 pions horizontaux */
                                    {
                                        PionColonne = j+2;
                                        i = 0;
                                        j = MAX_COL;
                                        compteur = 2;
                                    }
                                    else
                                    {
                                        if (j >= 3 && *(pTmp-MAX_COL-1) == 'O' && *(pTmp-(2*MAX_COL)-2) == ' ' && *(pTmp-MAX_COL-2) != ' ') /* Si déja 2 pions en diagonale gauche */
                                        {
                                            PionColonne = j-2;
                                            i = 0;
                                            j = MAX_COL;
                                            compteur = 2;
                                        }
                                        else
                                        {
                                            if (j <= 3 && *(pTmp-MAX_COL+1) == 'O' && *(pTmp-(2*MAX_COL)+2) == ' ' && *(pTmp-MAX_COL+2) != ' ') /* Si déja 2 pions en diagonale droite */
                                            {
                                                PionColonne = j+2;
                                                i = 0;
                                                j = MAX_COL;
                                                compteur = 2;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (j<=3 && *pTmp == 'O' && *(pTmp+1) == ' ' && *(pTmp+2) != 'X')
                            {
                                PionColonne = j+1;
                                i = 0;
                                j = MAX_COL;
                                compteur = 2;
                            }
                            else
                            {
                                if (i>=3 && *pTmp == 'O' && *(pTmp-MAX_COL) == ' ')
                                {
                                    PionColonne = j;
                                    i = 0;
                                    j = MAX_COL;
                                    compteur = 2;
                                }
                                else
                                {
                                    if (j>=3 && *pTmp == 'O' && *(pTmp-1) == ' ' && *(pTmp-2) != 'X')
                                    {
                                        PionColonne = j-1;
                                        i = 0;
                                        j = MAX_COL;
                                        compteur = 2;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (PionColonne == -1) /* Si il n'y vraiment aucune manière de trouver un alignement de pions */
        {
            PionColonne = rand() % 7+1;
            i = 0;
            j = MAX_COL;
            compteur = 2;
        }
        PionColonne--;
        for(i = MAX_LIGNES; i >= 0; i--)
        {
            if(*(pTab + (i*MAX_COL) + PionColonne) == ' ')
            {
                *(pTab + (i*MAX_COL) + PionColonne) = 'O';
                i=0;
            }
        }
    }
}

/* Process : Cette fonction permet de vérifier si il y a un alignement de 4 pions de même couleur (ici : X ou O) */
/* Input : pTab : adresse du début du tableau */
/* Output : Renvoie une variable short qui indique si il y a un alignement ou pas */
short VerificationJeu(char *pTab)
{
    short ControleJeu = 0, i,j;

    for(i = 0; i< MAX_LIGNES; i++)
    {
        for(j = 0; j< MAX_COL; j++, pTab++)
        {
            if(i<=2) /* On vérifie ici si il y a un alignement verticale des pions */
                if((*pTab == 'X' && *(pTab + MAX_COL) == 'X' && *(pTab + (2*MAX_COL)) == 'X' && *(pTab + (3*MAX_COL)) == 'X') || (*pTab == 'O' && *(pTab + MAX_COL) == 'O' && *(pTab + (2*MAX_COL)) == 'O' && *(pTab + (3*MAX_COL)) == 'O'))
                    ControleJeu = 1;
            if(j<=3) /* On vérifie ici si il y a un alignement horizontale des pions */
                if((*pTab == 'X' && *(pTab + 1) == 'X' && *(pTab + 2) == 'X' && *(pTab + 3) == 'X') || (*pTab == 'O' && *(pTab + 1) == 'O' && *(pTab + 2) == 'O' && *(pTab + 3) == 'O'))
                    ControleJeu = 1;
            if((j<=3) && (i<=2)) /* On vérifie ici si il y a un alignement en diagonale droit des pions */
                if((*pTab == 'X' && *(pTab + MAX_COL + 1) == 'X' && *(pTab + (2*MAX_COL) + 2) == 'X' && *(pTab + (3*MAX_COL) + 3 ) == 'X') || (*pTab == 'O' && *(pTab + MAX_COL + 1) == 'O' && *(pTab + (2*MAX_COL) + 2) == 'O' && *(pTab + (3*MAX_COL) + 3 ) == 'O'))
                    ControleJeu = 1;
            if((j>=3) && (i<=2)) /* On vérifie ici si il y a un alignement diagonale gauche des pions */
                if((*pTab == 'X' && *(pTab + MAX_COL - 1) == 'X' && *(pTab + (2* MAX_COL) - 2) == 'X' && *(pTab + (3* MAX_COL) - 3 ) == 'X') || (*pTab == 'O' && *(pTab + MAX_COL - 1) == 'O' && *(pTab + (2* MAX_COL) - 2) == 'O' && *(pTab + (3* MAX_COL) - 3 ) == 'O'))
                    ControleJeu = 1;
        }
    }
    return ControleJeu;
}

/* Process : Cette fonction permet d'initialiser le tableau avec des espaces */
/* Input : pTab : adresse du début du tableau */
/* Output : void */
void InitialiserTableau(char *pTab)
{
    short i, j;

    for (i=0;i<MAX_LIGNES;i++)
    {
        for (j=0;j<MAX_COL;j++,pTab++)
            *pTab = ' ';
    }
}

/* Process : Cette fonction permet d'afficher le tableau */
/* Input : pTab : adresse du début du tableau*/
/* Output : void */
void AfficheTableau(char *pTab)
{
    short i,j;

    printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
    printf("|---|---|---|---|---|---|---|\n");
    for(i=0; i<MAX_LIGNES; i++)
    {
        printf("|");
        for(j=0; j<MAX_COL; j++, pTab++)
            printf(" %c |", *pTab);

        printf("\n");
        printf("|---|---|---|---|---|---|---|\n");
    }
    printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n\n\n");
}

/* Process : Cette fonction permet de lire une chaine de caractères */
/* Input : pc : permet de d'ecrire une chaine de caractères à l'aide d'un pointeur */
/* Output : Renvoie une variable short qui controle si les caracteres entrés sont correctes */
short LireChaine (char *pc)
{
    char c ;
    int i=0,controle=1 ;

    fflush(stdin) ;
    c=getchar() ;
    while(c != '\n' || i < 2)
    {
        if(((c>='a' && c<='z') || (c>='A' && c<='Z')) || c=='-' || c==' ')
        {
            *pc=c ;
            pc++ ;
            i++ ;
            c=getchar() ;
            if (i > MAX_NOM-1)
                controle = 0;
        }
        else
        {
            controle = 0;
            i=2;
            c = '\n';
        }
    }
    *pc='\0' ;

    return controle ;
}

/* Process : le pointeur du début d'une ligne du tableau, MAX_COL */
/* Input : modifie les majuscules et les minuscules des prenoms */
/* Output : void */
void ModifChaine(char *pc)
{
    int i=0,verif;

    fflush (stdin);

    while ((i< MAX_COL-1))
    {
        fflush (stdin);
        if (i==0)
            *pc=toupper(*pc);
        else
        {
            if (*pc == ' ' || *pc == '-')
            {
                pc++;
                *pc = toupper(*pc);

            }
            else
                *pc=tolower(*pc);
        }
        pc++ ;
        i++ ;
    }
}
