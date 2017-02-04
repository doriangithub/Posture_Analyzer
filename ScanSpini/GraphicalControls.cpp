/*
*
*	File: GraphicalControls.cpp
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#include "GraphicalControls.h"
#include <QtWidgets\qlayout.h>
#include <QtWidgets\qlabel.h>
#include "LeftGLWindow.h"
#include "Data3D.h"


GraphicalControls::GraphicalControls(Data3D *data3D)
{
	this->data3D = data3D;
	QLayout *graphicsControlsLayout;
	setLayout(graphicsControlsLayout = new QHBoxLayout);
	leftGLWindow = new LeftGLWindow(data3D);

	graphicsControlsLayout->addWidget(leftGLWindow);
	leftGLWindow->setMinimumSize(600, 600);
	
	//////////////////////////////////////////////////////////////////////////////
	QLabel* fileLabel2;
	graphicsControlsLayout->addWidget(fileLabel2 = new QLabel);
	fileLabel2->setMinimumWidth(40);
	fileLabel2->setText("Second Data file:");
	//fileDataLabel2->setGeometry(0, 0, 400, 14);
	fileLabel2->setAlignment(Qt::AlignCenter);

}

void GraphicalControls::reInitilizeLeftGLWindow()
{
	this->leftGLWindow->callUpdate();
}

GraphicalControls::~GraphicalControls()
{

}
