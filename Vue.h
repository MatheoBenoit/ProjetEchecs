#pragma once
#include <QApplication>
#include <QPainter>
#include <qwidget.h>
#include <QPaintEvent>
#include <QMainWindow>
#include "classes_projet.hpp"


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
