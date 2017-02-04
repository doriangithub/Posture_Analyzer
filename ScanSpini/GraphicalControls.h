/*
*
*	File: GraphicalControls.h
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <QtWidgets\qwidget.h>
#include "Data3D.h"
#include "GraphicalControls.h"
#include "LeftGLWindow.h"


class GraphicalControls : public QWidget
{
	Data3D *data3D;
	LeftGLWindow *leftGLWindow;
public:
	GraphicalControls(Data3D *data3D);
	~GraphicalControls();
	void reInitilizeLeftGLWindow();
};

