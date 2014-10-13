/*
	Joey Gelpi - jbg29
	Assignment 4 - Camera
	Visual Studio 2012
*/


#pragma once
#ifndef FACE_H

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Point.h"
#include "MatrixMultiplyer.h"

class Face
{
public:
	Face(Point *_v1, Point *_v2, Point *_v3, Point *_v4);
	~Face(void);
	Point *v1, *v2, *v3, *v4;
	GLfloat normal[3];
private:
	void createFaceNormal();
};

#endif
