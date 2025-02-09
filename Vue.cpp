/*
* Fichier contenant l'implementation des methode des classes de la Vue du projet final.
* file   vue.cpp
*/
#include "vue.hpp"
# include "modele.hpp"

static constexpr int tailleCase = 100;
static constexpr int tailleTexte = 45;
static constexpr int tailleBoutons = 120;

namespace vue {

	// methodes de la classe Bouton

	Bouton::Bouton(QWidget* parent, int positionX, int positionY) : QPushButton(parent) {
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
		font.setPointSize(tailleTexte);
		setFont(font);
		QSize taille = QSize(tailleCase, tailleCase);
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
				Bouton* bouton = new Bouton(this, colonne, ligne);

				QFont font = VueEchiquier::font();
				bouton->initialiserTaille(font);
				bouton->couleurNormal(ligne, colonne);

				gridLayout->addWidget(bouton, nColonnes - 1 - colonne, ligne);
				ajouterBouton(bouton, ligne, colonne);

				connect(bouton, &QPushButton::clicked, this, &VueEchiquier::boutonAppuye);
			}
		}
		setCentralWidget(widget);
		setWindowTitle("Jeu d'Echec");
		initialiserPartie("PartieStandard.txt");
	}

	QPushButton* VueEchiquier::creerBouton(QGridLayout* gridLayout, QString nomBouton, int positionY) {
		QPushButton* bouton = new QPushButton(nomBouton);
		gridLayout->addWidget(bouton, positionY, nColonnes + 1);
		bouton->setFixedSize(QSize(tailleBoutons, tailleCase));
		return bouton;
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
			else if (dynamic_cast<modele::Fou*>(piece)) {
				couleur ? pieceVue = QChar(0x265D) : pieceVue = QChar(0x2657); // unicode de fou blanc ou noir
			}
			else if (dynamic_cast<modele::Pion*>(piece)) {
				couleur ? pieceVue = QChar(0x265F) : pieceVue = QChar(0x2659); // unicode de pion blanc ou noir
			}
			else if (dynamic_cast<modele::Reine*>(piece)) {
				couleur ? pieceVue = QChar(0x265B) : pieceVue = QChar(0x2655); // unicode de reine blanc ou noir
			}
		}
	}

	void VueEchiquier::ajouterBouton(Bouton * bouton, int ligne, int colonne) {
		matriceBoutons_[ligne][colonne] = bouton;
	}

	// methode appellee lors d'un click de bouton par l usager
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

	void VueEchiquier::mettrePieces() {
		for (int ligne = 0; ligne < nLignes; ligne++) {
			for (int colonne = 0; colonne < nColonnes; colonne++)
			{
				QChar pieceVue;
				identifierPiece(pieceVue, colonne, ligne);
				matriceBoutons_[ligne][colonne]->setText(pieceVue);
				matriceBoutons_[ligne][colonne]->couleurNormal(ligne, colonne); //on remet les bonnes couleurs si jamais une des cases avaient une couleur orange (case selectionnee)
			}
		}
	}

	void VueEchiquier::miseAJourVue() {
		bool mouvementFait = echiquier_.effectuerMouvement(positionChoisie_.first, positionChoisie_.second, positionVoulue_.first, positionVoulue_.second);
		if (mouvementFait) {
			mettrePieces();
		}
		else {
			std::cout << "Mouvement invalide." << std::endl;
			tourDesBlancs_ = !tourDesBlancs_; // on change le booleen pour que la couleur entrain de jouer doive reprendre son coup jusqu'a ce qu'il soit valide
		}
	}

	void VueEchiquier::initialiserPartie(std::string fichier) {
		echiquier_.modifierEchiquier(fichier); //on met les pieces dans lechiquier selon le fichier en parametre
		mettrePieces(); //on met les pieces dans la vue de lechiquier
		tourDesBlancs_ = true; 
		premierClickFait_ = false;
	}
}