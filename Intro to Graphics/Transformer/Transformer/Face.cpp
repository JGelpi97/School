/*
	Joey Gelpi - jbg29
	Assignment 3 - Transformer
*/


#include "Face.h"


Face::Face(Point *_v1, Point *_v2, Point *_v3, Point *_v4)
{
	v1 = _v1;
	v2 = _v2;
	v3 = _v3;
	v4 = _v4;
	createFaceNormal();
}

Face::~Face(void)
{

}

void Face::createFaceNormal()
{
	GLfloat edge2[3] = {v3->x - v2->x, v3->y - v2->y, v3->z - v2->z};
	GLfloat edge1[3] = {v1->x - v2->x, v1->y - v2->y, v1->z - v2->z};
	GLfloat *n = cross(edge1, edge2);
	normal[0] = n[0];
	normal[1] = n[1];
	normal[2] = n[2];
	normal[0] = normal[0];
	GLdouble length = dotProduct(normal, normal);
	length = sqrt(length);
	normal[0] = normal[0] / (length * 5);
	normal[1] = normal[1] / (length * 5);
	normal[2] = normal[2] / (length * 5);
}
