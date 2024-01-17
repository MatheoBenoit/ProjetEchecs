/*
* Fichier contenant les classes du modele du projet final et leurs fonctions.
* file classes_projet.hpp
*/
#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>

static constexpr int blanc = 0;
static constexpr int noir = 1;
static constexpr int nLignes = 8;
static constexpr int nColonnes = 8;
static constexpr int avantDerniereCase = 7;
static constexpr int nRoi = 2;
static constexpr int zeroEnASCII = 48;

namespace modele {
	class ConstructionInvalide : public std::logic_error {
	public:
		using std::logic_error::logic_error;
	};

	class Piece {
	public:
		Piece(bool couleur, int positionLigne, int positionColonne);
		virtual ~Piece() = default;
		bool setPosition(int positionLigne, int positionColonne);
		std::pair<int, int> getPosition() const;
		bool getCouleur() const;
		virtual bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const;

	protected:
		int positionLigne_ = 0;
		int positionColonne_ = 0;
		bool couleurNoire_; //0 = blanc, 1 = noir
	};

	class Roi : public Piece {
	public:
		Roi(bool couleur, int positionLigne, int positionColonne);
		~Roi();
		mutable bool aDejaBouge = false;
	private:
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;

	private:
		static int compteur_;
	};

	class Cavalier : public Piece {
	public:
		Cavalier(bool couleur, int positionLigne, int positionColonne);
	private:
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;
	};

	class Tour : public Piece {
	public:
		Tour(bool couleur, int positionLigne, int positionColonne);
	private:
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;
	};

	class Pion : public Piece {
	public:
		Pion(bool couleur, int positionLigne, int positionColonne);
	private:
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;
	};

	class Fou : public Piece {
	public:
		Fou(bool couleur, int positionLigne, int positionColonne);
	private:
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;
	};

	class Reine : public Piece {
	public:
		Reine(bool couleur, int positionLigne, int positionColonne);
	private:
		bool mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const override;
	};

	class Echiquier {
	public:
		Echiquier(); // construit un echiquier avec la position des pieces standard de début de partie
		~Echiquier();
		Piece* getPiece(int ligne, int colonne);
		void modifierEchiquier(std::string nomFichier); //place des pieces aux bonnes position selon le fichier passe en parametre
		bool effectuerMouvement(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);

	private:
		Piece* echiquier_[nLignes][nColonnes]; // nLignes, nColonnes = 8, car jeu d'échecs a 64 cases
		void copieProfonde(Piece* nouveauEchiquier[nLignes][nColonnes], Piece* echiquier[nLignes][nColonnes]);
		bool pieceEnChemin(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);
		bool echangerPiece(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);
		bool miseEnEchec(bool couleur);
		void promotionEnReine();
		bool effectuerRoc(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);
		void grandRoc(int positionX);
		void petitRoc(int positionX);
		bool enCheminPion(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);
		bool enCheminTour(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);
		bool enCheminFou(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);
		bool enCheminReine(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY);
		std::pair<int, int> getPositionRoi(bool couleur);
	};
}
