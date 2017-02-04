/*
*
*	File: InfoControls.cpp
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#include "InfoControls.h"
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qlayout.h>


InfoControls::InfoControls()
{
	QLayout *fileInfoLayout;
	setLayout(fileInfoLayout = new QHBoxLayout);

	QLabel* fileInfoLabel;
	fileInfoLayout->addWidget(fileInfoLabel = new QLabel);
	fileInfoLabel->setMinimumWidth(400);
	fileInfoLabel->setText("Surface points saved in: C:\\");
	fileInfoLabel->setGeometry(0, 0, 400, 14);
	//fileDataLabel->setAlignment(Qt::AlignCenter);
}


InfoControls::~InfoControls()
{
}
