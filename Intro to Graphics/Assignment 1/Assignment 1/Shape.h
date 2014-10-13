#ifndef SHAPE_H
#include <vector>
#include "Point.h"
#include <list>
using namespace std;

class Shape
{
public:
	int type;					//rect = 0, circ = 1
	vector<Point> vertices;		//a vector of points to easily store all the verices
	int numVertices;
	double rgb[3];				//color
	double coords[2];			//x, y coords
	double velocity[2];
	double width;
	double height;
	double radius;	
	double gravity;
	double yVel;				//y velocity
	double xVel;
	int oldKeyDir;

	/** Shape(double _rgb[3], int _type, double _width, double _height, double _radius, double _coords[2])
		Uses x, y, width, and height to create the list of vertices **/
	Shape(int*, int, double, double, double, double*);

	/** Draws the shape, called from the main programs display function. The main program shouldn't draw anything, it
		should tell everything to draw itself. **/
	void Draw(int window_w, int window_h);

	//Any changes we make to the shape(so movement)
	void Update(int window_w, int window_h, list<Shape> *shapeList, int keyDir);
};

#endif // !SHAPE_H