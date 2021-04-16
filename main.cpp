/*
* Fichier main.
* file
* author Benoit - Paraschivoiu et St - Arnaud
* date    8 avril 2021
* Créé le 1 avril 2021
*/
#include "classes_projet.hpp"
#include "Vue.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QLabel>

#if __has_include("bibliotheque_cours.hpp")
#include "bibliotheque_cours.hpp"
#define BIBLIOTHEQUE_COURS_INCLUS
using bibliotheque_cours::cdbg;
#else
auto& cdbg = clog;
#endif

#if __has_include("verification_allocation.hpp")
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.
#endif

void initialiserBibliothequeCours([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
#ifdef BIBLIOTHEQUE_COURS_INCLUS
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	// cdbg.setTee(&clog);  // Décommenter cette ligne pour que cdbg affiche sur la console en plus de la "Sortie" du débogueur.

	bibliotheque_cours::executerGoogleTest(argc, argv); // Attention de ne rien afficher avant cette ligne, sinon l'Explorateur de tests va tenter de lire votre affichage comme un résultat de test.
#endif
//NOTE: C'est normal que la couverture de code dans l'Explorateur de tests de Visual Studio ne couvre pas la fin de cette fonction ni la fin du main après l'appel à cette fonction puisqu'il exécute uniquement les tests Google Test dans l'appel ci-dessus.
}

int Roi::compteur_ = 0;

int main(int argc, char* argv[])
{
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	QApplication app(argc, argv);
	initialiserBibliothequeCours(argc, argv);

	/*CalcWindow calcWindow;
	calcWindow.show();*/
	Echiquier echiquier;
	//VueEchiquier vueEchiquier = VueEchiquier(nullptr, echiquier);
	/*echiquier.effectuerMouvement(0, 0, 0, 1);
	echiquier.effectuerMouvement(0, 0, 7, 0);
	echiquier.effectuerMouvement(0, 0, 6, 0);
	echiquier.effectuerMouvement(7, 7, 5, 7);
	echiquier.effectuerMouvement(5, 7, 5, 4);
	echiquier.effectuerMouvement(0, 6, 1, 4);
	echiquier.effectuerMouvement(1, 4, 0, 6);
	echiquier.effectuerMouvement(0, 7, 1, 7);
	echiquier.effectuerMouvement(0, 1, 2, 2);
	echiquier.effectuerMouvement(7, 7, 6, 7);
	echiquier.effectuerMouvement(7, 7, 0, 7);
	echiquier.effectuerMouvement(7, 7, 6, 7);*/
	VueEchiquier vueEchiquier = VueEchiquier(nullptr, echiquier);
	vueEchiquier.update();
	vueEchiquier.show();

	app.exec();

	return 0;
}
