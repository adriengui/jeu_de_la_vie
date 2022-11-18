#include <stdio.h>
#include <stdlib.h>
#include <libsx.h>
#include "donnees.h"
#include "vue.h"
#include "callbacks.h"

/*
 * Rôle : remplit la grille de l'interface graphique à partir du tableau d'états, affiche le nombre de cellules vivantes sur la grille et incrémente le nombre de tours
 */
void fillGrille(data *d) {
	static int x,y;
	static char str[10];
	
	ClearDrawArea();
	
	for(x=0;x<HAUTEUR;x++) {
		for(y=0;y<LARGEUR;y++) {			
			if(d->grille[x][y]==VIVANT)
				DrawFilledBox(y*TAILLE, x*TAILLE, TAILLE, TAILLE); // Si la cellule est vivante, on dessine une case pleine
			
			else
				DrawBox(y*TAILLE, x*TAILLE, TAILLE, TAILLE); // Sinon on dessine une case vide
		}
	}
	
	sprintf(str, "%d", nbVivants(d->grille)); // Pour mettre à jour les labels, on écrit les entiers dans la chaîne de caractères str puis on fait les mises à jour à partir de cette chaîne
	SetLabel(d->labelVivants, str);
	
	sprintf(str, "%d", d->tours++);
	SetLabel(d->labelTours, str);
	
	tourSuivant(d); // A la fin, on calcule la grille suivante
}

/*
 * Rôle : cette fonction va s'auto-appeler en permanence et mettra à jour l'interface seulement si l'on est en mode START, et ne fera rien sinon
 */
void doAnim(data *d) {
	if(d->run==START)
		fillGrille(d);
	
	AddTimeOut(d->time, (GeneralCB)doAnim, d);
}

/*
 * Rôle : met en place la structure de l'interface graphique grâce aux différents éléments (boutons, textes, grille...)
 */
void initDisplay(int argc, char **argv, void *d) {
	static char str[10];
	data *s = (data *)d;
	
	Widget graph[22];
	
	graph[0]=MakeDrawArea(LARGEUR*TAILLE, HAUTEUR*TAILLE, drawGrille, NULL);

	graph[1]=MakeButton("Start", start, d);
	graph[2]=MakeButton("Stop", stop, d);
	graph[3]=MakeButton("Pas a pas", PasAPas, d);
	
	graph[4]=MakeLabel("Nombre de tours : ");
	graph[5]=MakeLabel("0      ");
	s->labelTours=graph[5]; // On met cet élément dans la structure afin de pouvoir le modifier à chaque tour
	
	graph[6]=MakeLabel("Nombre d'individus vivants : ");
	graph[7]=MakeLabel("0      ");
	s->labelVivants=graph[7]; // Comme pour le nombre de tours, le nombre d'individus vivants sur la grille sera amené à être modifié en permanence,
							  // on le met donc dans la structure pour le rendre accessible
	
	graph[8]=MakeButton("Initialiser aleatoire", initGrilleRand, d);
	
	graph[9]=MakeLabel("Intervalle : ");
	sprintf(str, "%d", s->time);
	graph[10]=MakeLabel(str);
	s->labelTime=graph[10]; // Cet élément devant être modifié, on le met dans la structure
	graph[11]=MakeLabel("ms");
	
	graph[12]=MakeButton("Augmenter vitesse", speedUp, d);
	graph[13]=MakeButton("Diminuer vitesse", speedDown, d);
	
	graph[14]=MakeLabel("Mode : ");
	graph[15]=MakeButton("Classique", Classique, d);
	SetBgColor(graph[15], GetRGBColor(200, 200, 200)); // Comme on est en mode classique par défaut, ce bouton sera "foncé" au démarrage du jeu
	s->boutonClassique=graph[15];
	graph[16]=MakeButton("Day And Night", DayAndNight, d);
	s->boutonDayAndNight=graph[16]; // Ces deux boutons devront être dans la structure afin que l'on puisse modifier leur couleur lorsque l'on clique dessus
	
	graph[17]=MakeLabel("Motifs : ");
	graph[18]=MakeButton("Vaisseaux", vaisseaux, d);
	graph[19]=MakeButton("Horloge", horloge, d);
	graph[20]=MakeButton("Pentadecathlon", pentadecathlon, d);
	
	graph[21]=MakeButton("Quitter", quit, NULL);
	
	////////////////////////////////////////////////////////////
	
	// Dans cette partie, on agence simplement les différents Widgets afin de créer l'interface voulue
	
	SetWidgetPos(graph[1], PLACE_UNDER, graph[0], NO_CARE, NULL);
	SetWidgetPos(graph[2], PLACE_UNDER, graph[0], PLACE_RIGHT, graph[1]);
	SetWidgetPos(graph[3], PLACE_UNDER, graph[0], PLACE_RIGHT, graph[2]);
	
	SetWidgetPos(graph[4], PLACE_UNDER, graph[1], NO_CARE, NULL);
	SetWidgetPos(graph[5], PLACE_UNDER, graph[1], PLACE_RIGHT, graph[4]);
	
	SetWidgetPos(graph[6], PLACE_UNDER, graph[4], NO_CARE, NULL);
	SetWidgetPos(graph[7], PLACE_UNDER, graph[4], PLACE_RIGHT, graph[6]);
	
	SetWidgetPos(graph[8], PLACE_UNDER, graph[6], NO_CARE, NULL);
	
	SetWidgetPos(graph[9], PLACE_UNDER, graph[8], NO_CARE, NULL);
	SetWidgetPos(graph[10], PLACE_UNDER, graph[8], PLACE_RIGHT, graph[9]);
	SetWidgetPos(graph[11], PLACE_UNDER, graph[8], PLACE_RIGHT, graph[10]);
	
	SetWidgetPos(graph[12], PLACE_UNDER, graph[9], NO_CARE, NULL);
	SetWidgetPos(graph[13], PLACE_UNDER, graph[9], PLACE_RIGHT, graph[12]);
	
	SetWidgetPos(graph[14], PLACE_UNDER, graph[12], NO_CARE, NULL);
	SetWidgetPos(graph[15], PLACE_UNDER, graph[12], PLACE_RIGHT, graph[14]);
	SetWidgetPos(graph[16], PLACE_UNDER, graph[12], PLACE_RIGHT, graph[15]);
	
	SetWidgetPos(graph[17], PLACE_UNDER, graph[14], NO_CARE, NULL);
	SetWidgetPos(graph[18], PLACE_UNDER, graph[14], PLACE_RIGHT, graph[17]);
	SetWidgetPos(graph[19], PLACE_UNDER, graph[14], PLACE_RIGHT, graph[18]);
	SetWidgetPos(graph[20], PLACE_UNDER, graph[14], PLACE_RIGHT, graph[19]);
	
	SetWidgetPos(graph[21], PLACE_UNDER, graph[17], NO_CARE, NULL);
	

	GetStandardColors();

	ShowDisplay();
}
