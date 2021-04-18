/*
* Fichier contenant l'implementation des methode des classe de la Vue du projet final.
* file
* author Benoit - Paraschivoiu et St - Arnaud
* date    16 avril 2021
* Créé le 14 avril 2021
*/
#include "Vue.h"
#pragma warning(push, 0)
#include <QApplication>
#include <qwidget.h>
#include <QPainter>
#include <QPaintEvent>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QColor>
#include <QPalette>
#include <QGridLayout>
#pragma pop()

Bouton::Bouton(QChar& piece, QWidget* parent, int positionX, int positionY) : QPushButton(piece, parent) {
	positionX_ = positionX;
	positionY_ = positionY;
}

VueEchiquier::VueEchiquier(QWidget* parent, Echiquier& echiquier) : echiquier_(echiquier),  QMainWindow(parent) {
	
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