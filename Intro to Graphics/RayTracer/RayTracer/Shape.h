/*
	Joey Gelpi - jbg29
	Assignment 5 - Intersect
	Visual Studio 2012
*/


#ifndef SHAPE_H

#define _USE_MATH_DEFINES

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <malloc.h>
#include <crtdbg.h>

#include <math.h>
#include <vector>
#include <glut.h>
#include <iostream>
#include "Point.h"
#include "Face.h"
#include "MatrixMultiplyer.h"

#define CUBE		1
#define HOUSE		2
#define SPHERE		3
#define CYLINDER	4
#define CONE		5
#define TORUS		6

#define BLACK   0
#define RED     1
#define YELLOW  2
#define MAGENTA 3
#define GREEN   4
#define CYAN    5
#define BLUE    6
#define GREY    7
#define WHITE   8

/*
	Joey Gelpi - jbg29
	Assignment 2 - Stitcher
*/

using namespace std;

class Shape
{

public:	
	void draw();															//Determines the type of shape and calls that draw function
	void setRenderMode(int mode);
	void switchRenderMode();
	//0 = stack, 1 = slice
	void increaseTesselation(int type);
	//0 = stack, 1 = slice
	void decreaseTesselation(int type);
	void rotate(GLfloat ang, int x, int y, int z);
	void translate(GLfloat t, int x, int y, int z);
	void applyInverseCTM();
	void applyCTM();
	void intersect(GLfloat p[4], GLfloat d[4], GLfloat result[3]);
	Shape(int _type, double _maxHeight, double _ray, int _rs, int _vs);		//sphere, cyl, cone
	Shape(float r1, float r2, int _rs, int _vs);							//torus
	~Shape(void);
	vector<vector<Point>> vertices;
	GLfloat pos[3];

	GLfloat ctm[4][4];
	GLfloat invCtm[4][4];
	GLfloat dirInv[4][4];	//for multiplying dir of ray by

	GLfloat u[3];
	GLfloat v[3];
	GLfloat w[3];

	bool isPlayer;
private:
	void calcSphere(double _ray, int _rs, int _vs);
	void calcCylinder(double _maxHeight, double _ray, int _rs, int _vs);	//Calculate the cylinder vertices
	void calcCone(float r, float h, int rs, int vs);
	void calcTorus(float r1, float r2, int _rs, int _vs);
	void calcCube(int size);
	void calcHouse();

	double maxHeight, ray, radius1, radius2;
	int vs, rs, type, renderMode;
	//vector<vector<Point>> vertCyl;											//'Grid' of vertices
	vector<Point *> faceCyl;												//Pointers to a Point object defining faces. 4 consecutive points make a face.	

	//vector<vector<Point>> vertSph;
	vector<Point *> faceSph;	

	//vector<vector<Point>> vertCone;
	vector<Point *> faceCone;

	//vector<vector<Point>> vertTorus;
	vector<Point *> faceTorus;

	vector<vector<Face>> faces;
	vector<vector<GLfloat*>> vNormals;
};

#endif