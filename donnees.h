#pragma once

#include <libsx.h> // On aura ici besoin de cette librairie afin de créer des éléments de type Widget dans la structure

// La taille des cellules et les dimensions de la grille restant fixes pendant l'exécution du jeu, on peut les définir avec des #define
#ifndef TAILLE
#define TAILLE 20
#define HAUTEUR 30
#define LARGEUR 30
#endif

// On crée ces trois types afin de simplifier la compréhension du jeu
typedef enum { MORT, VIVANT } ETAT ;
typedef enum { STOP, START } RUN ;
typedef enum { CLASSIQUE, DAYANDNIGHT } MODE ;

// Cette strucuture contient tous les éléments dynamiques du jeu, qui nécessiteront d'être modifiés en permanence
typedef struct {
	ETAT grille[HAUTEUR][LARGEUR];
	RUN run;
	MODE mode;
	int time;
	Widget labelTime;
	int tours;
	Widget labelTours;
	Widget labelVivants;
	Widget boutonClassique;
	Widget boutonDayAndNight;
} data;

void initJeu(data *);

void fillTabRand(ETAT tab[HAUTEUR][LARGEUR]);

void afficheTab(const ETAT tab[HAUTEUR][LARGEUR]);

void copieTab(const ETAT a[HAUTEUR][LARGEUR], ETAT b[HAUTEUR][LARGEUR]);

int nbVoisins(const int, const int, const ETAT tab[HAUTEUR][LARGEUR]);

int nbVivants(const ETAT tab[HAUTEUR][LARGEUR]);

int compTab(const ETAT a[HAUTEUR][LARGEUR], const ETAT b[HAUTEUR][LARGEUR]);

int fillMotif(char *, ETAT tab[HAUTEUR][LARGEUR]);

void tourSuivant(data *);
