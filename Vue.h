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
	Bouton(QChar& piece, QWidget* parent, int x, int y);
	std::pair<int, int> getPosition() {
		return { x_, y_ };
	}
protected:
	int x_;
	int y_;
};

class VueEchiquier : public QMainWindow {
	Q_OBJECT
public:
	VueEchiquier(QWidget* parent, Echiquier& echiquier);
	~VueEchiquier() override = default;
	void identifierPiece(QChar& pieceVue, int colonne, int ligne) {
		if (echiquier_.getPiece(colonne, ligne))
		{
			Piece* piece = echiquier_.getPiece(colonne, ligne);
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
		matrice[ligne][colonne] = bouton;
	}

public slots:
	void appuye() {
		Bouton* e = qobject_cast<Bouton*>(sender());

		if (count) {
			position2 = e->getPosition();
			count--; 
			bool mouvement = echiquier_.effectuerMouvement(position1.first, position1.second, position2.first, position2.second);
			if (mouvement) {
				for (int ligne = 0; ligne < nLignes; ligne++) {
					for (int colonne = 0; colonne < nColonnes; colonne++)
					{
						QChar pieceVue;
						identifierPiece(pieceVue, colonne, ligne);
						matrice[ligne][colonne]->setText(pieceVue);	
					}
				}
			}
			else std::cout << "Mouvement invalide." << endl;

		}
		else {
			position1 = e->getPosition();
			count++;
		}
	}

private:
	Echiquier& echiquier_;
	int count = 0;
	std::pair<int, int> position1;
	std::pair<int, int> position2;
	Bouton* matrice[8][8];
};
