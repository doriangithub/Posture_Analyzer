/*
*
*	File: Data3D.h
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <vector>
#include <set>
#include "Vector3D.h"
#include <QtWidgets\qwidget.h>


#define MAX_NUMBER_HORIZONTS 190

class Data3D
{
	char * horizontsFileName;
	int numOfHorizonts;
	std::vector<Vector3D> arrayOfVectors;
	std::vector<Vector3D> arrayofVectorsInBorders;
	std::set<Vector3D> arrayOfUniqPoints;
	std::vector<Vector3D> arrayOfNetPoints;
	std::vector<Vector3D> horizontsPtr[MAX_NUMBER_HORIZONTS];
	float horizontLevel[MAX_NUMBER_HORIZONTS];
	bool borderStatus;
	float mostLeftCoord = 0;
	float mostRightCoord = 0;

	Vector3D minXsurfPoint = Vector3D(0.0, 0.0, 0.0);	//  minimal coordinate X fo Body
	Vector3D maxXsurfPoint = Vector3D(0.0, 0.0, 0.0);	//  maximal coordinate X fo Body
	Vector3D minYsurfPoint = Vector3D(0.0, 0.0, 0.0);	//  minimal coordinate Y fo Body
	Vector3D maxYsurfPoint = Vector3D(0.0, 0.0, 0.0);	//  maximal coordinate Y fo Body

	float leftYborder;
	float rightYborder;
	float topXborder;
	float bottomXborder;
	float maxDiff = 0.0001;   //distance between points in trangles
	int numVectorsInBorders;
	QString filePath;

public:
	//static const int MAX_NUMBER_HORIZONTS = 190;
	//static const int getMaxNumberHorizonts();
	void setNumOfHorizonts(int numHoriz);
	int  getNumOfHorizonts();
	std::vector<Vector3D> getArrayofVectors();
	std::vector<Vector3D> getArrayofHorizonts(int numHorizont);
	void insertVector(Vector3D newVector);
	void insertVectorPoints(Vector3D newVector);
	void makeSetOfUniqPoints();
	void getTrianglesInArea();
	void makeNetOfPoints2D();
	void makeNetOfPoints3D();
	float sign(Vector3D p1, Vector3D p2, Vector3D p3);
	bool pointInTriangle(Vector3D pt, Vector3D v1, Vector3D v2, Vector3D v3);
	float makeEquationPlaneThroughPoints(Vector3D n, Vector3D surfP, Vector3D netP1, Vector3D netP2);
	void makeSectionsXNetPoints();
	Vector3D CalculateStartNetPointForTriangle(Vector3D testVec1, Vector3D testVec2, Vector3D testVec3);
	void makeSectionsX();
	unsigned int getNumberOfPoints();
	bool bordesAreSet();
	void setBorderStatus(bool borderStatus);
	void makeBorderCoordinats();
	Vector3D getMinXsurfPoint();
	Vector3D getMaxXsurfPoint();
	Vector3D getMinYsurfPoint();
	Vector3D getMaxYsurfPoint();
	void setBorders(float leftYborder, float rightYborder, float topBorderShift, float bottomBorderShift);
	void resetData();
	void setInputFilePath(QString filePath);
	Data3D();
	~Data3D();
};

