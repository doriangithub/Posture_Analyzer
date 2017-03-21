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
//#include <QtWidgets\qQQmlApplicationEngine>

#include "MainWidget.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	//QQmlApplicationEngine engine;
	//engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	MainWidget mainWidg;
	mainWidg.setGeometry(50, 50, 1200, 600);
	mainWidg.show();
	return app.exec();
}