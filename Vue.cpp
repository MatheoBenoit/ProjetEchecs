/*
* Fichier contenant l'implementation des methode des classe de la Vue du projet final.
* file
* author Benoit - Paraschivoiu et St - Arnaud
* date    16 avril 2021
* Cr�� le 14 avril 2021
*/
#include "vue.hpp"

namespace vue {

	// methodes de la classe Bouton

	Bouton::Bouton(QChar& piece, QWidget* parent, int positionX, int positionY) : QPushButton(piece, parent) {
		positionX_ = positionX;
		positionY_ = positionY;
	}

	std::pair<int, int> Bouton::getPosition() {
		return { positionX_, positionY_ };
	}

	void Bouton::initialiserCouleur(QColor couleur) {
		QPalette couleurVue = palette();
		couleurVue.setColor(QPalette::Button, couleur);
		setAutoFillBackground(true);
		setFlat(true);
		setPalette(couleurVue);
	}

	void Bouton::couleurNormal(int ligne, int colonne) {
		QColor couleur;
		(ligne % 2) == (colonne % 2) ? couleur = QColor(200, 200, 229) : couleur = QColor(255, 255, 255); // violet et blanc
		initialiserCouleur(couleur);
	}

	void Bouton::couleurCaseSelectionne() {
		QColor couleur = QColor(255, 190, 125); // orange
		initialiserCouleur(couleur);
	}

	void Bouton::initialiserTaille(QFont font) {
		font.setPointSize(45);
		setFont(font);
		QSize taille = QSize(100, 100);
		setFixedSize(taille);
	}


	// methodes de la classe VueEchiquier

	VueEchiquier::VueEchiquier(QWidget* parent, modele::Echiquier& echiquier) : echiquier_(echiquier), QMainWindow(parent) {

		auto widget = new QWidget(this);
		auto layout = new QVBoxLayout(widget);
		auto gridLayout = new QGridLayout();

		layout->addLayout(gridLayout);
		gridLayout->setVerticalSpacing(0);
		gridLayout->setHorizontalSpacing(0);

		for (int ligne = 0; ligne < nLignes; ligne++) 
		{
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
		QPushButton* bouton = new QPushButton("&Partie1");
		gridLayout->addWidget(bouton, 9, 4);
		QObject::connect(bouton, &QPushButton::clicked, this, &VueEchiquier::boutonAppuye);
		QPushButton* bouton2 = new QPushButton("&Partie2");
		gridLayout->addWidget(bouton2, 9, 3);

		setCentralWidget(widget);
		setWindowTitle("Jeu d'Echec");
	}

	void VueEchiquier::identifierPiece(QChar & pieceVue, int colonne, int ligne) const {
		modele::Piece* piece = echiquier_.getPiece(colonne, ligne);
		if (piece)
		{
			bool couleur = piece->getCouleur();
			if (dynamic_cast<modele::Roi*>(piece)) {
				couleur ? pieceVue = QChar(0x265A) : pieceVue = QChar(0x2654); // unicode de roi blanc ou noir
			}
			else if (dynamic_cast<modele::Tour*>(piece)) {
				couleur ? pieceVue = QChar(0x265C) : pieceVue = QChar(0x2656); // unicode de tour blanche ou noire
			}
			else if (dynamic_cast<modele::Cavalier*>(piece)) {
				couleur ? pieceVue = QChar(0x265E) : pieceVue = QChar(0x2658); // unicode de cavalier blanc ou noir
			}
		}
	}

	void VueEchiquier::ajouterBouton(Bouton * bouton, int ligne, int colonne) {
		matriceBoutons_[ligne][colonne] = bouton;
	}

	// methode appellee lors d un click de bouton par l usager
	void VueEchiquier::boutonAppuye() {

		Bouton* boutonAppuye = qobject_cast<Bouton*>(sender());

		if (premierClickFait_) {
			premierClickFait_ = false;
			matriceBoutons_[positionChoisie_.second][positionChoisie_.first]->couleurNormal(positionChoisie_.first, positionChoisie_.second);
			positionVoulue_ = boutonAppuye->getPosition();
			miseAJourVue();
		}
		else {
			positionChoisie_ = boutonAppuye->getPosition();
			if (tourATour()) {
				premierClickFait_ = true;
				boutonAppuye->couleurCaseSelectionne();
			}
		}
	}

	bool VueEchiquier::tourATour() {
		modele::Piece* piece = echiquier_.getPiece(positionChoisie_.first, positionChoisie_.second);
		if (piece) { // si cette position de l echiquier est vide cela retourne un nullptr, equivalent donc a false
			bool couleur = piece->getCouleur(); // retourne faux si blanc et vrai si noir
			if (!couleur == tourDesBlancs_) { // on inverse donc la variable couleur pour savoir si c est le tour de la couleur selectionnee de jouer (blanc est 0 selon limplementation faites plus tot alors que si cest au tour des blancs, la variable tourDesBlancs est a true)
				tourDesBlancs_ = !tourDesBlancs_;
				return true;
			}
			std::cout << "Ce n'est pas � votre tour de jouer." << std::endl;
			return false;
		}
		else {
			std::cout << "Case vide s�lectionn�e." << std::endl;
			return false;
		}
	}

	void VueEchiquier::miseAJourVue() {
		bool mouvementFait = echiquier_.effectuerMouvement(positionChoisie_.first, positionChoisie_.second, positionVoulue_.first, positionVoulue_.second);
		if (mouvementFait) {
			for (int ligne = 0; ligne < nLignes; ligne++) {
				for (int colonne = 0; colonne < nColonnes; colonne++)
				{
					QChar pieceVue;
					identifierPiece(pieceVue, colonne, ligne);
					matriceBoutons_[ligne][colonne]->setText(pieceVue);
				}
			}
		}
		else {
			std::cout << "Mouvement invalide." << std::endl;
			tourDesBlancs_ = !tourDesBlancs_; // on change le booleen pour que la couleur en jeu reprenne son coup jusqu'a se qu'il soit valide
		}
	}
}