/*
*
*	File: MainWidget.h
*
*	Application: Posture Analyzer
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <QtWidgets\qwidget.h>
#include "Data3D.h"
#include "FileControls.h"
#include "SettingsControls.h"
#include "GraphicalControls.h"

class MainWidget : public QWidget
{
	Data3D *data3D;
	FileControls *fileControls;
	SettingsControls *settingsControls;
	GraphicalControls *graphicalControls;

public:
	MainWidget();
	~MainWidget();
};

