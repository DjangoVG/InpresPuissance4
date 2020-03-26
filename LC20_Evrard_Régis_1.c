#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*-----------------------------------------------------------------------------------*/
/* Auteur : Régis Evrard                                                             */
/* Groupe : 2201                                                                     */
/* Application : LC20_Evrard_Régis_1.c                                                 */
/* Date de la dernière maj de l'exercice : 06/03/2019                                */
/*-----------------------------------------------------------------------------------*/

#define MAX_LIGNES 6
#define MAX_COL 7
#define MAX_NOM 15
#define MAX_PION MAX_LIGNES*MAX_COL

void AfficheTableau(char *); /* <-- Affichage du tableau */
void InitialiserTableau(char *); /* <-- Initialisation du tableau */
void AjoutPionTableau (char *, char *, char *, char *); /* <-- Ajout d'un pion dans le tableau */
short LireChaine (char *); /* <-- Simple lirechaine */
void ModifChaine(char *); /* <-- Modifier les maj/min des noms encodés */
short VerificationJeu(char *); /* <-- Verification si il y a un alignement de 4 pions */

int main()
{
    char Tableau[MAX_LIGNES][MAX_COL], Joueur1[MAX_NOM], Joueur2[MAX_NOM];
    short controle=1, CompteurPion, Resultat=0, ControleJeu=1;

    printf("**************************************\n") ;
    printf("*                                    *\n") ;
    printf("* Bienvenue sur le jeu Puissance 4 ! *\n") ;
    printf("*                                    *\n") ;
    printf("**************************************\n\n") ;

    do
    {
        do
        {
            printf ("Veuillez entrer le nom du joueur 1 : ");
            memset (Joueur1, 0, sizeof (Joueur1));
            controle = LireChaine(&Joueur1[0]);
            ModifChaine(&Joueur1[0]);
            do
            {
                printf ("\nVeuillez entrer le nom du joueur 2 (Non identique au Joueur1) : ");
                memset (Joueur2, 0, sizeof (Joueur2));
                controle = LireChaine(&Joueur2[0]);
                ModifChaine(&Joueur2[0]);
            } while (strcmp(Joueur1,Joueur2) == 0);
        } while (controle == 0);

        system("cls");
        InitialiserTableau(&Tableau[0][0]);
        AfficheTableau(&Tableau[0][0]);
        CompteurPion = 0;
        Resultat = 0;
        do
        {
            AjoutPionTableau(&Joueur1[0], &Tableau[0][0], &Joueur1[0], &Joueur2[0]);
            system("cls");
            AfficheTableau(&Tableau[0][0]);
            CompteurPion++;
            if(CompteurPion >= 7)
            {
                Resultat = VerificationJeu(&Tableau[0][0]);
                if(Resultat==1)
                    printf("\n\nBien joue %s ! Vous remportez la partie!\n\n",Joueur1);
            }
            if(Resultat == 0)
            {
                AjoutPionTableau(&Joueur2[0], &Tableau[0][0], &Joueur1[0], &Joueur2[0]);
                system("cls");
                AfficheTableau(&Tableau[0][0]);
                CompteurPion++;
                if(CompteurPion >= 8)
                {
                    Resultat = VerificationJeu(&Tableau[0][0]);
                    if(Resultat == 1)
                        printf("\n\nBien joue %s ! Vous remportez la partie!\n\n",Joueur2);
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
void AjoutPionTableau (char *pNom, char *pTab, char *pNomJoueur1, char *pNomJoueur2)
{
    int PionColonne=0,i;

    do /*Le joueur choisi la colonne*/
    {
        printf("C'est a %s de jouer! Choisissez une colonne! => ", pNom);
        scanf ("%hd", &PionColonne);
    } while (PionColonne <=0 || PionColonne > 7);

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
    PionColonne--; /* On décrémente PionColonne pour pouvoir anayser les cases en commencant à la case [0][0] */
    for(i = MAX_LIGNES; i >= 0; i--)
    {
        if(*(pTab + (i*MAX_COL) + PionColonne) == ' ')
        {
            if(strcmp(pNom,pNomJoueur1) == 0)
            {
                *(pTab + (i*MAX_COL) + PionColonne) = 'X';
                i=-1;
            }
            else
            {
                *(pTab + (i*MAX_COL) + PionColonne) = 'O';
                i=-1;
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
            if((j>=3) && (i<=2)) /* On vérifie ici si il y a un alignement verticale gauche des pions */
                if((*pTab == 'X' && *(pTab + MAX_COL - 1) == 'X' && *(pTab + (2* MAX_COL) - 2) == 'X' && *(pTab + (3* MAX_COL) - 3 ) == 'X') || (*pTab == 'O' && *(pTab + MAX_COL - 1) == 'O' && *(pTab + (2* MAX_COL) - 2) == 'O' && *(pTab + (3* MAX_COL) - 3 ) == 'O'))
                    ControleJeu = 1;
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
        for (j=0;j<MAX_COL;j++,pTab++)
            *pTab = ' ';
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
