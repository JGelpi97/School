/*
	Joey Gelpi - jbg29
	Assignment 3 - Transformer
*/


#include <vector>
#include <glut.h>

GLfloat *multiply(GLfloat a[4][4], GLfloat b[4]);

GLfloat *cross(GLfloat a[4], GLfloat b[4]);

GLfloat *faceNormal(GLfloat a[4], GLfloat b[4], GLfloat c[4]);

GLdouble dotProduct(GLfloat a[3], GLfloat b[3]);
