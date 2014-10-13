/*
	Joey Gelpi - jbg29
	Assignment 3 - Transformer
*/

#include "MatrixMultiplyer.h"

GLfloat *multiply(GLfloat a[4][4], GLfloat b[4])
{
	GLfloat *result = new GLfloat[4];
	result[0] = (a[0][0] * b[0]) + (a[0][1] * b[1]) + (a[0][2] * b[2]) + (a[0][3] * b[3]);
	result[1] = (a[1][0] * b[0]) + (a[1][1] * b[1]) + (a[1][2] * b[2]) + (a[1][3] * b[3]);
	result[2] = (a[2][0] * b[0]) + (a[2][1] * b[1]) + (a[2][2] * b[2]) + (a[2][3] * b[3]);
	result[3] = (a[3][0] * b[3]) + (a[3][1] * b[1]) + (a[3][2] * b[2]) + (a[3][3] * b[3]);
	return result;

}

GLfloat *cross(GLfloat a[3], GLfloat b[3])
{
	GLfloat *result = new GLfloat[3];
	result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[0] * b[2] - a[2] * b[0];
    result[2] = a[0] * b[1] - a[1] * b[0];
	return result;
}

GLdouble dotProduct(GLfloat a[3], GLfloat b[3])
{
	return ((a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]));
}

GLfloat *faceNormal(GLfloat a[4], GLfloat b[4], GLfloat c[4])
{
	GLfloat edge1[4] = {b[0] - a[0], b[1] - a[1], b[2] - a[2], 1};
    GLfloat edge2[4] = {c[0] - a[0], c[1] - a[1], c[2] - a[2], 1};
    GLfloat *normal = cross(edge1,edge2);
    return normal;
}
