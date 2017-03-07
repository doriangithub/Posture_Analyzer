/*
*
*	File: LeftGLWindow.cpp
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

class LeftGLWindow : public QGLWidget
{
public:
	LeftGLWindow(Data3D *data3D);
	~LeftGLWindow();
	void displayData();
	void loadBodyData();
	void loadLinesData();
	void loadRightBorder();
	void loadLeftBorder();
	void loadTopBorder();
	void loadMiddleTopBorder();
	void loadMiddleBottomBorder();
	void loadBottomBorder();
	void loadCube();

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
	float modelDistanceMiddle;
	float modelDistanceRight;
	float rightBorderShift = 0.0f;
	float leftBorderShift = 0.0f;
	float topBorderShift = 0.0f;
	float bottomBorderShift = 0.0f;
	bool showBody = true;
	bool showBorders = true;
	bool showLines = true;
	bool showTriangl = true;

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
	GLuint numVerticesCube;
	GLuint cubeBuffSize;
	
	GLuint triangleVertexBufferID;
	GLuint bodyVertexBufferID;
	GLuint lineVertexBufferID;
	GLuint cubeVertexBufferID;
	GLuint linesVertexBufferID;
	GLuint manyLinesVertexBufferID;
	GLuint rightBorderVertexBufferID;
	GLuint leftBorderVertexBufferID;
	GLuint topBorderVertexBufferID;
	GLuint middleTopBorderVertexBufferID;
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
	GLuint middleTopBorderVertexArrayObjectID;
	GLuint bottomBorderVertexArrayObjectID;

	GLuint cubeVertexArrayObjectID;

	int newMousePosistionY;
	int oldmousePositionY;

	int mousePressPositionX;
	int mousePressPositionY;

	float aspectRatioView1;
	float aspectRatioView3;

	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void modelDistanceUpdate(int mousePointerY);
	void modelDistanceUpdate(int mousePointerY, int pressX, int pressY);

	void keyPressEvent(QKeyEvent *e);

	int widthViewPort;

	float shiftXByKeyPress = 0.0f;
	float shiftYByKeyPress = 0.0f;

	int activeViewport = 1;

	// viewports coordinates
	int view1coordX;
	int view1coordY;
	int view1width;
	int view1hight;

	int view2coordX;
	int view2coordY;
	int view2width;
	int view2hight;

	int view3coordX;
	int view3coordY;
	int view3width;
	int view3hight;

	int view4coordX;
	int view4coordY;
	int view4width;
	int view4hight;

	void setActiveViewPort(int mousePressPositionX, int mousePressPositionY);
	
	int widthAll;
	int heightAll;
public:
	void callUpdate();
};

