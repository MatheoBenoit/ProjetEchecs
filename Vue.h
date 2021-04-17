/*
* Fichier contenant les classes de la Vue du projet final et leurs fonctions.
* file Vue.h
* auteurs Benoit - Paraschivoiu et St - Arnaud
* date    16 avril 2021
* Créé le 14 avril 2021
*/
#pragma once
#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <qwidget.h>
#include <QPaintEvent>
#include <QMainWindow>
#include <QChar>
#include "classes_projet.hpp"

class Bouton : public QPushButton{
	Q_OBJECT
public:
	Bouton(QChar& piece, QWidget* parent, int positionX, int positionY);
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
	VueEchiquier(QWidget* parent, Echiquier& echiquier);
	~VueEchiquier() override = default;
	void identifierPiece(QChar& pieceVue, int colonne, int ligne) {
		Piece* piece = echiquier_.getPiece(colonne, ligne);
		if (piece)
		{
			bool couleur = piece->getCouleur();
			if (dynamic_cast<Roi*>(piece)) {
				couleur ? pieceVue = QChar(0x265A) : pieceVue = QChar(0x2654);
			}
			else if (dynamic_cast<Tour*>(piece)) {
				couleur ? pieceVue = QChar(0x265C) : pieceVue = QChar(0x2656);
			}
			else if (dynamic_cast<Cavalier*>(piece)) {
				couleur ? pieceVue = QChar(0x265E) : pieceVue = QChar(0x2658);
			}
		}
	}

	void ajouterBouton(Bouton* bouton, int ligne, int colonne) {
		matriceBoutons[ligne][colonne]= bouton;
	}

protected slots:
	void boutonAppuye() {
		Bouton* boutonAppuye = qobject_cast<Bouton*>(sender());
		if (nClick) {
			nClick--; 
			position2 = boutonAppuye->getPosition();
			miseAJourVue();
			matriceBoutons[position1.second][position1.first]->couleurNormal(position1.first, position1.second);
		}
		else {
			position1 = boutonAppuye->getPosition();
			nClick++;
			boutonAppuye->couleurCaseSelectionne();
		}
	}

private:
	Echiquier& echiquier_;
	int nClick = 0;
	std::pair<int, int> position1;
	std::pair<int, int> position2;
	Bouton* matriceBoutons[nLignes][nColonnes];
	void miseAJourVue() {
		bool mouvement = echiquier_.effectuerMouvement(position1.first, position1.second, position2.first, position2.second);
		if (mouvement) {
			for (int ligne = 0; ligne < nLignes; ligne++) {
				for (int colonne = 0; colonne < nColonnes; colonne++)
				{
					QChar pieceVue;
					identifierPiece(pieceVue, colonne, ligne);
					matriceBoutons[ligne][colonne]->setText(pieceVue);
				}
			}
		}
		else std::cout << "Mouvement invalide." << endl;
	}
};
