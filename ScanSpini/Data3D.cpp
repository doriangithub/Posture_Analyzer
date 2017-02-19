/*
*
*	File: Data3D.cpp
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#include <fstream>
#include "Data3D.h"

Data3D::Data3D()
{
}


Data3D::~Data3D()
{
}

std::vector<Vector3D> Data3D::getArrayofVectors()
{
	return arrayOfVectors;
}

std::vector<Vector3D> Data3D::getArrayofHorizonts(int numHorizont)
{
	return horizontsPtr[numHorizont];
}

void Data3D::insertVector(Vector3D newVector)
{
	arrayOfVectors.push_back(newVector);
}

void Data3D::insertVectorPoints(Vector3D newVector)
{
	arrayOfUniqPoints.insert(newVector);
}

unsigned int Data3D::getNumberOfPoints()
{
	return arrayOfUniqPoints.size();
}

void Data3D::makeSetOfUniqPoints()
{
	int i = 0;
	for (std::vector<Vector3D>::iterator itVec3d = arrayOfVectors.begin(); itVec3d != arrayOfVectors.end(); itVec3d++)
	{
		if (i % 4 == 0)
		{
			i++;
			continue;
		}

		this->insertVectorPoints((Vector3D)*itVec3d);
		i++;
		itVec3d++;
		this->insertVectorPoints((Vector3D)*itVec3d);
		i++;
		itVec3d++;
		this->insertVectorPoints((Vector3D)*itVec3d);
		i++;
	}
}


//
// Функция расчета стартовых координат для границ исследуемой области 
//
void Data3D::makeBorderCoordinats()
{
	// масссив arrayOfUniqPoints содержит уникальные 3D точки поверхности 
	// это массив уже отсортирован по координате x, поэтому первый элемент 
	// содержит координату с минимальным значением x (minXsurfPoint)
	std::set<Vector3D>::iterator itmin = arrayOfUniqPoints.begin();
	minXsurfPoint = Vector3D((*itmin).x, (*itmin).y, (*itmin).z);

	// последний элемент массива сожержит координату с максимальным значением x
	std::set<Vector3D>::reverse_iterator itmax = arrayOfUniqPoints.rbegin();
	maxXsurfPoint = Vector3D((*itmax).x, (*itmax).y, (*itmax).z);

	// перед поиском максимального и минимального значения координат y
	// присвоим любые реальные значения, для простоты возмем координаты найденые выше 
	minYsurfPoint = Vector3D((*itmin).x, (*itmin).y, (*itmin).z);
	maxYsurfPoint = Vector3D((*itmin).x, (*itmin).y, (*itmin).z);

	// цикл поиска минимальных и максимальных значений y
	for (std::set<Vector3D>::iterator it = arrayOfUniqPoints.begin(); it != arrayOfUniqPoints.end(); it++)
	{
		// берем первый элемент массива и сравнивам с имеющимся значение
		// если элемент меньше, то берем его как минимальный 
		if ((*it).y < minYsurfPoint.y)
		{
			minYsurfPoint = Vector3D((*it).x, (*it).y, (*it).z);
		}

		// таким же образом ищем максимальную координату
		if ((*it).y > maxYsurfPoint.y)
		{
			maxYsurfPoint = Vector3D((*it).x, (*it).y, (*it).z);
		}
	}
	setBorderStatus(true);
}


void Data3D::setBorders(float leftBorderShift, float rightBorderShift, float topBorderShift, float bottomBorderShift)
{
	this->leftYborder = maxYsurfPoint.y - leftBorderShift;
	this->rightYborder = minYsurfPoint.y - rightBorderShift;
	this->topXborder = maxXsurfPoint.x + topBorderShift;
	this->bottomXborder = minXsurfPoint.x + bottomBorderShift;
}

/*! Convert a QString to an std::wstring */
std::wstring qToStdWString(const QString &str)
{
#ifdef _MSC_VER
	return std::wstring((const wchar_t *)str.utf16());
#else
	return str.toStdWString();
#endif
}

/*! Convert an std::wstring to a QString */
QString stdWToQString(const std::wstring &str)
{
#ifdef _MSC_VER
	return QString::fromUtf16((const ushort *)str.c_str());
#else
	return QString::fromStdWString(str);
#endif
}





