/*
*
*	File: MainFile.cpp
*
*	Application: Posture Analyzer
*	Description: Application load an processing mesh model of scanned human back
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/


#include <QtWidgets\qapplication.h>
#include "MainWidget.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWidget mainWidg;
	mainWidg.setGeometry(50, 50, 800, 600);
	mainWidg.show();
	return app.exec();
}