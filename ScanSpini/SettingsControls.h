/*
*
*	File: SettingsControls.h
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <QtWidgets\qwidget.h>
#include <QtWidgets\qlineedit.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qpushbutton.h>
#include "Data3D.h"
#include "GraphicalControls.h"

class SettingsControls : public QWidget
{
	Q_OBJECT
public:
	SettingsControls(Data3D *data3D, GraphicalControls *graphicalControls);
	~SettingsControls();

private slots:
	void handleMakeSectionsButton();

private:
	QLayout *settingsControlsLayout;
	QLabel* numSectionHorizLabel;
	QLineEdit* numSectionsHorizLineEdit;
	QPushButton *makeSectionsButton;
	Data3D *data3D;
	GraphicalControls *graphicalControls;
	
};