// функция построения горизонтальных профилей
void Data3D::makeSectionsXNetPoints()
{
	// задаем границы по оси x (вдоль позвоночника)
	float minX = bottomXborder; 
	float maxX = topXborder;	

	// находим обсолютную длину чтобы определить шаг между профилями
	float sumMaxMin = std::abs(minX) + std::abs(maxX);
	const int number_horizonts = numOfHorizonts;
	float stepHorizont = sumMaxMin / (number_horizonts - 1);
	
	// задаем допуск который в три раза меньше расстосяния между точками поверхности 
	float treshold = maxDiff / 2.0;

	// переменные для хранения значений величин определяющих корридор для профиля
	float corridorTop;
	float corridorBottom;

	// создаем массивы для хранаия профилей
	for (int i = 0; i < number_horizonts; i++)
	{
		std::vector<Vector3D> horizLevelProfile;
		horizontsPtr[i] = horizLevelProfile;
		if (i == 0)
		{
			horizontLevel[i] = maxX;
		}
		else
		{
			horizontLevel[i] = maxX - stepHorizont * (i);
		}
	}

	// выберам точки попадающие в корридор
	for (std::vector<Vector3D>::iterator it = arrayOfNetPoints.begin(); it != arrayOfNetPoints.end(); it++)
	{
		// apply borders
		if ((*it).y < rightYborder || (*it).y > leftYborder)
		{
			continue;
		}

		if ((*it).x < bottomXborder || (*it).x > topXborder)
		{
			continue;
		}

		for (int i = 0; i < number_horizonts; i++)
		{

			corridorTop = horizontLevel[i] - treshold;
			corridorBottom = horizontLevel[i] + treshold;

			// сохряняем координаты образующие профиль
			if (corridorTop <= (*it).x && (*it).x < corridorBottom)
			{
				if (horizontsPtr[i].size() == 0)
				{
					horizontsPtr[i].push_back(*it);
					continue;
				}
				
				// координаты точек сохраняем в порядке возрастания
				std::vector<Vector3D>::iterator itPoint = horizontsPtr[i].begin();
				while (itPoint != horizontsPtr[i].end())
				{
					if ((*it).y <= (*itPoint).y)
					{
						itPoint = horizontsPtr[i].insert(itPoint, (*it));
						break;
					}
					else
					{
						itPoint++;
						if (itPoint == horizontsPtr[i].end())
						{
							horizontsPtr[i].push_back(*it);
							break;
						}
					}
				}
			}
		}
	}

	// make output file name from input file full path


	std::wstring wpath = qToStdWString(filePath);
	const wchar_t* path = wpath.c_str();

	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	_wsplitpath_s(path, drive, dir, fname, ext);
	printf("Path extracted with _splitpath:\n");
	printf("  Drive: %s\n", drive);
	printf("  Dir: %s\n", dir);
	printf("  Filename: %s\n", fname);
	printf("  Ext: %s\n", ext);

	const wchar_t * extTxt = L".txt";
	
	
	wcsncat_s(fname, extTxt, 4);

	// сохраняем результаты в текстовый файл 


	std::wofstream fout(fname, std::ios::out); // | std::ios::binary);

	//std::wofstream fout(L"C:\\tttttt.txt", std::ios::out); // | std::ios::binary);

	//std::ofstream fout("horizontsX.txt", std::ios::out); // | std::ios::binary);

	for (int i = 0; i < numOfHorizonts; i++)
	{
		size_t size = horizontsPtr[i].size();
		fout << "Horizont number: " << i << "; Number of points: " << size;
		fout << "----------------------------------------\n";

		for (int t = 0; t < size; t++)
		{
			fout << ((horizontsPtr[i])[t]).x << ";" << ((horizontsPtr[i])[t]).y << ";" << ((horizontsPtr[i])[t]).z << std::endl;
		}
	}
	fout.close();
}



