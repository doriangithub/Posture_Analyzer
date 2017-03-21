/*
*
*	File: ShapeGenerator.h
*
*	Application : Posture Analyzer
*
*	Copyright(c) 2016 - 2017 Dorian Sharevich, Richard Voinov
*
*/

#include "ShapeGenerator.h"
#include <glm\glm.hpp>
#include "Vertex.h"
#include "VertexWithNormal.h"


#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

ShapeData ShapeGenerator::makeBodyModel(std::vector<Vector3D> arrayOfVectors)
{
	if (arrayOfVectors.size() > 0)
	{
		ShapeData retShape;

		// conver NormVertexVertexVertex to VertexColor

		int pCnt = 4;
		float col = 0.2f;
		std::vector<glm::vec3> newArrayOfVertex;
		Vector3D testVec;
		glm::vec3 normalVec;
		float x;
		float y;
		float z;

		for (std::vector<Vector3D>::iterator it = arrayOfVectors.begin(); it != arrayOfVectors.end(); ++it)
		{
			testVec = (Vector3D(*it));
			x = testVec.x;
			y = testVec.y;
			z = testVec.z;

			if (pCnt % 4 == 0)
			{
				normalVec = glm::vec3(x, y, z);
				//newArrayOfVertex.push_back(glm::vec3(x, y, z));
			}
			else
			{
				newArrayOfVertex.push_back(glm::vec3(x, y, z));

				if (col > 1.0)
					col = 0.5;
				col = col + 0.2f;
				newArrayOfVertex.push_back(glm::vec3(1.0, +0.8f, +0.6f));
				newArrayOfVertex.push_back(normalVec);
			}
			pCnt++;
		}

		retShape.numVertices = newArrayOfVertex.size();
		retShape.verticesWithNormal = new VertexWithNormal[retShape.numVertices];
		memcpy(retShape.verticesWithNormal, newArrayOfVertex.data(), retShape.numVertices * sizeof(glm::vec3));

		return retShape;
	}
	else
	{
		ShapeData retShape;

		Vertex thisTri[] =
		{
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(+1.0, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(thisTri);
		retShape.vertices = new Vertex[retShape.numVertices];

		retShape.verticesWithNormal = NULL;

		memcpy(retShape.vertices, thisTri, sizeof(thisTri));

		GLushort indices[] = { 0, 1, 2 };
		retShape.numIndices = NUM_ARRAY_ELEMENTS(indices);
		retShape.indices = new GLushort[retShape.numIndices];

		memcpy(retShape.indices, indices, sizeof(indices));

		return retShape;
	}

}


ShapeData ShapeGenerator::makeHorizLine(std::vector<Vector3D> arrayOfVectors)
{
	ShapeData retShape;

	retShape.numVertices = arrayOfVectors.size();
	retShape.vertices = new Vertex[retShape.numVertices];

	memcpy(retShape.vertices, arrayOfVectors.data(), retShape.numVertices * sizeof(glm::vec3));

	return retShape;
}


ShapeData ShapeGenerator::makeCubeFromFile(std::vector<Vector3D> arrayOfVectors)
{
	if (arrayOfVectors.size() > 0)
	{
		ShapeData retShape;

	
		//memcpy(retShape.vertices, newArrayOfVertex.data(), retShape.numVertices*(sizeof(glm::vec3)));

		// conver NVVV to VC

		int pCnt = 4;
		float col = 0.2f;
		std::vector<glm::vec3> newArrayOfVertex;
		Vector3D testVec;
		float x;
		float y;
		float z;
		
		for (std::vector<Vector3D>::iterator it = arrayOfVectors.begin(); it != arrayOfVectors.end(); ++it)
		{
			if (pCnt % 4 == 0)
			{
				//glNormal3d(curP.GetX(), curP.GetY(), curP.GetZ());
			}
			else
			{
				testVec = (Vector3D(*it));
				x = testVec.x;
				y = testVec.y;
				z = testVec.z;

				newArrayOfVertex.push_back(glm::vec3(x, y, z));

				if (col > 1.0)
					col = 0.5;
				col = col + 0.2f;
				newArrayOfVertex.push_back(glm::vec3(col, +0.0f, +0.0f));
			}
			pCnt++;
		}

		retShape.numVertices = newArrayOfVertex.size();
		retShape.vertices = new Vertex[retShape.numVertices];
		memcpy(retShape.vertices, newArrayOfVertex.data(), retShape.numVertices * sizeof(glm::vec3));


		unsigned short stackIndices[] = {
			0, 1, 2, 0, 2, 3, // Top
			4, 5, 6, 4, 6, 7, // Front
			8, 9, 10, 8, 10, 11, // Right
			12, 13, 14, 12, 14, 15, // Left
			16, 17, 18, 16, 18, 19, // Back
			20, 22, 21, 20, 23, 22, // Bottom
		};

		//unsigned short stackIndices[] = {
		//	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		//	21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 
		//};

		retShape.numIndices = NUM_ARRAY_ELEMENTS(stackIndices);
		retShape.indices = new GLushort[retShape.numIndices];
		memcpy(retShape.indices, stackIndices, sizeof(stackIndices));
		return retShape;
	}
	else
	{
		ShapeData retShape;

		Vertex thisTri[] =
		{
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(+1.0, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(thisTri);
		retShape.vertices = new Vertex[retShape.numVertices];

		memcpy(retShape.vertices, thisTri, sizeof(thisTri));

		GLushort indices[] = { 0, 1, 2 };
		retShape.numIndices = NUM_ARRAY_ELEMENTS(indices);
		retShape.indices = new GLushort[retShape.numIndices];

		memcpy(retShape.indices, indices, sizeof(indices));

		return retShape;
	}
}

ShapeData ShapeGenerator::makeTriangle()
{
	ShapeData retShape;

	Vertex thisTri[] =
	{
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(+1.0, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};

	retShape.numVertices = NUM_ARRAY_ELEMENTS(thisTri);
	retShape.vertices = new Vertex[retShape.numVertices];

	memcpy(retShape.vertices, thisTri, sizeof(thisTri));

	GLushort indices[] = { 0, 1, 2 };
	retShape.numIndices = NUM_ARRAY_ELEMENTS(indices);
	retShape.indices = new GLushort[retShape.numIndices];

	memcpy(retShape.indices, indices, sizeof(indices));

	return retShape;
}


ShapeData ShapeGenerator::makeLine()
{
	ShapeData retShape;

	Vertex thisTri[] =
	{
		glm::vec3(0.344726563, 0.196289063, -0.501291156),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(1.0f, 0.0f, 0.0f),
	};

	retShape.numVertices = NUM_ARRAY_ELEMENTS(thisTri);
	retShape.vertices = new Vertex[retShape.numVertices];

	memcpy(retShape.vertices, thisTri, sizeof(thisTri));

	//GLushort indices[] = { 0, 1, 2 };
	//retShape.numIndices = NUM_ARRAY_ELEMENTS(indices);
	//retShape.indices = new GLushort[retShape.numIndices];

	//memcpy(retShape.indices, indices, sizeof(indices));

	return retShape;
}



ShapeData ShapeGenerator::makeCube()
{
	ShapeData retShape;

	glm::vec3 stackVerts[] = {
		glm::vec3(-0.3f, +0.3f, +0.3f), // 0
		glm::vec3(+1.0f, +0.0f, +0.0f), // Colour
		glm::vec3(+0.3f, +0.3f, +0.3f), // 1
		glm::vec3(+0.0f, +1.0f, +0.0f), // Colour
		glm::vec3(+0.3f, +0.3f, -0.3f), // 2
		glm::vec3(+0.0f, +0.0f, +1.0f), // Colour
		glm::vec3(-0.3f, +0.3f, -0.3f), // 3
		glm::vec3(+1.0f, +1.0f, +1.0f), // Colour
		glm::vec3(-0.3f, +0.3f, -0.3f), // 4
		glm::vec3(+1.0f, +0.0f, +1.0f), // Colour
		glm::vec3(+0.3f, +0.3f, -0.3f), // 5
		glm::vec3(+0.0f, +0.5f, +0.2f), // Colour
		glm::vec3(+0.3f, -0.3f, -0.3f), // 6
		glm::vec3(+0.8f, +0.6f, +0.4f), // Colour
		glm::vec3(-0.3f, -0.3f, -0.3f), // 7
		glm::vec3(+0.3f, +1.0f, +0.5f), // Colour
		glm::vec3(+0.3f, +0.3f, -0.3f), // 8
		glm::vec3(+0.2f, +0.5f, +0.2f), // Colour
		glm::vec3(+0.3f, +0.3f, +0.3f), // 9
		glm::vec3(+0.9f, +0.3f, +0.7f), // Colour
		glm::vec3(+0.3f, -0.3f, +0.3f), // 10
		glm::vec3(+0.3f, +0.7f, +0.5f), // Colour
		glm::vec3(+0.3f, -0.3f, -0.3f), // 11
		glm::vec3(+0.5f, +0.7f, +0.5f), // Colour
		glm::vec3(-0.3f, +0.3f, +0.3f), // 12
		glm::vec3(+0.7f, +0.8f, +0.2f), // Colour
		glm::vec3(-0.3f, +0.3f, -0.3f), // 13
		glm::vec3(+0.5f, +0.7f, +0.3f), // Colour
		glm::vec3(-0.3f, -0.3f, -0.3f), // 14
		glm::vec3(+0.4f, +0.7f, +0.7f), // Colour
		glm::vec3(-0.3f, -0.3f, +0.3f), // 15
		glm::vec3(+0.2f, +0.5f, +1.0f), // Colour
		glm::vec3(+0.3f, +0.3f, +0.3f), // 16
		glm::vec3(+0.6f, +1.0f, +0.7f), // Colour
		glm::vec3(-0.3f, +0.3f, +0.3f), // 17
		glm::vec3(+0.6f, +0.4f, +0.8f), // Colour
		glm::vec3(-0.3f, -0.3f, +0.3f), // 18
		glm::vec3(+0.2f, +0.8f, +0.7f), // Colour
		glm::vec3(+0.3f, -0.3f, +0.3f), // 19
		glm::vec3(+0.2f, +0.7f, +1.0f), // Colour
		glm::vec3(+0.3f, -0.3f, -0.3f), // 20
		glm::vec3(+0.8f, +0.3f, +0.7f), // Colour
		glm::vec3(-0.3f, -0.3f, -0.3f), // 21
		glm::vec3(+0.8f, +0.9f, +0.5f), // Colour
		glm::vec3(-0.3f, -0.3f, +0.3f), // 22
		glm::vec3(+0.5f, +0.8f, +0.5f), // Colour
		glm::vec3(+0.3f, -0.3f, +0.3f), // 23
		glm::vec3(+0.9f, +1.0f, +0.2f), // Colour
	};

	retShape.numVertices = NUM_ARRAY_ELEMENTS(stackVerts);
	retShape.vertices = new Vertex[retShape.numVertices];
	memcpy(retShape.vertices, stackVerts, sizeof(stackVerts));

	unsigned short stackIndices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};

	retShape.numIndices = NUM_ARRAY_ELEMENTS(stackIndices);
	retShape.indices = new GLushort[retShape.numIndices];
	memcpy(retShape.indices, stackIndices, sizeof(stackIndices));
	return retShape;
}


ShapeData ShapeGenerator::makeRightBorder(Data3D *data3D)
{
	ShapeData retShape;

	if (data3D->bordesAreSet())
	{
		// calculate new coordinates right border
		float xBordMin = data3D->getMinXsurfPoint().x;
		float xBordMax = data3D->getMaxXsurfPoint().x;
		float yBordMin = data3D->getMinYsurfPoint().y;
		float yBordMax = data3D->getMaxYsurfPoint().y;
		float zBordMax = data3D->getMaximalZValue();
		float zBordMin = data3D->getMinimalZValue();

		// add 10% extra to the front, so it will be more vissible
		zBordMax = zBordMax + (zBordMax - zBordMin) * 0.1;

		// calculate location depending on body center
		float bodyCenter = 0.0 - data3D->shiftToCenter;
		// distance from body center calculate as 1/x from the whole width
		float bordShift = (yBordMax - yBordMin) / 10.0;

		float yBord = bodyCenter - bordShift;

		
		Vertex RightBorder[] =  
		{
			glm::vec3(xBordMax, yBord,  zBordMax), // 0.0
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBord, zBordMin),  // -2.0
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBord,  zBordMax),  // 0.0
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			//---------------------------------------

			glm::vec3(xBordMax, yBord,  zBordMax),  // 0.0
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax, yBord, zBordMin),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBord, zBordMin),  //yBordMin
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(RightBorder);
		retShape.verticesWithNormal = new VertexWithNormal[retShape.numVertices];

		memcpy(retShape.verticesWithNormal, RightBorder, sizeof(RightBorder));

	}
	else
	{
		Vertex RightBorder[] =
		{
			glm::vec3(-1.0f, -1.0f,  0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f,  1.0f,  0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f,  0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(RightBorder);
		retShape.verticesWithNormal = new VertexWithNormal[retShape.numVertices];

		memcpy(retShape.verticesWithNormal, RightBorder, sizeof(RightBorder));
	}

	return retShape;
}



ShapeData ShapeGenerator::makeTopBorder(Data3D *data3D)
{
	ShapeData retShape;

	if (data3D->bordesAreSet())
	{
		// calculate new coordinates left border
		float xBordMin = data3D->getMinXsurfPoint().x;
		float xBordMax = data3D->getMaxXsurfPoint().x;
		float yBordMin = data3D->getMinYsurfPoint().y;
		float yBordMax = data3D->getMaxYsurfPoint().y;

		float positAdj = (xBordMax + xBordMin) / 4.0;

		Vertex topBorder[] =
		{
			glm::vec3(xBordMax - positAdj, yBordMin, 0.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax - positAdj, yBordMax, -2.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax - positAdj, yBordMax, 0.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			//---------------------------------------

			glm::vec3(xBordMax - positAdj, yBordMin, 0.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax - positAdj, yBordMin, -2.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax - positAdj, yBordMax, -2.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(topBorder);
		retShape.verticesWithNormal = new VertexWithNormal[retShape.numVertices];

		memcpy(retShape.verticesWithNormal, topBorder, sizeof(topBorder));

	}
	else
	{
		Vertex topBorder[] =
		{
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),

			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
		};
		retShape.numVertices = NUM_ARRAY_ELEMENTS(topBorder);
		retShape.vertices = new Vertex[retShape.numVertices];

		memcpy(retShape.vertices, topBorder, sizeof(topBorder));

		GLushort indices[] = { 0, 1, 2 };
		retShape.numIndices = NUM_ARRAY_ELEMENTS(indices);
		retShape.indices = new GLushort[retShape.numIndices];

		memcpy(retShape.indices, indices, sizeof(indices));
	}

	return retShape;
}

ShapeData ShapeGenerator::makeMiddleTopBorder(Data3D *data3D)
{
	ShapeData retShape;

	if (data3D->bordesAreSet())
	{
		// calculate new coordinates left border
		float xBordMin = data3D->getMinXsurfPoint().x;
		float xBordMax = data3D->getMaxXsurfPoint().x;
		
		float yBordMin = data3D->getMinYsurfPoint().y;
		float yBordMax = data3D->getMaxYsurfPoint().y;



		float positAdj = (xBordMax + xBordMin) / 4.0;

		Vertex topBorder[] =
		{
			glm::vec3(xBordMax - positAdj, yBordMin, 0.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax - positAdj, yBordMax, -2.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax - positAdj, yBordMax, 0.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			//---------------------------------------

			glm::vec3(xBordMax - positAdj, yBordMin, 0.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax - positAdj, yBordMin, -2.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax - positAdj, yBordMax, -2.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(topBorder);
		retShape.verticesWithNormal = new VertexWithNormal[retShape.numVertices];

		memcpy(retShape.verticesWithNormal, topBorder, sizeof(topBorder));

	}
	else
	{
		Vertex topBorder[] =
		{
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),

			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
		};
		retShape.numVertices = NUM_ARRAY_ELEMENTS(topBorder);
		retShape.vertices = new Vertex[retShape.numVertices];

		memcpy(retShape.vertices, topBorder, sizeof(topBorder));

		GLushort indices[] = { 0, 1, 2 };
		retShape.numIndices = NUM_ARRAY_ELEMENTS(indices);
		retShape.indices = new GLushort[retShape.numIndices];

		memcpy(retShape.indices, indices, sizeof(indices));
	}

	return retShape;
}

ShapeData ShapeGenerator::makeBottomBorder(Data3D *data3D)
{
	ShapeData retShape;

	if (data3D->bordesAreSet())
	{
		// calculate new coordinates left border
		float xBordMin = data3D->getMinXsurfPoint().x;
		float xBordMax = data3D->getMaxXsurfPoint().x;
		float yBordMin = data3D->getMinYsurfPoint().y;
		float yBordMax = data3D->getMaxYsurfPoint().y;

		Vertex bottomBorder[] =
		{
			glm::vec3(xBordMin, yBordMin, 0.0f),
			glm::vec3(1.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBordMax, -2.0f),
			glm::vec3(1.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBordMax, 0.0f),
			glm::vec3(1.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			//---------------------------------------

			glm::vec3(xBordMin, yBordMin, 0.0f),
			glm::vec3(1.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBordMin, -2.0f),
			glm::vec3(1.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBordMax, -2.0f),
			glm::vec3(1.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(bottomBorder);
		retShape.verticesWithNormal = new VertexWithNormal[retShape.numVertices];

		memcpy(retShape.verticesWithNormal, bottomBorder, sizeof(bottomBorder));
	}
	else
	{
		Vertex bottomBorder[] =
		{
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),

			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
		};
		retShape.numVertices = NUM_ARRAY_ELEMENTS(bottomBorder);
		retShape.vertices = new Vertex[retShape.numVertices];

		memcpy(retShape.vertices, bottomBorder, sizeof(bottomBorder));

		GLushort indices[] = { 0, 1, 2 };
		retShape.numIndices = NUM_ARRAY_ELEMENTS(indices);
		retShape.indices = new GLushort[retShape.numIndices];

		memcpy(retShape.indices, indices, sizeof(indices));
	}

	return retShape;
}



ShapeData ShapeGenerator::makeLeftBorder(Data3D *data3D)
{
	ShapeData retShape;

	if (data3D->bordesAreSet())
	{
		// calculate new coordinates left border
		float xBordMin = data3D->getMinXsurfPoint().x;
		float xBordMax = data3D->getMaxXsurfPoint().x;
		float yBordMin = data3D->getMinYsurfPoint().y;
		float yBordMax = data3D->getMaxYsurfPoint().y;
		float zBordMax = data3D->getMaximalZValue();
		float zBordMin = data3D->getMinimalZValue();

		// add 10% extra to the front, so it will be more vissible
		zBordMax = zBordMax + (zBordMax - zBordMin) * 0.1;

		// calculate location depending on body center
		float bodyCenter = 0.0 - data3D->shiftToCenter;
		// distance from body center calculate as 1/x from the whole width
		float bordShift = (yBordMax - yBordMin) / 10.0;

		float yBord = bodyCenter + bordShift;

		Vertex leftBorder[] =
		{
			glm::vec3(xBordMax, yBord,  zBordMax),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBord, zBordMin),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBord, zBordMax),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			//---------------------------------------

			glm::vec3(xBordMax, yBord,  zBordMax),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMax, yBord, zBordMin),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(xBordMin, yBord, zBordMin),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(leftBorder);
		retShape.verticesWithNormal = new VertexWithNormal[retShape.numVertices];

		memcpy(retShape.verticesWithNormal, leftBorder, sizeof(leftBorder));

	}
	else
	{
		Vertex leftBorder[] =
		{
			glm::vec3(-1.0f, -1.0f,  0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f,  1.0f,  0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f,  0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		};

		retShape.numVertices = NUM_ARRAY_ELEMENTS(leftBorder);
		retShape.verticesWithNormal = new VertexWithNormal[retShape.numVertices];

		memcpy(retShape.verticesWithNormal, leftBorder, sizeof(leftBorder));
	}

	return retShape;
}