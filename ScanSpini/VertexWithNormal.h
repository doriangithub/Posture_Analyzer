/*
*
*	File: VertexWithNormal.h
*
*	Application : Posture Analyzer
*
*	Copyright(c) 2016 - 2017 Dorian Sharevich, Richard Voinov
*
*/

#pragma once
#include <glm\glm.hpp>

struct VertexWithNormal
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};