// функция вычисления на сколько нужно сдвинуть исследуюмый объект, чтобы он был по центру окна
void Data3D::calculateShiftToCenter()
{
	// возмем ширину на 3 уровнях, подсчитаем середину. 
	// если две из них близки, то это и возмем это за середину.
	// если все сильно разные, то возмем среднее значение

	float lev1min = 0.0;
	float lev1max = 0.0;

	float lev2min = 0.0;
	float lev2max = 0.0;

	float lev3min = 0.0;
	float lev3max = 0.0;


	// задаем границы по оси x (вдоль позвоночника) по максимальным значениям модели

	float minX = minXsurfPoint.x;
	float maxX = maxXsurfPoint.x;
	float minY = minYsurfPoint.y;
	float maxY = maxYsurfPoint.y;

	// находим обсолютную длину чтобы определить шаг между профилями
	float sumMaxMin = std::abs(minY) + std::abs(maxY);
	float stepHorizont = sumMaxMin / 6;							// шаг - четверть

	// задаем допуск который в три раза меньше расстосяния между точками поверхности 
	float treshold = maxDiff / 2.0;

	// переменные для хранения значений величин определяющих корридор для профиля
	float corridorTop;
	float corridorBottom;


	// выберам точки попадающие в корридор
	for (int i = 1; i < 4; i++)
	{

		for (std::vector<Vector3D>::iterator it = arrayOfVectors.begin(); it != arrayOfVectors.end(); it++)
		{

			// apply borders
			if ((*it).y < ((minY + stepHorizont * i) - treshold )|| (*it).y > ((minY + stepHorizont * i) + treshold))
			{
				continue;
			}

			if (i == 1)
			{
				if (lev1min == 0.0 && lev1max == 0.0)
				{
					lev1min = (*it).x;
					lev1max = (*it).x;
				}

				if ((*it).x <= lev1min)
				{
					lev1min = (*it).x;
				}

				if ((*it).x > lev1max)
				{
					lev1max = (*it).x;
				}
			}

			if (i == 2)
			{

				if (lev2min == 0.0 && lev2max == 0.0)
				{
					lev2min = (*it).x;
					lev2max = (*it).x;
				}

				if ((*it).x <= lev2min)
				{
					lev2min = (*it).x;
				}

				if ((*it).x > lev2max)
				{
					lev2max = (*it).x;
				}
			}

			if (i == 3)
			{

				if (lev3min == 0.0 && lev3max == 0.0)
				{
					lev3min = (*it).x;
					lev3max = (*it).x;
				}

				if ((*it).x <= lev3min)
				{
					lev3min = (*it).x;
				}

				if ((*it).x > lev3max)
				{
					lev3max = (*it).x;
				}
			}
		}

		// decide where is the center

	}

	float center1 = (lev1min + lev1max) / 2.0;
	float center2 = (lev2min + lev2max) / 2.0;
	float center3 = (lev3min + lev3max) / 2.0;
	
	shiftToCenter = (center1 + center2 + center3) / 3;

}


void  Data3D::setNumOfHorizonts(int numHoriz)
{
	this->numOfHorizonts = numHoriz;
}

int  Data3D::getNumOfHorizonts()
{
	return this->numOfHorizonts;
}


bool Data3D::bordesAreSet()
{
	return borderStatus;
}

void Data3D::setBorderStatus(bool borderStatus)
{
	this->borderStatus = borderStatus;
}

Vector3D Data3D::getMinXsurfPoint()
{
	return this->minXsurfPoint;
}

Vector3D Data3D::getMaxXsurfPoint()
{
	return this->maxXsurfPoint;
}

Vector3D Data3D::getMinYsurfPoint()
{
	return this->minYsurfPoint;
}

Vector3D Data3D::getMaxYsurfPoint()
{
	return this->maxYsurfPoint;
}

void Data3D::resetData()
{
	arrayOfVectors.clear();
	std::vector<Vector3D>().swap(arrayOfVectors);

	arrayOfUniqPoints.clear();
	std::set<Vector3D>().swap(arrayOfUniqPoints);

	borderStatus = false;
	mostLeftCoord = 0.0;
	mostRightCoord = 0.0;

	minXsurfPoint = Vector3D(0.0, 0.0, 0.0);	//  minimal coordinate X fo Body
	maxXsurfPoint = Vector3D(0.0, 0.0, 0.0);	//  maximal coordinate X fo Body
	minYsurfPoint = Vector3D(0.0, 0.0, 0.0);	//  minimal coordinate Y fo Body
	maxYsurfPoint = Vector3D(0.0, 0.0, 0.0);	//  maximal coordinate Y fo Body

	leftYborder = 0.0;
	rightYborder = 0.0;

	numOfHorizonts = 0;
}


