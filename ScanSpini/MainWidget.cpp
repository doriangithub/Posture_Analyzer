/*
*
*	File: MainWidget.cpp
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#include "MainWidget.h"
#include <QtWidgets\qvboxlayout>
//#include <QtWidgets\qlabel.h>
#include "FileControls.h"
#include "SettingsControls.h"
#include "InfoControls.h"
#include "GraphicalControls.h"


MainWidget::MainWidget()
{
	data3D = new Data3D();
	graphicalControls = new GraphicalControls(data3D);
	settingsControls = new SettingsControls(data3D, graphicalControls);
		
	QVBoxLayout *mainLayout;
	setLayout(mainLayout = new QVBoxLayout);

	QVBoxLayout *topLayout;
	
	mainLayout->addLayout(topLayout = new QVBoxLayout);
	topLayout->addWidget(fileControls = new FileControls(data3D, graphicalControls));
	topLayout->addWidget(settingsControls);
	topLayout->addWidget(new InfoControls());
	//topLayout:setMaximumHeight(150);
	//topLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

	mainLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
	mainLayout->addWidget(graphicalControls);
	
	//mainLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
	//mainLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

	//mainLayout->addWidget();
	


}


MainWidget::~MainWidget()
{
}
