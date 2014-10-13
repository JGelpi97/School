/*
	Joey Gelpi - jbg29
	Assignment 3 - Transformer
*/


#pragma once
#include <stdlib.h>
#include <glut.h>

/** Instead of messing with arrays, Ill make a point object thats just a vector, except c++ already has a vector class.**/
class Point
{
	public:
		Point(GLfloat x, GLfloat y, GLfloat z, int _w);
		Point();
		GLfloat x, y, z;
		int w;
};