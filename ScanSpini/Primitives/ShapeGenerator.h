/*
*
*	File: ShapeGenerator.cpp
*
*	Application : Posture Analyzer
*
*	Copyright(c) 2016 - 2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <vector>
#include "ShapeData.h"
#include "Vector3D.h"
#include "Data3D.h"


class ShapeGenerator
{
	std::vector<Vector3D> arrayOfVectors;
	Data3D *data3D;
public:
	static ShapeData makeTriangle();
	static ShapeData makeLine();
	static ShapeData makeCube();
	static ShapeData makeCubeFromFile(std::vector<Vector3D> arrayOfVectors);
	static ShapeData makeBodyModel(std::vector<Vector3D> arrayOfVectors);
	static ShapeData makeHorizLine(std::vector<Vector3D> arrayOfVectors);
	static ShapeData makeRightBorder(Data3D *data3D);
	static ShapeData makeLeftBorder(Data3D *data3D);
	static ShapeData makeTopBorder(Data3D *data3D);
	static ShapeData makeMiddleTopBorder(Data3D *data3D);
	static ShapeData makeBottomBorder(Data3D *data3D);
};

