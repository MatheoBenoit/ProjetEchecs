/*
* Fichier contenant les classes de la Vue du projet final et leurs fonctions.
* file    vue.hpp
* auteurs Benoit - Paraschivoiu et St - Arnaud
* date    8 mai 2021
* Créé le 14 avril 2021
*/
#pragma warning(push, 0)
#include <QApplication>
#include <QPushButton>
#include <Qwidget.h>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QColor>
#include <QPalette>
#include <QStack>
#include <QGridLayout>
#pragma pop()
#include "modele.hpp"
#include "modele.hpp"

namespace vue {
	class Bouton : public QPushButton {
		Q_OBJECT
	public:
		Bouton(QWidget* parent, int positionX, int positionY);
		~Bouton() override = default;
		std::pair<int, int> getPosition();
		void couleurNormal(int ligne, int colonne);
		void couleurCaseSelectionne();
		void initialiserTaille(QFont font);
		void initialiserCouleur(QColor couleur);

	private:
		int positionX_;
		int positionY_;
	};

	class VueEchiquier : public QMainWindow {
		Q_OBJECT
	public:
		VueEchiquier(QWidget* parent, modele::Echiquier& echiquier);
		~VueEchiquier() override = default;

	protected slots:
		void boutonAppuye();
		void initialiserPartie(std::string fichier);
		void redo();
		void undo();
		void initPartieStandard();

	private:
		QStack<modele::Commande*> *undoStack;
		modele::Echiquier& echiquier_;
		bool premierClickFait_ = false;
		bool tourDesBlancs_ = true;
		std::pair<int, int> positionChoisie_; // position sur l echiquier de la piece qu on veutb bouger
		std::pair<int, int> positionVoulue_; // position ou on deplace la piece prealablement choisie
		Bouton* matriceBoutons_[nLignes][nColonnes];
		QPushButton* creerBouton(QGridLayout* gridLayout, QString nomBouton, int positionY);
		void initBoutonsPartie(QGridLayout* gridLayout);
		void identifierPiece(QChar& pieceVue, int colonne, int ligne) const;
		void ajouterBouton(Bouton* bouton, int ligne, int colonne);
		bool tourATour();
		void miseAJourVue();
		void mettrePieces();
	};
}
