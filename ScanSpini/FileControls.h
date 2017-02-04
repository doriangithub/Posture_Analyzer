/*
*
*	File: FileControls.h
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <QtWidgets\qwidget.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlineedit.h>
#include <QtWidgets\qlabel.h>
#include <Windows.h>
#include "Data3D.h"
#include "GraphicalControls.h"
#include "SettingsControls.h"

class FileControls : public QWidget
{
	Q_OBJECT
public:
	FileControls(Data3D *data3D, GraphicalControls *graphicalControls);
	~FileControls();

private slots:
	void handleBrowseButton();
	void handleStartButton();
	
private:
	QLabel* fileDataLabel;
	QLineEdit* fileDataLineEdit;
	QLineEdit* numHorizontalSections;
	QPushButton *browsePushButton;
	QPushButton *startPushButton;
	QString filePath;
	Data3D *data3D;
	GraphicalControls *graphicalControls;
	SettingsControls *settingsControls;

	void ReadInChunks(const WCHAR *pszFileName);
	//void readStlFile(const wchar_t* filePath);
	void DisplayError(LPTSTR lpszFunction);

};

