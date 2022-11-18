#include <stdio.h>
#include <stdlib.h>
#include "donnees.h"

/*
 * Rôle : initialise les données de fonctionnement du jeu
 */
void initJeu(data *d) {
	fillTabRand(d->grille); // D'abord on initialise la grille de manière aléatoire
	d->run=STOP; // Afin que le jeu ne se lance pas dès qu'on l'exécute
	d->mode=CLASSIQUE; // Afin d'être en mode classique par défaut
	d->time=1000; // Temps par défaut entre deux tours. Arbitraire mais peut ensuite être modifié via l'interface
	d->tours=0; // Evidemment, lorsque le jeu vient d'être lancé, il n'a pas encore fonctionné donc le nombre de tours est nul
}

/*
 * Rôle : remplit un tableau de manière aléatoire avec des 0 et des 1 (mort et vivant)
 */
void fillTabRand(ETAT tab[HAUTEUR][LARGEUR]) {
    static int x,y;
    
    for(x=0;x<HAUTEUR;x++) {
        for(y=0;y<LARGEUR;y++)
            tab[x][y]=rand()&1;
	}
}

/*
 * Rôle : affiche un tableau dans le terminal
 */
void afficheTab(const ETAT tab[HAUTEUR][LARGEUR]) {
    static int x,y;
    
    for(x=0;x<HAUTEUR;x++) {
        for(y=0;y<LARGEUR;y++)
            printf("%2d", tab[x][y]);
        printf("\n");
    }
}

/*
 * Rôle : copie le tableau a dans le tableau b
 * Antécédent : tableau a
 * Conséquent : tableau b rempli avec les valeurs du tableau a
 */
void copieTab(const ETAT a[HAUTEUR][LARGEUR], ETAT b[HAUTEUR][LARGEUR]) {
    static int x,y;
    
    for(x=0;x<HAUTEUR;x++) {
        for(y=0;y<LARGEUR;y++)
            b[x][y]=a[x][y];
	}
}

/*
 * Rôle : renvoie le nombre de voisins (cellules vivantes) de la cellule de coordonnées i,j dans un tableau
 * Antécédent : tableau tab et coordonnées i,j
 * Conséquent : nombre de voisins de la cellule i,j
 */
int nbVoisins(const int i, const int j, const ETAT tab[HAUTEUR][LARGEUR]) {
    static int x,y;
    int s=0;
    
    for(x=i-1;x<=i+1;x++) { // Il faut parcourir les cases directement voisines de i,j donc on va de la i-1,j-1 à la i+1,j+1
        for(y=j-1;y<=j+1;y++)
            if(x>=0 && y>=0 && x<HAUTEUR && y<LARGEUR && tab[x][y]==VIVANT && !(x==i && y==j)) // Il faut biensur tester que la case sur laquelle on se trouve ne se situe pas en dehors de la grille
				s++;																   		   // et qu'il ne s'agit pas de la case i,j car évidemment, il ne faut pas compter la case elle-même en
	}																						   // tant que voisin
														
    return s;
}

/*
 * Rôle : renvoie le nombre de cellules vivantes dans un tableau
 * Antécédent : tableau tab
 * Conséquent : nombre de cellules vivantes dans le tableau
 */
int nbVivants(const ETAT tab[HAUTEUR][LARGEUR]) {
	static int x,y;	
	int s=0;
	
    for(x=0;x<HAUTEUR;x++) {
        for(y=0;y<LARGEUR;y++)
            if(tab[x][y]==VIVANT)
				s++;
	}
	
	return s;
}

/*
 * Rôle : compare deux tableaux et renvoie 1 si les deux sont égaux, 0 sinon
 * Antécédent : tableaux a et b
 * Conséquent : 1 si les tableaux sont égaux, 0 sinon
 */
int compTab(const ETAT a[HAUTEUR][LARGEUR], const ETAT b[HAUTEUR][LARGEUR]) {
	static int x,y;
	
    for(x=0;x<HAUTEUR;x++) {
        for(y=0;y<LARGEUR;y++)
            if(a[x][y] != b[x][y])
				return 0;
	}
	
	return 1;
}

/*
 * Rôle : remplit un tableau à partir d'un fichier dont le nom est donné en paramètre. Cette fonction est de type int afin de renvoyer 1 si le fichier a bien été trouvé, 0 sinon
 * Antécédent : nom de fichier et tableau tab
 * Conséquent : tableau tab rempli avec les valeurs se trouvant dans le fichier
 */
int fillMotif(char *nom, ETAT tab[HAUTEUR][LARGEUR]) {
	static int x,y;
	FILE *fic;
	
	if((fic=fopen(nom, "r"))==NULL) {
		fprintf(stderr, "Fichier introuvable\n");
		return 0;
	}
	
	else {
		for(x=0;x<HAUTEUR;x++) {
			for(y=0;y<LARGEUR;y++) {
				tab[x][y]=fgetc(fic)-'0'; // On soustrait le code ASCII de la valeur 0 afin de bien récupérer la valeur décimale des chiffres et non leur code ASCII.
				fgetc(fic); // On fait un fgetc() "dans le vide" après chaque caractère afin de ne pas prendre en compte les espaces et les caractères de fin de ligne (\n)
			}
		}

		fclose(fic);
		return 1;
	}
}

/*
 * Rôle : calcule le nouvel état de la grille à partir des règles du jeu de la vie (classique ou Day and Night)
 */
void tourSuivant(data *d) {
    static int x,y,s;
    static ETAT copieGrille[HAUTEUR][LARGEUR];
    
    copieTab(d->grille,copieGrille); // On fait une sauvegarde de la grille actuelle car au fur et à mesure qu'elle sera modifiée, on ne pourra plus s'y fier pour calculer les nouvelles cellules

    for(x=0;x<HAUTEUR;x++) {
        for(y=0;y<LARGEUR;y++) {
            s=nbVoisins(x, y, copieGrille);
            if(d->mode==CLASSIQUE) // Si l'on est en mode classique, on applique les règles classiques
				d->grille[x][y]=(s==3 || (copieGrille[x][y]==VIVANT && s==2)) ? 1 : 0;
				
			else // Sinon, cela veut dire que l'on est en mode Day and Night et on applique les règles correspondantes
				d->grille[x][y]=(s==3 || s==6 || s==7 || s==8 || (copieGrille[x][y]==VIVANT && s==4)) ? 1 : 0;
        }
    }
    
    if(compTab(d->grille, copieGrille)) // A la fin, on compare la grille au tour n et au tour n+1, si elles sont identiques,
		d->run=STOP;					// cela signifie que l'on est dans une situation bloquée et on met le jeu en pause.
										// Cela permet notamment de voir combien de tours met une configuration pour se bloquer.
}
