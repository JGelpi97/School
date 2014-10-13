/*
	Joey Gelpi - jbg29
	Assignment 3 - Transformer
*/


#ifndef SHAPE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include "Point.h"
#include "Face.h"
#include "MatrixMultiplyer.h"

#define HOUSE    0
#define CUBE     1
#define TESCUBE  2
#define CYLINDER 3
#define SPHERE   4
#define TORUS    5
#define MESH     6
#define GEODESIC 7
#define CONE	 8

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
	//0 = stack, 1 = slice
	void increaseTesselation(int type);
	//0 = stack, 1 = slice
	void decreaseTesselation(int type);
	Shape(int _type, double _maxHeight, double _ray, int _rs, int _vs);		//sphere, cyl, cone
	Shape(float r1, float r2, int _rs, int _vs);							//torus
	~Shape(void);
	vector<vector<Point>> vertices;
	GLfloat yVector[4];
	GLfloat xVector[4];
	GLfloat zVector[4];
	GLfloat position[3];
private:
	void calcSphere(double _ray, int _rs, int _vs);
	void calcCylinder(double _maxHeight, double _ray, int _rs, int _vs);	//Calculate the cylinder vertices
	void calcCone(float r, float h, int rs, int vs);
	void calcTorus(float r1, float r2, int _rs, int _vs);

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