void Data3D::getTrianglesInArea()
{
	if (arrayOfVectors.size() > 0)
	{
		int pCnt = 4;
		float x;
		float y;
		float z;

		// firgre out what the maximun distance between triangle points and use it for setting of treshhold

		// take tri triangles to find the maximum
		if (arrayOfVectors.size() > 5)
		{
			Vector3D vector1 = arrayOfVectors.at(1);
			Vector3D vector2 = arrayOfVectors.at(2);
			Vector3D vector3 = arrayOfVectors.at(3);

			// what is the maximum diference
			float x1 = vector1.x;
			float x2 = vector2.x;
			float x3 = vector3.x;

			float diff1 = abs(x1 - x2);
			float diff2 = abs(x2 - x3);
			float diff3 = abs(x3 - x1);

			maxDiff = diff1;
			if (diff2 > maxDiff)
				maxDiff = diff2;
			if (diff3 > maxDiff)
				maxDiff = diff3;
			if (maxDiff > 0.0001)
				maxDiff = 0.0001;

		}

		float treshholdBorders = maxDiff + maxDiff;

		for (std::vector<Vector3D>::iterator it = arrayOfVectors.begin(); it != arrayOfVectors.end(); ++it)
		{
			Vector3D testVec1 = (Vector3D(*it));
			++it;
			Vector3D testVec2 = (Vector3D(*it));
			++it;
			Vector3D testVec3 = (Vector3D(*it));
			++it;
			Vector3D testVec4 = (Vector3D(*it));

			Vector3D normalVec;
			x = testVec1.x;
			y = testVec1.y;
			z = testVec1.z;


			// check if this triangle within borders
			float leftYborderPlus = leftYborder - treshholdBorders;
			float rightYborderPlus = rightYborder + treshholdBorders;
			float topXborderPlus = topXborder + treshholdBorders;
			float bottomXborderPlus = bottomXborder - treshholdBorders;

			if (testVec2.y < rightYborderPlus || testVec2.y > leftYborderPlus)
			{
				continue;
			}

			if (testVec2.x < bottomXborderPlus || testVec2.x > topXborderPlus)
			{
				continue;
			}

			if (testVec3.y < rightYborderPlus || testVec3.y > leftYborderPlus)
			{
				continue;
			}

			if (testVec3.x < bottomXborderPlus || testVec3.x > topXborderPlus)
			{
				continue;
			}

			if (testVec4.y < rightYborderPlus || testVec4.y > leftYborderPlus)
			{
				continue;
			}

			if (testVec4.x < bottomXborderPlus || testVec4.x > topXborderPlus)
			{
				continue;
			}

			arrayofVectorsInBorders.push_back(testVec1);
			arrayofVectorsInBorders.push_back(testVec2);
			arrayofVectorsInBorders.push_back(testVec3);
			arrayofVectorsInBorders.push_back(testVec4);

		}

		numVectorsInBorders = arrayofVectorsInBorders.size();
	}
}


//void Data3D::makeNetOfPoints2D()
//{
//	// make array of 3D points for area within borders
//
//	float x = bottomXborder;
//	float y = rightYborder;
//
//	maxDiff = 0.001;
//
//	while (x <= topXborder)
//	{
//		while (y <= leftYborder)
//		{
//			//arrayOfNetPoints.push_back(Vector3D(x, y, 0.0));
//			y = y + maxDiff;
//		}
//		x = x + maxDiff;
//		y = rightYborder;
//	}
//
//}


float Data3D::sign(Vector3D p1, Vector3D p2, Vector3D p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}


bool Data3D::pointInTriangle(Vector3D pt, Vector3D v1, Vector3D v2, Vector3D v3)
{
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}


