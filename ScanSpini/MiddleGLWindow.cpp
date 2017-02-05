/*
*
*	File: MiddleGLWindow.cpp
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#include "MiddleGLWindow.h"
#include "Primitives\Vertex.h"
#include "Primitives\ShapeGenerator.h"
#include "FileControls.h"


using namespace std;

const float X_DELTA = 0.1f;
const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
const uint MAX_TRIS = 20;

//uint numTris = 0;


string readShaderCodeMiddle(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to laod ..." << fileName << endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());


}


MiddleGLWindow::MiddleGLWindow(Data3D *data3D)
{
	this->data3D = data3D;
	rightBorderShift = 0.0f;
	leftBorderShift = 0.0f;
	topBorderShift = 0.0f;
	bottomBorderShift = 0.0f;
}

void MiddleGLWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	//sendDataToOpenGL();
	displayData();
	setupVertexArrays();
	installShaders();
	loadRightBorder();
	loadLeftBorder();
}


void MiddleGLWindow::displayData()
{
	// see video 65

	// first check if there is scanned data 

	int sizeOfScannedData = data3D->getArrayofVectors().size();

	if (sizeOfScannedData > 0)
	{
		// set up camera for scaned body
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		fovYinRadian = 0.3490658504f; // initial fovY
		roatationInRadian = -1.5707963268f; // initial rotation (90 grad)
		modelDistance = -2.0f;
	}
	else
	{
		// set up camera for logo
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		fovYinRadian = 1.5708f; // initial fovY
		roatationInRadian = 0.0f; // initial rotation (90 grad)
		modelDistance = -2.0f;
	}

	// set up initial shapes 

	// LOGO
	ShapeData shape = ShapeGenerator::makeTriangle();

	numTrangleVertices = shape.numVertices;
	glGenBuffers(1, &triangleVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	shape.cleanup();

	// BODY
	shape = ShapeGenerator::makeBodyModel(data3D->getArrayofVectors());

	numBobyVertices = shape.numVertices;

	glGenBuffers(1, &bodyVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bodyVertexBufferID);
	if (shape.verticesWithNormal == NULL)
	{
		glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.verticesWithNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));

	shape.cleanup();

	// LINE
	shape = ShapeGenerator::makeLine();

	numLineVertices = shape.numVertices;
	glGenBuffers(1, &lineVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, lineVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_FLOATS_PER_VERTICE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * NUM_FLOATS_PER_VERTICE, (char*)(sizeof(float) * 3));
	shape.cleanup();

	// LINES

	numLines = data3D->getNumOfHorizonts();

	GLsizeiptr allLinesBuffSize = 0;

	ShapeData linesShapes[MAX_NUMBER_HORIZONTS];

	for (int i = 0; i < numLines; i++)
	{
		linesShapes[i] = ShapeGenerator::makeHorizLine(data3D->getArrayofHorizonts(i));
		numLinesVertices[i] = linesShapes[i].numVertices;
		allLinesBuffSize += linesShapes[i].vertexPositionBufferSize();
	}

	glGenBuffers(1, &linesVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, linesVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, allLinesBuffSize, 0, GL_STATIC_DRAW);

	GLsizeiptr currentoffset = 0;
	for (int i = 0; i < numLines; i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, currentoffset, linesShapes[i].vertexPositionBufferSize(), linesShapes[i].vertices);
		currentoffset += linesShapes[i].vertexPositionBufferSize();
	}

}


void MiddleGLWindow::installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	string temp = readShaderCodeMiddle("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();	// vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);		// 0 - OpenGL, you will figure out the size
	temp = readShaderCodeMiddle("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();  // fragmentShadeCode
	glShaderSource(fragmentShaderID, 1, adapter, 0);	// 0 - OpenGL, you will figure out the size

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	{
		return;
	}

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (!checkProgramStatus(programID))
		return;

	glUseProgram(programID);

	// test shader for line

	GLuint vertexShaderLineID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderLineID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapterLine[1];
	string tempLine = readShaderCodeMiddle("VertexShaderCodeLine.glsl");
	adapterLine[0] = tempLine.c_str();	// vertexShaderCode;
	glShaderSource(vertexShaderLineID, 1, adapterLine, 0);		// 0 - OpenGL, you will figure out the size
	tempLine = readShaderCodeMiddle("FragmentShaderCodeLine.glsl");
	adapterLine[0] = tempLine.c_str();  // fragmentShadeCode
	glShaderSource(fragmentShaderLineID, 1, adapterLine, 0);	// 0 - OpenGL, you will figure out the size

	glCompileShader(vertexShaderLineID);
	glCompileShader(fragmentShaderLineID);

	if (!checkShaderStatus(vertexShaderLineID) || !checkShaderStatus(fragmentShaderLineID))
	{
		return;
	}

	programLineID = glCreateProgram();

	glAttachShader(programLineID, vertexShaderLineID);
	glAttachShader(programLineID, fragmentShaderLineID);

	glLinkProgram(programLineID);

	glDeleteShader(vertexShaderLineID);
	glDeleteShader(fragmentShaderLineID);

	if (!checkProgramStatus(programLineID))
		return;

	//glUseProgram(programLineID);





	// install shaders for body

	//GLuint vertexShaderBodyID = glCreateShader(GL_VERTEX_SHADER);
	//GLuint fragmentShaderBodyID = glCreateShader(GL_FRAGMENT_SHADER);

	//const char* adapterBody[1];
	//string tempBody = readShaderCodeMiddle("VertexBodyShared.glsl");
	//adapterBody[0] = tempBody.c_str();	// vertexShaderCode;
	//glShaderSource(vertexShaderBodyID, 1, adapterBody, 0);		// 0 - OpenGL, you will figure out the size
	//tempBody = readShaderCodeMiddle("FragmentBodyShader.glsl");
	//adapterBody[0] = tempBody.c_str();  // fragmentShadeCode
	//glShaderSource(fragmentShaderBodyID, 1, adapterBody, 0);	// 0 - OpenGL, you will figure out the size

	//glCompileShader(vertexShaderBodyID);
	//glCompileShader(fragmentShaderBodyID);

	//if (!checkShaderStatus(vertexShaderBodyID) || !checkShaderStatus(fragmentShaderBodyID))
	//{
	//	return;
	//}

	//programBodyID = glCreateProgram();

	//glAttachShader(programBodyID, vertexShaderBodyID);
	//glAttachShader(programBodyID, fragmentShaderBodyID);

	//glLinkProgram(programBodyID);

	//glDeleteShader(vertexShaderBodyID);
	//glDeleteShader(fragmentShaderBodyID);

	//if (!checkProgramStatus(programBodyID))
	//	return;

	//glUseProgram(programBodyID);


}

void MiddleGLWindow::setupVertexArrays()
{
	glGenVertexArrays(1, &triangleVertexArrayObjectID);
	glGenVertexArrays(1, &bodyVertexArrayObjectID);
	glGenVertexArrays(1, &lineVertexArrayObjectID);


	// LOGO
	glBindVertexArray(triangleVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	// BODY
	glBindVertexArray(bodyVertexArrayObjectID);
	glEnableVertexAttribArray(0);  // vertices
	glEnableVertexAttribArray(1);  // color
	glEnableVertexAttribArray(2);  // normal
	glBindBuffer(GL_ARRAY_BUFFER, bodyVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

	// LINE
	glBindVertexArray(lineVertexArrayObjectID);
	glEnableVertexAttribArray(0);  // vertices
	glEnableVertexAttribArray(1);  // color
	glBindBuffer(GL_ARRAY_BUFFER, lineVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));


	// LINES

	glGenVertexArrays(1, &nextLineVertexArrayObjectID);
	glBindVertexArray(nextLineVertexArrayObjectID);
	glEnableVertexAttribArray(0);  // vertices
	glBindBuffer(GL_ARRAY_BUFFER, lineVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);


	// RIGHT BORDER

	glGenVertexArrays(1, &rightBorderVertexArrayObjectID);
	glBindVertexArray(rightBorderVertexArrayObjectID);
	glEnableVertexAttribArray(0);  // vertices
	glEnableVertexAttribArray(1);  // color
	glEnableVertexAttribArray(2);  // normal
	glBindBuffer(GL_ARRAY_BUFFER, rightBorderVertexBufferID); // triangleVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

	// LEFT BORDER

	glGenVertexArrays(1, &leftBorderVertexArrayObjectID);
	glBindVertexArray(leftBorderVertexArrayObjectID);
	glEnableVertexAttribArray(0);  // vertices
	glEnableVertexAttribArray(1);  // color
	glEnableVertexAttribArray(2);  // normal
	glBindBuffer(GL_ARRAY_BUFFER, leftBorderVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

	// TOP BORDER

	glGenVertexArrays(1, &topBorderVertexArrayObjectID);
	glBindVertexArray(topBorderVertexArrayObjectID);
	glEnableVertexAttribArray(0);  // vertices
	glEnableVertexAttribArray(1);  // color
	glEnableVertexAttribArray(2);  // normal
	glBindBuffer(GL_ARRAY_BUFFER, topBorderVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

	// BOTTOM BORDER

	glGenVertexArrays(1, &bottomBorderVertexArrayObjectID);
	glBindVertexArray(bottomBorderVertexArrayObjectID);
	glEnableVertexAttribArray(0);  // vertices
	glEnableVertexAttribArray(1);  // color
	glEnableVertexAttribArray(2);  // normal
	glBindBuffer(GL_ARRAY_BUFFER, bottomBorderVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));
}



void MiddleGLWindow::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glUseProgram(programID);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -2.0f));
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), -1.5707963268f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projectionMatrix = glm::perspective(fovYinRadian, ((float)width()) / height(), 0.1f, 10.0f);

	glm::mat4 fullTransfomMatrix = projectionMatrix * translationMatrix * rotationMatrix;

	GLint fullTransfomMatrixUniformLocation = glGetUniformLocation(programID, "fullTransfomMatrix");
	glUniformMatrix4fv(fullTransfomMatrixUniformLocation, 1, GL_FALSE, &fullTransfomMatrix[0][0]);


	// LOGO
	glBindVertexArray(triangleVertexArrayObjectID);
	//glDrawArrays(GL_TRIANGLES, 0, numTrangleVertices);

	// SET UP LIGHT

	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPosition");
	glm::vec3 lightPosition(0.8f, 0.5f, 0.0f);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	glm::vec3 ambientLight(1.0f, 1.0f, 1.0f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	if (showBody)
	{

		// BODY
		glBindVertexArray(bodyVertexArrayObjectID);

		glm::mat4 translationMatrixBody = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, modelDistance));
		glm::mat4 rotationMatrixBody = glm::rotate(glm::mat4(), roatationInRadian, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 projectionMatrixBody = glm::perspective(fovYinRadian, ((float)width()) / height(), 0.1f, 10.0f);

		glm::mat4 fullTransfomMatrixBody = projectionMatrixBody * translationMatrixBody * rotationMatrixBody;

		GLint fullTransfomMatrixUniformLocationBody = glGetUniformLocation(programID, "fullTransfomMatrix");
		glUniformMatrix4fv(fullTransfomMatrixUniformLocationBody, 1, GL_FALSE, &fullTransfomMatrixBody[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, numBobyVertices);

	}
	//glCullFace(GL_FRONT_AND_BACK);


	if (showBorders)
	{
		// RIGHT BORDER

		glBindVertexArray(rightBorderVertexArrayObjectID);
		GLint ambientLightUniformLocationRightBorder = glGetUniformLocation(programID, "ambientLight");
		glUniform3fv(ambientLightUniformLocationRightBorder, 1, &ambientLight[0]);

		glm::mat4 translationMatrixBodyRightBorder = glm::translate(glm::mat4(), glm::vec3(0.0f + rightBorderShift, 0.0f, modelDistance));
		glm::mat4 rotationMatrixBodyRightBorder = glm::rotate(glm::mat4(), roatationInRadian, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 projectionMatrixBodyRightBorder = glm::perspective(fovYinRadian, ((float)width()) / height(), 0.1f, 10.0f);

		glm::mat4 fullTransfomMatrixBodyRightBorder = projectionMatrixBodyRightBorder *
			translationMatrixBodyRightBorder * rotationMatrixBodyRightBorder;

		GLint fullTransfomMatrixUniformLocationBodyRightBorder = glGetUniformLocation(programID, "fullTransfomMatrix");
		glUniformMatrix4fv(fullTransfomMatrixUniformLocationBodyRightBorder, 1, GL_FALSE,
			&fullTransfomMatrixBodyRightBorder[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// LEFT BORDER

		glBindVertexArray(leftBorderVertexArrayObjectID);
		//GLint ambientLightUniformLocationRightBorder = glGetUniformLocation(programID, "ambientLight");
		//glUniform3fv(ambientLightUniformLocationRightBorder, 1, &ambientLight[0]);

		glm::mat4 translationMatrixBodyLeftBorder = glm::translate(glm::mat4(), glm::vec3(0.0f + leftBorderShift, 0.0f, modelDistance));
		glm::mat4 rotationMatrixBodyLeftBorder = glm::rotate(glm::mat4(), roatationInRadian, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 projectionMatrixBodyLeftBorder = glm::perspective(fovYinRadian, ((float)width()) / height(), 0.1f, 10.0f);

		glm::mat4 fullTransfomMatrixBodyLeftBorder = projectionMatrixBodyLeftBorder *
			translationMatrixBodyLeftBorder * rotationMatrixBodyLeftBorder;

		GLint fullTransfomMatrixUniformLocationBodyLeftBorder = glGetUniformLocation(programID, "fullTransfomMatrix");
		glUniformMatrix4fv(fullTransfomMatrixUniformLocationBodyLeftBorder, 1, GL_FALSE,
			&fullTransfomMatrixBodyLeftBorder[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		// TOP BORDER

		glBindVertexArray(topBorderVertexArrayObjectID);
		//GLint ambientLightUniformLocationRightBorder = glGetUniformLocation(programID, "ambientLight");
		//glUniform3fv(ambientLightUniformLocationRightBorder, 1, &ambientLight[0]);

		glm::mat4 translationMatrixBodyTopBorder = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f + topBorderShift, modelDistance));
		glm::mat4 rotationMatrixBodyTopBorder = glm::rotate(glm::mat4(), roatationInRadian, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 projectionMatrixBodyTopBorder = glm::perspective(fovYinRadian, ((float)width()) / height(), 0.1f, 10.0f);

		glm::mat4 fullTransfomMatrixBodyTopBorder = projectionMatrixBodyTopBorder *
			translationMatrixBodyTopBorder * rotationMatrixBodyTopBorder;

		GLint fullTransfomMatrixUniformLocationBodyTopBorder = glGetUniformLocation(programID, "fullTransfomMatrix");
		glUniformMatrix4fv(fullTransfomMatrixUniformLocationBodyTopBorder, 1, GL_FALSE,
			&fullTransfomMatrixBodyTopBorder[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		// BOTTOM BORDER

		glBindVertexArray(bottomBorderVertexArrayObjectID);
		//GLint ambientLightUniformLocationRightBorder = glGetUniformLocation(programID, "ambientLight");
		//glUniform3fv(ambientLightUniformLocationRightBorder, 1, &ambientLight[0]);

		glm::mat4 translationMatrixBodyBottomBorder = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f + bottomBorderShift, modelDistance));
		glm::mat4 rotationMatrixBodyBottomBorder = glm::rotate(glm::mat4(), roatationInRadian, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 projectionMatrixBodyBottomBorder = glm::perspective(fovYinRadian, ((float)width()) / height(), 0.1f, 10.0f);

		glm::mat4 fullTransfomMatrixBodyBottomBorder = projectionMatrixBodyBottomBorder *
			translationMatrixBodyBottomBorder * rotationMatrixBodyBottomBorder;

		GLint fullTransfomMatrixUniformLocationBodyBottomBorder = glGetUniformLocation(programID, "fullTransfomMatrix");
		glUniformMatrix4fv(fullTransfomMatrixUniformLocationBodyBottomBorder, 1, GL_FALSE,
			&fullTransfomMatrixBodyBottomBorder[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}


	// LINES
	glUseProgram(programLinesID);
	GLint ambientLightUniformLocationLines = glGetUniformLocation(programLinesID, "ambientLight");
	glm::vec3 ambientLightLines(1.0f, 1.0f, 1.0f);
	glUniform3fv(ambientLightUniformLocationLines, 1, &ambientLightLines[0]);

	glm::mat4 translationMatrixLines = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, modelDistance + 0.008));
	glm::mat4 rotationMatrixLines = glm::rotate(glm::mat4(), roatationInRadian, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projectionMatrixLines = glm::perspective(fovYinRadian, ((float)width()) / height(), 0.1f, 10.0f);

	glm::mat4 fullTransfomMatrixLines = projectionMatrixLines * translationMatrixLines * rotationMatrixLines;

	GLint fullTransfomMatrixUniformLocationLines = glGetUniformLocation(programLineID, "fullTransfomMatrix");
	glUniformMatrix4fv(fullTransfomMatrixUniformLocationLines, 1, GL_FALSE, &fullTransfomMatrixLines[0][0]);

	glBindVertexArray(nextLineVertexArrayObjectID);

	GLuint vertexDataOffset = 0;
	GLuint numVertexForStripLine = 0;

	for (int i = 0; i < numLines; i++)
	{
		numVertexForStripLine = numLinesVertices[i];
		glDrawArrays(GL_LINE_STRIP, vertexDataOffset, numVertexForStripLine);
		vertexDataOffset += numLinesVertices[i];
	}



}




bool MiddleGLWindow::checkProgramStatus(GLint programID)
{
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool MiddleGLWindow::checkShaderStatus(GLint shaderID)
{
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}



void MiddleGLWindow::installBodyShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	string temp = readShaderCodeMiddle("VertexBodyShared.glsl");
	adapter[0] = temp.c_str();	// vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);		// 0 - OpenGL, you will figure out the size
	temp = readShaderCodeMiddle("FragmentBodyShader.glsl");
	adapter[0] = temp.c_str();  // fragmentShadeCode
	glShaderSource(fragmentShaderID, 1, adapter, 0);	// 0 - OpenGL, you will figure out the size

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	{
		return;
	}

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (!checkProgramStatus(programID))
		return;

	glUseProgram(programID);
}


void MiddleGLWindow::installLinesShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	string temp = readShaderCodeMiddle("VertexShaderCodeLines.glsl");
	adapter[0] = temp.c_str();	// vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);		// 0 - OpenGL, you will figure out the size
	temp = readShaderCodeMiddle("FragmentShaderCodeLines.glsl");
	adapter[0] = temp.c_str();  // fragmentShadeCode
	glShaderSource(fragmentShaderID, 1, adapter, 0);	// 0 - OpenGL, you will figure out the size

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	{
		return;
	}

	programLinesID = glCreateProgram();

	glAttachShader(programLinesID, vertexShaderID);
	glAttachShader(programLinesID, fragmentShaderID);

	glLinkProgram(programLinesID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (!checkProgramStatus(programLinesID))
		return;

	//glUseProgram(programLinesID);
}


void MiddleGLWindow::installBorderShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	string temp = readShaderCodeMiddle("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();	// vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);		// 0 - OpenGL, you will figure out the size
	temp = readShaderCodeMiddle("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();  // fragmentShadeCode
	glShaderSource(fragmentShaderID, 1, adapter, 0);	// 0 - OpenGL, you will figure out the size

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	{
		return;
	}

	programTrangleID = glCreateProgram();

	glAttachShader(programTrangleID, vertexShaderID);
	glAttachShader(programTrangleID, fragmentShaderID);

	glLinkProgram(programTrangleID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (!checkProgramStatus(programTrangleID))
		return;

	glUseProgram(programTrangleID);
}




MiddleGLWindow::~MiddleGLWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}


void MiddleGLWindow::loadBodyData()
{
	glBindVertexArray(bodyVertexArrayObjectID);
	int sizeOfScannedData = data3D->getArrayofVectors().size();
	if (sizeOfScannedData > 0)
	{
		// set up camera for scaned body
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		fovYinRadian = 0.6490658504f; // initial fovY
		roatationInRadian = +1.5707963268f; // initial rotation (90 grad)
		modelDistance = -2.0f;
	}
	ShapeData shape = ShapeGenerator::makeBodyModel(data3D->getArrayofVectors());
	numBobyVertices = shape.numVertices;
	//glGenBuffers(1, &bodyVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bodyVertexBufferID);
	if (shape.verticesWithNormal != NULL)
	{
		glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.verticesWithNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	shape.cleanup();

	data3D->makeSetOfUniqPoints();
	data3D->makeBorderCoordinats();
}


void MiddleGLWindow::loadLinesData()
{
	glBindVertexArray(nextLineVertexArrayObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, linesVertexBufferID);

	GLsizeiptr allLinesBuffSize = 0;
	ShapeData linesShapes[MAX_NUMBER_HORIZONTS];

	numLines = data3D->getNumOfHorizonts();

	for (int i = 0; i < numLines; i++)
	{
		linesShapes[i] = ShapeGenerator::makeHorizLine(data3D->getArrayofHorizonts(i));
		numLinesVertices[i] = linesShapes[i].numVertices;
		allLinesBuffSize += linesShapes[i].vertexPositionBufferSize();
	}

	glBufferData(GL_ARRAY_BUFFER, allLinesBuffSize, 0, GL_STATIC_DRAW);

	GLsizeiptr currentoffset = 0;
	for (int i = 0; i < numLines; i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, currentoffset, linesShapes[i].vertexPositionBufferSize(), linesShapes[i].vertices);
		currentoffset += linesShapes[i].vertexPositionBufferSize();
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);	//NUM_FLOATS_PER_VERTICE

}


void MiddleGLWindow::loadRightBorder()
{
	glBindVertexArray(rightBorderVertexArrayObjectID);

	glGenBuffers(1, &rightBorderVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, rightBorderVertexBufferID);

	GLsizeiptr allLinesBuffSize = 0;
	ShapeData rightBorderShape = ShapeGenerator::makeRightBorder(this->data3D);

	numVerticesRightBorder = rightBorderShape.numVertices;
	rightBorderBuffSize = rightBorderShape.vertexPositionBufferSize();

	//glBufferData(GL_ARRAY_BUFFER, RightBorderBuffSize, 0, GL_STATIC_DRAW);

	glBufferData(GL_ARRAY_BUFFER, rightBorderShape.vertexBufferSize(), rightBorderShape.verticesWithNormal, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

}


void MiddleGLWindow::loadLeftBorder()
{
	glBindVertexArray(leftBorderVertexArrayObjectID);

	glGenBuffers(1, &leftBorderVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, leftBorderVertexBufferID);

	GLsizeiptr allLinesBuffSize = 0;
	ShapeData leftBorderShape = ShapeGenerator::makeLeftBorder(this->data3D);

	numVerticesLeftBorder = leftBorderShape.numVertices;
	rightBorderBuffSize = leftBorderShape.vertexPositionBufferSize();

	//glBufferData(GL_ARRAY_BUFFER, RightBorderBuffSize, 0, GL_STATIC_DRAW);

	glBufferData(GL_ARRAY_BUFFER, leftBorderShape.vertexBufferSize(), leftBorderShape.verticesWithNormal, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

}


void MiddleGLWindow::loadTopBorder()
{
	glBindVertexArray(topBorderVertexArrayObjectID);

	glGenBuffers(1, &topBorderVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, topBorderVertexBufferID);

	GLsizeiptr allLinesBuffSize = 0;
	ShapeData topBorderShape = ShapeGenerator::makeTopBorder(this->data3D);

	numVerticesTopBorder = topBorderShape.numVertices;
	rightBorderBuffSize = topBorderShape.vertexPositionBufferSize();

	glBufferData(GL_ARRAY_BUFFER, topBorderShape.vertexBufferSize(), topBorderShape.verticesWithNormal, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

}


void MiddleGLWindow::loadBottomBorder()
{
	glBindVertexArray(bottomBorderVertexArrayObjectID);

	glGenBuffers(1, &bottomBorderVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bottomBorderVertexBufferID);

	GLsizeiptr allLinesBuffSize = 0;
	ShapeData bottomBorderShape = ShapeGenerator::makeBottomBorder(this->data3D);

	numVerticesBottomBorder = bottomBorderShape.numVertices;
	rightBorderBuffSize = bottomBorderShape.vertexPositionBufferSize();

	glBufferData(GL_ARRAY_BUFFER, bottomBorderShape.vertexBufferSize(), bottomBorderShape.verticesWithNormal, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

}


void MiddleGLWindow::callUpdate()
{
	installBodyShaders();
	loadBodyData();
	installLinesShaders();
	loadLinesData();
	//installBorderShaders();
	loadRightBorder();
	loadLeftBorder();
	loadTopBorder();
	loadBottomBorder();
	update();
}

void MiddleGLWindow::mouseMoveEvent(QMouseEvent* e)
{
	std::cout << "Mouse hold x = " << e->x() << "  y = " << e->y() << endl;

	modelDistanceUpdate(e->x());

	std::cout << "after event" << endl;

	repaint();
	setFocus();
}


void MiddleGLWindow::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)    // Left button...
	{
		oldmousePositionY = e->y();
		std::cout << "Press event" << endl;
	}
	else if (e->button() == Qt::RightButton)   // Right button...
	{
		// Do something related to the right button
	}
	else if (e->button() == Qt::MidButton)   // Middle button...
	{
		// Do something related to the middle button
	}
	setFocus();
}


void MiddleGLWindow::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)    // Left button...
	{
		std::cout << "release event" << endl;
	}
	else if (e->button() == Qt::RightButton)   // Right button...
	{
		// Do something related to the right button
	}
	else if (e->button() == Qt::MidButton)   // Middle button...
	{
		// Do something related to the middle button
	}
	setFocus();
}


void MiddleGLWindow::modelDistanceUpdate(int mousePointerY)
{
	int mouseDelta = mousePointerY - oldmousePositionY;
	std::cout << ">>>>>>>>>>> mouseDelta = " << mouseDelta << endl;
	modelDistance += (mouseDelta*0.01);
	std::cout << "-- modelDistance = " << modelDistance << endl;
	oldmousePositionY = mousePointerY;
	std::cout << "    mousePointerX = " << mousePointerY << endl;
	std::cout << "oldmousePositionX = " << oldmousePositionY << endl;
}


void MiddleGLWindow::keyPressEvent(QKeyEvent *e)
{
	std::cout << "-- keyPressEvent = " << endl;
	switch (e->key())
	{
	case Qt::Key::Key_O:
		rightBorderShift -= 0.01;
		repaint();
		break;
	case Qt::Key::Key_P:
		rightBorderShift += 0.01;
		repaint();
		break;
	case Qt::Key::Key_W:
		leftBorderShift -= 0.01;
		repaint();
		break;
	case Qt::Key::Key_E:
		leftBorderShift += 0.01;
		repaint();
		break;
	case Qt::Key::Key_T:
		topBorderShift += 0.01;
		repaint();
		break;
	case Qt::Key::Key_G:
		topBorderShift -= 0.01;
		repaint();
		break;
	case Qt::Key::Key_J:
		bottomBorderShift += 0.01;
		repaint();
		break;
	case Qt::Key::Key_M:
		bottomBorderShift -= 0.01;
		repaint();
		break;
	case Qt::Key::Key_B: 
		showBody = !showBody;
		repaint();
		break;
	case Qt::Key::Key_V:
		showBorders = !showBorders;
		repaint();
		break;
	}
	data3D->setBorders(leftBorderShift, rightBorderShift, topBorderShift, bottomBorderShift);
	std::cout << "-- rightBorderShift = " << rightBorderShift << endl;
}