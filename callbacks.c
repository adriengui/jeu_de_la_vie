#include <stdio.h>
#include <stdlib.h>
#include <libsx.h>
#include "donnees.h"
#include "vue.h"
#include "callbacks.h"

/*
 * Rôle : dessine une grille vide
 */
void drawGrille(Widget w, int width, int height, void *d) {
	static int x,y;
	
	for(x=0;x<HAUTEUR;x++) {
		for(y=0;y<LARGEUR;y++)
			DrawBox(y*TAILLE, x*TAILLE, TAILLE, TAILLE);
	}
}

/*
 * Rôle : ferme le jeu
 */
void quit(Widget w, void *d) {
	exit(EXIT_SUCCESS);
}

/*
 * Rôle : lance le jeu en mode automatique
 */
void start(Widget w, void *d) {
	data *s = (data *)d;
	
	s->run=START; // Il n'y a qu'à passer cette variable à l'état START, puisque la fonction qui fait l'animation s'auto-appelle en permanence
}

/*
 * Rôle : met en pause le jeu
 */
void stop(Widget w, void *d) {
	data *s = (data *)d;
	
	s->run=STOP; // Même chose pour mettre en pause
}

/*
 * Rôle : effectue un seul tour comme son nom l'indique
 */
void PasAPas(Widget w, void *d) {
	data *s = (data *)d;
	
	fillGrille(s);
}

/*
 * Rôle : remplit la grille de manière aléatoire
 */
void initGrilleRand(Widget w, void *d) {
	data *s = (data *)d;
	
	s->tours=0; // On réinitialise le nombre de tours
	fillTabRand(s->grille); // On remplit la grille de manière aléatoire
	if(s->run==STOP)
		fillGrille(s); // Et si le jeu n'est pas en train de tourner, on affiche la grille. Dans le cas contraire, cela se fera au tour suivant
					   // et on évite ainsi des problèmes de synchronisation
}

/*
 * Rôle : initialise la grille avec le motif des vaisseaux
 */
void vaisseaux(Widget w, void *d) {
	data *s = (data *)d;
	
	if(fillMotif("vaisseaux", s->grille)) { // Si la fonction qui lit les fichiers renvoie 0, cela signifie que le fichier n'a pas été trouvé et on ne fait rien.
		s->tours=0;							// Sinon, on réinitialise le compteur de tours et on remplit la grille sur le même principe que la fonction précédente.
		if(s->run==STOP)
			fillGrille(s);
	}
}

/*
 * Rôle : initialise la grille avec le motif de l'horloge
 */
void horloge(Widget w, void *d) {
	data *s = (data *)d;
	
	if(fillMotif("horloge", s->grille)) { // Idem que pour les vaisseaux
		s->tours=0;
		if(s->run==STOP)
			fillGrille(s);
	}
}

/*
 * Rôle : initialise la grille avec le motif du pentadecathlon
 */
void pentadecathlon(Widget w, void *d) {
	data *s = (data *)d;
	
	if(fillMotif("pentadecathlon", s->grille)) { // Idem que pour les deux fonctions précédentes
		s->tours=0;
		if(s->run==STOP)
			fillGrille(s);
	}
}

/*
 * Rôle : passe le jeu en mode Day and Night et actualise la couleur des boutons de sélection
 */
void DayAndNight(Widget w, void *d) {
	data *s = (data *)d;
	
	s->mode=DAYANDNIGHT;
	SetBgColor(w, GetRGBColor(200, 200, 200)); // Le bouton sur lequel on a cliqué est facilement récupérable par le paramètre w.
	SetBgColor(s->boutonClassique, WHITE);	   // En revanche pour l'autre, on le récupère via la structure.
}

/*
 * Rôle : passe le jeu en mode Classique et actualise la couleur des boutons de sélection
 */
void Classique(Widget w, void *d) {
	data *s = (data *)d;
	
	s->mode=CLASSIQUE;
	SetBgColor(w, GetRGBColor(200, 200, 200)); // Idem que pour la fonction précédente
	SetBgColor(s->boutonDayAndNight, WHITE);
}

/*
 * Rôle : augmente la vitesse d'exécution du jeu en diminuant le temps entre deux tours
 */
void speedUp(Widget w, void *d) {
	static char str[10];
	data *s = (data *)d;
	
	if(s->time>100) { // On a choisi d'augmenter et diminuer les temps de transition par pas de 100ms, donc avant de décrémenter, on vérifie que l'intervalle actuel
		s->time-=100; // soit bien supérieur à 100 ms, sinon on se retrouve avec un temps de 0s et le jeu plante
		sprintf(str, "%d", s->time);
		SetLabel(s->labelTime, str);
	}
}

/*
 * Rôle : diminue la vitesse d'exécution du jeu en augmentant le temps entre deux tours
 */
void speedDown(Widget w, void *d) {
	static char str[10];
	data *s = (data *)d;

	s->time+=100;
	sprintf(str, "%d", s->time);
	SetLabel(s->labelTime, str);
}
