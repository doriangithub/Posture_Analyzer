/*
*
*	File: SettingsControls.cpp
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#include <QtWidgets\qlayout.h>
#include "SettingsControls.h"
#include "GraphicalControls.h"


SettingsControls::SettingsControls(Data3D *data3D, GraphicalControls *graphicalControls)
{
	this->data3D = data3D;
	this->graphicalControls = graphicalControls;
	this->makeSectionsButton = new QPushButton;

	setLayout(settingsControlsLayout = new QHBoxLayout);

	settingsControlsLayout->addWidget(numSectionHorizLabel = new QLabel);
	numSectionHorizLabel->setMinimumWidth(40);
	numSectionHorizLabel->setMaximumWidth(150);
	numSectionHorizLabel->setAlignment(Qt::AlignLeft);
	numSectionHorizLabel->setAlignment(Qt::AlignVCenter);
	numSectionHorizLabel->setText("Number of horizontal sections:");

	//numSectionHorizLabel->setGeometry(0, 0, 150, 14);

	settingsControlsLayout->addWidget(numSectionsHorizLineEdit = new QLineEdit);
	numSectionsHorizLineEdit->setMinimumWidth(22);
	numSectionsHorizLineEdit->setMaximumWidth(22);
	numSectionsHorizLineEdit->setText("4");
	//numSectionsHorizLineEdit->setGeometry(0, 0, 50, 14);

	settingsControlsLayout->addWidget(makeSectionsButton);
	makeSectionsButton->setEnabled(true);
	makeSectionsButton->setMinimumWidth(80);
	makeSectionsButton->setText("Make sections");
	makeSectionsButton->setGeometry(0, 0, 100, 14);
	connect(makeSectionsButton, SIGNAL(released()), this, SLOT(handleMakeSectionsButton()));

	settingsControlsLayout->addItem(new QSpacerItem(100, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
}

void SettingsControls::handleMakeSectionsButton()
{
	int numSections = (numSectionsHorizLineEdit->text()).toInt();

	data3D->setNumOfHorizonts(numSections);

	int numOfPoints = (data3D->getArrayofVectors()).size();
	if (numOfPoints > 0)
	{
		//
		data3D->getTrianglesInArea();
		//data3D->makeNetOfPoints2D();
		data3D->makeNetOfPoints3D();
		data3D->makeSectionsXNetPoints();
		//data3D->makeSectionsX();
		graphicalControls->reInitilizeLeftGLWindow();
	}
}


SettingsControls::~SettingsControls()
{
}
