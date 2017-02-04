/*
*
*	File: ShapeData.h
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <GL\glew.h>
#include "Vertex.h"
#include "VertexPosition.h"
#include "VertexWithNormal.h"

struct ShapeData 
{
	ShapeData() : vertices(0), numVertices(0), indices(0), numIndices(0) {}
	Vertex* vertices;
	VertexWithNormal* verticesWithNormal;
	GLuint numVertices;
	GLushort *indices;
	GLuint numIndices;

	GLsizeiptr vertexBufferSize() const 
	{
		return numVertices * sizeof(Vertex);
	}

	GLsizeiptr vertexPositionBufferSize() const
	{
		return numVertices * sizeof(VertexPosition);
	}

	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(GLushort);
	}

	void cleanup()
	{
		delete[] vertices;
		delete[] indices;
		numVertices = numIndices = 0;
	}
};