void Data3D::makeNetOfPoints3D()
{
	// set dummy data for testing

	//std::vector<Vector3D> testVectors;

	//testVectors.push_back(Vector3D( 0.0, 0.0,  1.0));
	//testVectors.push_back(Vector3D(-2.0, -2.0, 0.0));
	//testVectors.push_back(Vector3D( 0.0, -1.0, 0.0));
	//testVectors.push_back(Vector3D(-1.0,  0.0, 0.0));

	//rightYborder = -2.1;
	//bottomXborder = -2.5;
	//maxDiff = 0.3;

	while (!arrayofVectorsInBorders.empty())	// arrayofVectorsInBorders
	{
		Vector3D testVec3 = arrayofVectorsInBorders.back();
		arrayofVectorsInBorders.pop_back();

		Vector3D testVec2 = arrayofVectorsInBorders.back();
		arrayofVectorsInBorders.pop_back();

		Vector3D testVec1 = arrayofVectorsInBorders.back();
		arrayofVectorsInBorders.pop_back();

		Vector3D testVecN = arrayofVectorsInBorders.back();
		arrayofVectorsInBorders.pop_back();


		// calculate rest of net points to cover the triangle by adding steps 
		
		float y_tri_min = testVec1.y;
		if (testVec2.y < y_tri_min)
			y_tri_min = testVec2.y;
		if (testVec3.y < y_tri_min)
			y_tri_min = testVec3.y;

		float y_tri_max = testVec1.y;
		if (testVec2.y > y_tri_max)
			y_tri_max = testVec2.y;
		if (testVec3.y > y_tri_max)
			y_tri_max = testVec3.y;

		// find difference between min and max X
		float y_tri_difference = abs(y_tri_min - y_tri_max);

		// calculate net points covering our triangle
		// find difference between start net point X and min X of rangle

		float y_dists_to_tri = rightYborder - y_tri_min;  //topXborderPlus, leftYborder, bottomXborder

														  // calculate number of steps need from net point X to get to neareast point of triangle
		float numbStepY = y_dists_to_tri / maxDiff;
		int y_int = abs((int)numbStepY);

		float y_start_net_for_tri = rightYborder + y_int * maxDiff;

		//=========================================================

		float x_tri_min = testVec1.x;
		if (testVec2.x < x_tri_min)
			x_tri_min = testVec2.x;
		if (testVec3.x < x_tri_min)
			x_tri_min = testVec3.x;

		float x_tri_max = testVec1.x;
		if (testVec2.x > x_tri_max)
			x_tri_max = testVec2.x;
		if (testVec3.x > x_tri_max)
			x_tri_max = testVec3.x;

		// find difference between min and max X
		float x_tri_difference = abs(x_tri_min - x_tri_max);

		// calculate net points covering our triangle
		// find difference between start net point X and min X of rangle

		float x_dists_to_tri = bottomXborder - x_tri_min;  //topXborderPlus, leftYborder, bottomXborder

		float numbStepX = x_dists_to_tri / maxDiff;	// calculate number of steps need from net point X to get to neareast point of triangle
		int x_int = abs((int)numbStepX);

		float x_start_net_for_tri = bottomXborder + x_int * maxDiff;

		//=====================================================

		float x_n_p;
		float y_n_p;
		float z_n_p = 0.0;

		std::vector<Vector3D> arrayOfTempNetPoints;

		for(x_n_p = x_start_net_for_tri; x_n_p <= x_tri_max; x_n_p += maxDiff)
		{
			for (y_n_p = y_start_net_for_tri; y_n_p <= y_tri_max; y_n_p += maxDiff)
			{
				arrayOfTempNetPoints.push_back(Vector3D(x_n_p, y_n_p, z_n_p));
			}
		}
		
		for (std::vector<Vector3D>::iterator it = arrayOfTempNetPoints.begin(); it != arrayOfTempNetPoints.end(); ++it)
		{
			Vector3D pointToCheck = Vector3D((*it).x, (*it).y, (*it).y);
			Vector3D netPoint0 = Vector3D((*it).x, (*it).y, -1.0);
			Vector3D netPoint1 = Vector3D((*it).x, (*it).y,  1.0);

			if (pointInTriangle(pointToCheck, testVec1, testVec2, testVec3))
			{
				// calculate instersecftion point
				z_n_p = makeEquationPlaneThroughPoints(testVecN, testVec1, netPoint0, netPoint1);
			}
			else
			{
				continue;
			}

			// save intersect point
			arrayOfNetPoints.push_back(Vector3D((*it).x, (*it).y, z_n_p));
		}
	}

	int s = arrayOfNetPoints.size();

}


Vector3D Data3D::CalculateStartNetPointForTriangle(Vector3D testVec1, Vector3D testVec2, Vector3D testVec3)
{
	return Vector3D(0.0, 0.0, 0.0);
}


float Data3D::makeEquationPlaneThroughPoints(Vector3D n, Vector3D surfP, Vector3D netP1, Vector3D netP2)
{
	float t = (n.x * surfP.x - n.x * netP1.x - n.y * netP1.y + n.y * surfP.y - n.z * netP1.z + n.z * surfP.z) /
		(n.x * netP2.x - n.x * netP1.x - n.y * netP2.y - n.y * netP1.y + n.z * netP2.z - n.z * netP1.z);

	float x = t * (netP2.x - netP1.x) + netP1.x;
	float y = t * (netP2.y - netP1.y) + netP1.y;
	float z = t * (netP2.z - netP1.z) + netP1.z;

	return z;
}

void Data3D::setInputFilePath(QString filePath)
{
	this->filePath = filePath;
}

