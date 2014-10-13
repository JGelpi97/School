/*
	Joey Gelpi - jbg29
	Assignment 4 - Camera
	Visual Studio 2012
*/


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <vector>
#include <glut.h>

GLfloat *multiply(GLfloat a[4][4], GLfloat b[4]);

GLfloat *cross(GLfloat a[4], GLfloat b[4]);

GLfloat *faceNormal(GLfloat a[4], GLfloat b[4], GLfloat c[4]);

GLdouble dotProduct(GLfloat a[3], GLfloat b[3]);

GLfloat** matrixMult4d(GLfloat A[4][4], GLfloat B[4][4]);