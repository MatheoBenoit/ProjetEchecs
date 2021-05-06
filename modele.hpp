/*
* Fichier contenant les classes du modele du projet final et leurs fonctions.
* file classes_projet.hpp
* auteurs Benoit - Paraschivoiu et St - Arnaud
* date    8 avril 2021
* Créé le 1 avril 2021
*/
#pragma once

#include <string>
#include <utility>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>

static constexpr int gauche = 0;
static constexpr int droite = 1;
static constexpr int blanc = 0;
static constexpr int noir = 1;
static constexpr int nLignes = 8;
static constexpr int nColonnes = 8;
static constexpr int nRoi = 2;
static constexpr int zeroEnASCII = 48;

namespace modele {
	class ConstructionInvalide : public std::logic_error {
	public:
		using std::logic_error::logic_error;
	};

	class Piece {
	public:
		Piece(bool couleur);
		Piece(bool couleur, int positionLigne, int positionColonne);
		virtual ~Piece() = default;
		Piece& operator= (const Piece& autrePiece);
		bool setPosition(int positionLigne, int positionColonne);
		std::pair<int, int> getPosition() const;
		bool getCouleur() const;
		virtual bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const;

	protected:
		int positionLigne_ = 0;
		int positionColonne_ = 0;
		bool couleur_; //0 = blanc, 1 = noir
	};

	class Roi : public Piece {
	public:
		Roi(bool couleur);
		Roi(bool couleur, int positionLigne, int positionColonne);
		~Roi();
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;

	private:
		static int compteur_;
	};

	class Cavalier : public Piece {
	public:
		Cavalier(bool couleur, bool cote);
		Cavalier(bool couleur, int positionLigne, int positionColonne);
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;
	};

	class Tour : public Piece {
	public:
		Tour(bool couleur, bool cote);
		Tour(bool couleur, int positionLigne, int positionColonne);
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;
	};

	class Echiquier {
	public:
		Echiquier(); // construit un echiquier avec la position des pieces standard de début de partie
		Echiquier(std::string nomFichier); // construit un echiquier avec les position des pieces ecrite dans un fichier
		~Echiquier();
		Piece* getPiece(int ligne, int colonne);
		void copieProfonde(Piece* nouveauEchiquier[nLignes][nColonnes], Piece* echiquier[nLignes][nColonnes]);
		bool effectuerMouvement(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);

	private:
		Piece* echiquier_[nLignes][nColonnes]; // nLignes, nColonnes = 8, car jeu d'échecs a 64 cases
		bool echangerPiece(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY, bool pieceAdverse);
		//puisque la seule piece dans notre cas qui peut rencontrer une piece dans son chemin est la tour, le code de cette methode sera implemente en consequent. 
		//Si jamais une version complete du jeu voudra etre faites, le code suivant devra etre changer pour considerer que le fou et la reine (le pion aussi lorsquil avance de deux) pouraient avoir ce probleme.
		bool pieceEnChemin(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);
		std::pair<int, int> getPositionRoi(bool couleur);
		bool miseEnEchec(bool couleur);
	};
}
