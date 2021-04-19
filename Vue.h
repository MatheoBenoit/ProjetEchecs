/*
* Fichier contenant les classes de la Vue du projet final et leurs fonctions.
* file Vue.h
* auteurs Benoit - Paraschivoiu et St - Arnaud
* date    16 avril 2021
* Créé le 14 avril 2021
*/
#pragma warning(push, 0)
#include <QApplication>
#include <QPushButton>
#include <qwidget.h>
#include <QMainWindow>
#include <QChar>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QColor>
#include <QPalette>
#include <QGridLayout>
#pragma pop()
#include "classes_projet.hpp"

namespace vue {
	class Bouton : public QPushButton {
		Q_OBJECT
	public:
		Bouton(QChar& piece, QWidget* parent, int positionX, int positionY) : QPushButton(piece, parent) {
			positionX_ = positionX;
			positionY_ = positionY;
		}
		~Bouton() override = default;
		std::pair<int, int> getPosition() {
			return { positionX_, positionY_ };
		}

		void initialiserCouleur(QColor couleur) {
			QPalette couleurVue = palette();
			couleurVue.setColor(QPalette::Button, couleur);
			setAutoFillBackground(true);
			setFlat(true);
			setPalette(couleurVue);
		}

		void couleurNormal(int ligne, int colonne) {
			QColor couleur;
			(ligne % 2) == (colonne % 2) ? couleur = QColor(200, 200, 229) : couleur = QColor(255, 255, 255);
			initialiserCouleur(couleur);
		}

		void couleurCaseSelectionne() {
			QColor couleur = QColor(255, 190, 125);
			initialiserCouleur(couleur);
		}

		void initialiserTaille(QFont font) {
			font.setPointSize(45);
			setFont(font);
			QSize taille = QSize(100, 100);
			setFixedSize(taille);
		}

	protected:
		int positionX_;
		int positionY_;
	};

	class VueEchiquier : public QMainWindow {
		Q_OBJECT
	public:
		VueEchiquier(QWidget* parent, modele::Echiquier& echiquier) : echiquier_(echiquier), QMainWindow(parent) {

			auto widget = new QWidget(this);
			auto layout = new QVBoxLayout(widget);
			auto gridLayout = new QGridLayout();

			layout->addLayout(gridLayout);
			gridLayout->setVerticalSpacing(0);
			gridLayout->setHorizontalSpacing(0);

			for (int ligne = 0; ligne < nLignes; ligne++) {
				for (int colonne = 0; colonne < nColonnes; colonne++)
				{
					QChar pieceVue;
					identifierPiece(pieceVue, colonne, ligne);
					Bouton* bouton = new Bouton(pieceVue, this, colonne, ligne);

					QFont font = VueEchiquier::font();
					bouton->initialiserTaille(font);
					bouton->couleurNormal(ligne, colonne);

					gridLayout->addWidget(bouton, nColonnes - 1 - colonne, ligne);
					ajouterBouton(bouton, ligne, colonne);

					QObject::connect(bouton, &QPushButton::clicked, this, &VueEchiquier::boutonAppuye);
				}
			}
			setCentralWidget(widget);
			setWindowTitle("Jeu d'Echec");
		}

		~VueEchiquier() override = default;

		void identifierPiece(QChar& pieceVue, int colonne, int ligne) {
			modele::Piece* piece = echiquier_.getPiece(colonne, ligne);
			if (piece)
			{
				bool couleur = piece->getCouleur();
				if (dynamic_cast<modele::Roi*>(piece)) {
					couleur ? pieceVue = QChar(0x265A) : pieceVue = QChar(0x2654);
				}
				else if (dynamic_cast<modele::Tour*>(piece)) {
					couleur ? pieceVue = QChar(0x265C) : pieceVue = QChar(0x2656);
				}
				else if (dynamic_cast<modele::Cavalier*>(piece)) {
					couleur ? pieceVue = QChar(0x265E) : pieceVue = QChar(0x2658);
				}
			}
		}

		void ajouterBouton(Bouton* bouton, int ligne, int colonne) {
			matriceBoutons[ligne][colonne] = bouton;
		}

	protected slots:
		void boutonAppuye() {
			Bouton* boutonAppuye = qobject_cast<Bouton*>(sender());
			if (premierClickFait) {
				premierClickFait = false;
				matriceBoutons[position1.second][position1.first]->couleurNormal(position1.first, position1.second);
				position2 = boutonAppuye->getPosition();
				miseAJourVue();
			}
			else {
				position1 = boutonAppuye->getPosition();
				if (tourATour()) {
					premierClickFait = true;
					boutonAppuye->couleurCaseSelectionne();
				}
			}
		}

	private:
		modele::Echiquier& echiquier_;
		bool premierClickFait = false;
		bool tourDesBlancs = true;
		std::pair<int, int> position1;
		std::pair<int, int> position2;
		Bouton* matriceBoutons[nLignes][nColonnes];

		bool tourATour() {
			modele::Piece* piece = echiquier_.getPiece(position1.first, position1.second);
			if (piece) {
				bool couleur = piece->getCouleur();
				if (!couleur == tourDesBlancs) {
					tourDesBlancs = !tourDesBlancs;
					return true;
				}
				std::cout << "Ce nest pas votre tour de jouer." << std::endl;
			}
			return false;
		}

		void miseAJourVue() {
			bool mouvementFait = echiquier_.effectuerMouvement(position1.first, position1.second, position2.first, position2.second);
			if (mouvementFait) {
				for (int ligne = 0; ligne < nLignes; ligne++) {
					for (int colonne = 0; colonne < nColonnes; colonne++)
					{
						QChar pieceVue;
						identifierPiece(pieceVue, colonne, ligne);
						matriceBoutons[ligne][colonne]->setText(pieceVue);
					}
				}
			}
			else {
				std::cout << "Mouvement invalide." << endl;
				tourDesBlancs = !tourDesBlancs; // on change le booleen pour que la couleur en jeu reprenne son coup jusqu'a se qu'il soit valide
			}
		}
	};
}
