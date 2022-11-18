#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libsx.h>
#include "donnees.h"
#include "vue.h"

int main(int argc, char *argv[]) {

	srand(time(NULL));
	
	data jeu; // On déclare une structure qui va contenir toutes les données de fonctionnement du jeu
	
	initJeu(&jeu); // On l'initialise

	if(OpenDisplay(argc, argv) == 0) {
		fprintf(stderr, "Can’t open  display\n");
		return EXIT_FAILURE;
	}

	doAnim(&jeu); // On lance la fonction qui s'auto-appellera en permanence

	initDisplay(argc, argv, &jeu); // Puis on initialise l'interface graphique

	MainLoop();

	return EXIT_SUCCESS;
}
