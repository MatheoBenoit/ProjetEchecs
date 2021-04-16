#pragma once
#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <qwidget.h>
#include <QPaintEvent>
#include <QMainWindow>
#include "classes_projet.hpp"

class Bouton : public QPushButton{
public:
	Bouton(QChar& piece, QWidget* parent, int x, int y);
protected:
	int x_;
	int y_;
};

class VueEchiquier : public QMainWindow {
	Q_OBJECT
public:
	VueEchiquier(QWidget* parent, Echiquier& echiquier);
	~VueEchiquier() override = default;

signals:

public slots:

private:
	Echiquier& echiquier_;
};
