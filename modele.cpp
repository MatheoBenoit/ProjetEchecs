/*
* Fichier contenant l'implementation des methode des classe du modele du projet final.
* file
* author Benoit - Paraschivoiu et St - Arnaud
* date    16 avril 2021
* Créé le 14 avril 2021
*/
#include "modele.hpp"

namespace modele {

	// methodes de la classe Piece 

	Piece::Piece(bool couleur, int positionLigne, int positionColonne) {
		couleurNoire_ = couleur;
		positionLigne_ = positionLigne;
		positionColonne_ = positionColonne;
	}

	Piece & Piece::operator= (const Piece & autrePiece) {
		if (this != &autrePiece) {
			positionLigne_ = autrePiece.positionLigne_;
			positionColonne_ = autrePiece.positionColonne_;
			couleurNoire_ = autrePiece.couleurNoire_;
		}
		return *this;
	}


	bool Piece::setPosition(int positionLigne, int positionColonne) {
		if (this->mouvementValide(positionLigne, positionColonne)) {
			positionLigne_ = positionLigne;
			positionColonne_ = positionColonne;
			return true;
		}
		else {
			return false;
		}
	}

	std::pair<int, int> Piece::getPosition() const { return { positionLigne_, positionColonne_ }; }

	bool Piece::getCouleur() const { return couleurNoire_; }

	bool Piece::mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const {
		if ((positionLigneVoulue < 0) | (positionLigneVoulue >= nLignes)) return false;
		else if ((positionColonneVoulue < 0) | (positionColonneVoulue >= nColonnes)) return false;
		else if ((positionColonneVoulue == positionColonne_ && positionLigneVoulue == positionLigne_)) return false;
		return true;
	}

	//  methodes de la classe Roi

	Roi::Roi(bool couleur, int positionLigne, int positionColonne) : Piece(couleur, positionLigne, positionColonne) {
		if (compteur_ >= nRoi) throw ConstructionInvalide("Plus de deux instances de Roi.");
		compteur_++;
	}

	Roi::~Roi() {
		compteur_--;
	}

	bool Roi::mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const {
		if (Piece::mouvementValide(positionLigneVoulue, positionColonneVoulue)) {

			bool validationLigne = abs(positionLigne_ - positionLigneVoulue) <= 1;
			bool validationColonne = abs(positionColonne_ - positionColonneVoulue) <= 1;

			if (validationLigne && validationColonne) {
				return true;
			}
		}
		return false;
	}

	//  methodes de la classe Cavalier

	Cavalier::Cavalier(bool couleur, int positionLigne, int positionColonne) : Piece(couleur, positionLigne, positionColonne) {}

	bool Cavalier::mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const {
		if (Piece::mouvementValide(positionLigneVoulue, positionColonneVoulue)) {
			int variationLigne = abs(positionLigne_ - positionLigneVoulue);
			int variationColonne = abs(positionColonne_ - positionColonneVoulue);

			bool validationLigne = (variationLigne == 2) | (variationLigne == 1);
			bool validationColonne = (variationColonne == 2) | (variationColonne == 1);
			bool validationMouvement = variationColonne != variationLigne;

			if (validationColonne && validationLigne && validationMouvement) {
				return true;
			}
		}
		return false;
	}

	//  methodes de la classe Tour

	Tour::Tour(bool couleur, int positionLigne, int positionColonne) : Piece(couleur, positionLigne, positionColonne) {}

	bool Tour::mouvementValide(int positionLigneVoulue, int positionColonneVoulue) const {
		if (Piece::mouvementValide(positionLigneVoulue, positionColonneVoulue)) {
			int variationLigne = abs(positionLigne_ - positionLigneVoulue);
			int variationColonne = abs(positionColonne_ - positionColonneVoulue);

			if ((variationColonne == 0 && variationLigne > 0) | (variationLigne == 0 && variationColonne > 0)) {
				return true;
			}
		}
		return false;
	}

	// methodes de la classe Echiquier
	
	Echiquier::Echiquier() { 
		for (int ligne = 0; ligne < nLignes; ligne++) {
			for (int colonne = 0; colonne < nColonnes; colonne++) {
					echiquier_[ligne][colonne] = nullptr;
			}
		}
	}

	Echiquier::~Echiquier() {
		for (int ligne = 0; ligne < nLignes; ligne++) {
			for (int colonne = 0; colonne < nColonnes; colonne++) {
				delete echiquier_[ligne][colonne];
				echiquier_[ligne][colonne] = nullptr;
			}
		}
	}

	void Echiquier::modifierEchiquier(std::string nomFichier) { // la lecture des positions se fait selon une notation tres similaire a la notation FEN
		std::ifstream fichier(nomFichier);
		for (int ligne = 0; ligne < nLignes; ligne++) {
			for (int colonne = 0; colonne < nColonnes; colonne++) {
				try
				{
					char caractereFEN;
					fichier >> caractereFEN;

					if (caractereFEN == '/') colonne -= 1; // ce caractere sert a delimiter une nouvelle ligne en notation FEN, on veut qu'il agisse comme iteration vide
					else if (caractereFEN == 't') echiquier_[ligne][colonne] = new Tour(noir, ligne, colonne);
					else if (caractereFEN == 'T') echiquier_[ligne][colonne] = new Tour(blanc, ligne, colonne);
					else if (caractereFEN == 'c') echiquier_[ligne][colonne] = new Cavalier(noir, ligne, colonne);
					else if (caractereFEN == 'C') echiquier_[ligne][colonne] = new Cavalier(blanc, ligne, colonne);
					else if (caractereFEN == 'r') echiquier_[ligne][colonne] = new Roi(noir, ligne, colonne);
					else if (caractereFEN == 'R') echiquier_[ligne][colonne] = new Roi(blanc, ligne, colonne);
					else {
						int iterations = caractereFEN - zeroEnASCII;
						for (int i = 0; i < iterations; i++)
						{
							echiquier_[ligne][colonne + i] = nullptr;
						}
						colonne += iterations - 1;
					}
				}
				catch (ConstructionInvalide& e)
				{
					std::cout << "Erreur: " << e.what() << "Cette construction a ete bloque.";
				}
			}
		}
	}

