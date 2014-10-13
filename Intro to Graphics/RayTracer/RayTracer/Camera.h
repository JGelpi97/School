/*
	Joey Gelpi - jbg29
	Assignment 4 - Camera
	Visual Studio 2012
*/

#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <math.h>
#include "MatrixMultiplyer.h"
#include <glut.h>

#ifndef M_PI //In VS 2010 M_PI seems to be missing, so we will put it here
#define M_PI 3.14159265
#endif

#define PERSPECTIVE 1
#define ORTHOGRAPHIC 2

class Camera
{
public:
	Camera(void);
	~Camera(void);
	void CreateView();
	void normalize(GLfloat *p);
	void translate(GLfloat t, int x, int y, int z);
	void rotate(GLfloat ang, int x, int y, int z);
	void setUVW();
	void switchViewType();
	GLfloat pos[4];
	GLfloat at[4];
	GLfloat up[4];

	GLfloat u[3];
	GLfloat v[3];
	GLfloat w[3];

	GLfloat dir[4];
	GLdouble right, left, top, bottom, height, width, far, near;

	int camType;
	GLdouble fovy, aspect;
};

