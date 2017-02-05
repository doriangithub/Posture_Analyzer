/*
*
*	File: MiddleGLWindow.cpp
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <QtOpenGL\qgl.h>
#include "Vector3D.h"
#include "Data3D.h"

class MiddleGLWindow : public QGLWidget
{
public:
	MiddleGLWindow(Data3D *data3D);
	~MiddleGLWindow();
	void displayData();
	void loadBodyData();
	void loadLinesData();
	void loadRightBorder();
	void loadLeftBorder();
	void loadTopBorder();
	void loadBottomBorder();

protected:

	Data3D *data3D;

	std::vector<Vector3D> arrayOfVectors;

	void initializeGL();
	void setupVertexArrays();
	void paintGL();
	

	bool checkShaderStatus(GLint shaderID);
	bool checkProgramStatus(GLint programID);

	void installShaders();
	void installBodyShaders();
	void installLinesShaders();
	void installBorderShaders();

	// for camera
	float fovYinRadian;
	float roatationInRadian;
	float modelDistance;
	float rightBorderShift = 0.0f;
	float leftBorderShift = 0.0f;
	float topBorderShift = 0.0f;
	float bottomBorderShift = 0.0f;
	bool showBody = true;
	bool showBorders = true;

	GLuint programID;
	GLuint programBodyID;
	GLuint programLineID;
	GLuint programLinesID;
	GLuint programTrangleID;
	
	GLuint numTrangleVertices;
	GLuint numBobyVertices;
	GLuint numLineVertices;
	GLuint numLinesVertices[MAX_NUMBER_HORIZONTS];
	GLuint numVerticesRightBorder;
	GLuint numVerticesLeftBorder;
	GLuint numVerticesTopBorder;
	GLuint numVerticesBottomBorder;
	GLuint rightBorderBuffSize;
	
	GLuint triangleVertexBufferID;
	GLuint bodyVertexBufferID;
	GLuint lineVertexBufferID;
	GLuint linesVertexBufferID;
	GLuint manyLinesVertexBufferID;
	GLuint rightBorderVertexBufferID;
	GLuint leftBorderVertexBufferID;
	GLuint topBorderVertexBufferID;
	GLuint bottomBorderVertexBufferID;
	int numLines;
	GLuint buffers[50];

	GLuint triangleVertexArrayObjectID;
	GLuint bodyVertexArrayObjectID;
	GLuint lineVertexArrayObjectID;
	GLuint buffersObjectID[50];
	GLuint nextLineVertexArrayObjectID;
	GLuint rightBorderVertexArrayObjectID;
	GLuint leftBorderVertexArrayObjectID;
	GLuint topBorderVertexArrayObjectID;
	GLuint bottomBorderVertexArrayObjectID;

	int newMousePosistionY;
	int oldmousePositionY;

	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void modelDistanceUpdate(int mousePointerY);

	void keyPressEvent(QKeyEvent *e);
	
public:
	void callUpdate();
};