	Piece* Echiquier::getPiece(int ligne, int colonne) {
		return echiquier_[ligne][colonne];
	}

	void Echiquier::copieProfonde(Piece* nouveauEchiquier[nLignes][nColonnes], Piece* echiquier[nLignes][nColonnes]) {
		for (int ligne = 0; ligne < nLignes; ligne++)
		{
			for (int colonne = 0; colonne < nColonnes; colonne++)
			{
				nouveauEchiquier[ligne][colonne] = echiquier[ligne][colonne];
			}
		}
	}

	bool Echiquier::effectuerMouvement(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY) {
		bool retour;
		Piece* echiquierTemporaire[8][8];
		copieProfonde(echiquierTemporaire, echiquier_);

		if (echiquier_[positionActuelleX][positionActuelleY] == nullptr) return false; //peut pas bouger une piece qui existe pas
		bool couleur = echiquier_[positionActuelleX][positionActuelleY]->getCouleur();
		if (pieceEnChemin(positionActuelleX, positionActuelleY, positionVoulueX, positionVoulueY)) return false;
		else if (echiquier_[positionVoulueX][positionVoulueY] != nullptr) { //donc il y a une piece
			bool memeCouleur = echiquier_[positionVoulueX][positionVoulueY]->getCouleur() == couleur;

			if (memeCouleur)
				return false; //peut pas bouger sur une piece de ta couleur
			else {
				//donc il y a une piece adverse
				retour = echangerPiece(positionActuelleX, positionActuelleY, positionVoulueX, positionVoulueY, true);
			}
		}
		else {
			//donc il ny a pas de piece
			retour = echangerPiece(positionActuelleX, positionActuelleY, positionVoulueX, positionVoulueY, false);
		}
		//on regarde si la derniere modification a genere un echec
		if (retour && miseEnEchec(couleur)) {
			// on est alors en echec
			// on veut remettre les pieces a letat initial et retourner faux
			copieProfonde(echiquier_, echiquierTemporaire);
			echiquier_[positionActuelleX][positionActuelleY]->setPosition(positionActuelleX, positionActuelleY); // on remet les attributs de la piece en question a leur valeur initiale pusique le coup est impossible 
			return false;
		}
		return retour;
	}

	bool Echiquier::echangerPiece(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY, bool pieceAdverse) {
		if (echiquier_[positionActuelleX][positionActuelleY]->setPosition(positionVoulueX, positionVoulueY)) { //on change les attributs de la piece quon bouge si le mouvement est valide
			if (!dynamic_cast<Roi*>(echiquier_[positionVoulueX][positionVoulueY])) {
				//on rentre ici si le mouvement est valide et quon ne mange pas un roi
				if (pieceAdverse) delete echiquier_[positionVoulueX][positionVoulueY];
				echiquier_[positionVoulueX][positionVoulueY] = echiquier_[positionActuelleX][positionActuelleY]; //on change sa position dans la matrice
				echiquier_[positionActuelleX][positionActuelleY] = nullptr; //il y a maintenant rien a la position actuelle
				return true;
			}
		}
		return false;
	}

	bool Echiquier::pieceEnChemin(int positionActuelleX, int positionActuelleY, int positionVoulueX, int positionVoulueY) {
		if (dynamic_cast<Tour*>(echiquier_[positionActuelleX][positionActuelleY]) == nullptr) return false;
		std::pair<int, int> position;
		int variationLigne = abs(positionActuelleX - positionVoulueX);
		variationLigne > 0 ? position = std::minmax(positionActuelleX, positionVoulueX) : position = std::minmax(positionActuelleY, positionVoulueY);

		bool enChemin;
		for (int i = position.first + 1; i < position.second; i++) { //on ne regarde pas sil y a une piece a la positon actuelle ni voulu, car ceci est deja considerer dans la fonction effectuerMouvement
			variationLigne > 0 ? enChemin = (echiquier_[i][positionActuelleY] != nullptr) : enChemin = (echiquier_[positionActuelleX][i] != nullptr);
			if (enChemin) return true;
		}
		return false;
	}

	std::pair<int, int> Echiquier::getPositionRoi(bool couleur) {
		for (int ligne = 0; ligne < nLignes; ligne++)
		{
			for (int colonne = 0; colonne < nColonnes; colonne++)
			{
				if (dynamic_cast<Roi*>(echiquier_[ligne][colonne]) && echiquier_[ligne][colonne]->getCouleur() == couleur)
					return echiquier_[ligne][colonne]->getPosition();
			}
		}
		return {}; //pour eviter le warning "les chemins de code ne retourne pas tous une valeur"
	}

	bool Echiquier::miseEnEchec(bool couleur) {
		int positionRoiX = getPositionRoi(couleur).first;
		int positionRoiY = getPositionRoi(couleur).second;
		for (int ligne = 0; ligne < nLignes; ligne++)
		{
			for (int colonne = 0; colonne < nColonnes; colonne++)
			{
				if (!echiquier_[ligne][colonne] || echiquier_[ligne][colonne]->getCouleur() == couleur) continue;
				else if (!pieceEnChemin(ligne, colonne, positionRoiX, positionRoiY) && echiquier_[ligne][colonne]->mouvementValide(positionRoiX, positionRoiY)) {
					std::cout << "Vous etes en echec: ";
					return true;
				}
			}
		}
		return false;
	}

}