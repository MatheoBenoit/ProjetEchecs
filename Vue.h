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
protected:
	int x_;
	int y_;
public slots:
	/*std::pair<int, int> appuye() {
		return { x_, y_ };
	}*/
	void appuye() {
		appuye1(x_, y_);
	}
	void appuye1(int x, int y) {
		std::cout << x << " " << y << std::endl;
	}

};

class VueEchiquier : public QMainWindow {
	Q_OBJECT
public:
	VueEchiquier(QWidget* parent, Echiquier& echiquier);
	~VueEchiquier() override = default;

signals:


public slots:
	void appuye2() {
		echiquier_.effectuerMouvement(0, 0, 3, 0);
	}

private:
	Echiquier& echiquier_;
};
