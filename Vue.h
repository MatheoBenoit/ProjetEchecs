#pragma once
#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <qwidget.h>
#include <QPaintEvent>
#include <QMainWindow>
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
public slots:

};

class VueEchiquier : public QMainWindow {
	Q_OBJECT
public:
	VueEchiquier(QWidget* parent, Echiquier& echiquier);
	~VueEchiquier() override = default;

signals:


public slots:
	void appuye() {
		Bouton* e = qobject_cast<Bouton*>(sender());

		if (count) {
			position2 = e->getPosition();
			count--; 
			bool mouvement = echiquier_.effectuerMouvement(position1.first, position1.second, position2.first, position2.second);
			if (!mouvement) std::cout << "Mouvement invalide" << endl;
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
};
