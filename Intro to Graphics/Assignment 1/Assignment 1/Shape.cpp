#include "shape.h"
#include <vector>
#include <glut.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <list>

/**
	int type; //rect, circ
	std::vector<Point> vertices;
	int numVertices;
	double rgb[3];
	double coords[2];
	double velocity[2];
	double width;
	double height;
	double radius;
	Shape(double*, int, double, double, double, double*);
	void Draw(int, int);
**/

Shape::Shape(int _rgb[3], int _type, double _width, double _height, double _radius, double _coords[2])
{	
	rgb[0] = _rgb[0];
	rgb[1] = _rgb[1];
	rgb[2] = _rgb[2];
	type = _type;
	width = _width;
	height = _height;
	coords[0] = _coords[0];
	coords[1] = _coords[1];
	radius = _radius;
	gravity = .5;
	yVel = 0;
	xVel = 0;
	oldKeyDir = 1;
	
	//Create the 4 verts of a rect
	if (type == 0)
	{		
		numVertices = 4;
		vertices.push_back(Point(coords[0], coords[1]));
		vertices.push_back(Point(coords[0], coords[1] + height));
		vertices.push_back(Point(coords[0] + width, coords[1] + height));		
		vertices.push_back(Point(coords[0] + width, coords[1]));
	}
	//Create a bunch of points around a circle and draw a polygon with them
	else if (type == 1)
	{
		float circX, circY;
		float angleRads = 2 * M_PI;
		for (; angleRads >=0; angleRads -= .1)
		{
			circX = radius * cos(angleRads) + coords[0];
			circY = radius * sin(angleRads) + coords[1];
			vertices.push_back(Point(circX, circY));
		}
		//set coords of circle to be in lower left corner
		coords[0] -= radius;
		coords[1] -= radius;
	}
}

void Shape::Draw(int window_w, int window_h)
{
	//Draw vertices for rect
	if (type == 0)
	{	
		glColor3f(rgb[0], rgb[1], rgb[2]);
		glBegin(GL_POLYGON);
			glVertex2f(vertices[0].x / window_w, vertices[0].y / window_h);
			glVertex2f(vertices[1].x / window_w, vertices[1].y / window_h);
			glVertex2f(vertices[2].x / window_w, vertices[2].y / window_h);
			glVertex2f(vertices[3].x / window_w, vertices[3].y / window_h);
		glEnd();
		
		//glRectf(coords[0]/window_w, coords[1]/window_h, coords[0]/window_w + width, coords[1]/window_h + height);  So much easier
	}
	//Draw vertices for circ
	else if (type == 1)
	{
		glColor3f(rgb[0], rgb[1], rgb[2]);
		glBegin(GL_POLYGON);
			for (int i = 0; i < vertices.size() - 1; i++)
			{
				glVertex2f(vertices[i].x / window_w, vertices[i].y / window_h);
			}
			
		glEnd();
	}
}

void Shape::Update(int window_w, int window_h, list<Shape> *shapeList, int keyDir)
{	
	yVel += gravity;		//accelerate/slow down if going up

	//Check direction user wants shapes to go
	//Accelerate if key still down or stop
	if (keyDir == 0 && oldKeyDir == 0)
	{
		xVel += -.1;
	}
	else if (keyDir == 0 && oldKeyDir != 0)
	{
		xVel = -.5;
	}
	else if (keyDir == 2 && oldKeyDir == 2)
	{
		xVel += .1;
	}
	else if (keyDir == 2 && oldKeyDir != 2)
	{
		xVel = .5;
	}
	else if (keyDir == 1)
	{
		xVel = 0;
	}
	oldKeyDir = keyDir;
	//Move all vertices by yVel
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].y -= yVel;
		if (coords[1] != 0)		//Dont move in x dir if on ground
			vertices[i].x += xVel;
	}
	coords[1] -= yVel;
	if (coords[1] <= 0)
	{		
		yVel = -yVel + .9;								//flip it and slow it down
		if (coords[1] < 0)								//if the shape is below the line
		{
			for (int i = 0; i < vertices.size(); i++)
			{
				vertices[i].y -= coords[1];				//move all vertices up bythe distance below the line
			}
			coords[1] = 0;								//set position to line
		}
	}

	//Collision code that I dont have time to fix
	/**
	for (list<Shape>::iterator it = shapeList->begin(); it != shapeList->end(); ++it)
	{
		if (yVel < 0)		//if object is moving up
		{
			if ((it->coords[0] != coords[0]) && (it->coords[1] != coords[1]) && ((coords[1] + height) > it->coords[1]) && (coords[0] < (it->coords[0] + it->width)) && ((coords[0] + width) > it->coords[0]))
			{
				yVel = -yVel;
				it->yVel = -(it->yVel);
				for (int i = 0; i < vertices.size(); i++)
				{
					vertices[i].y -= abs((coords[1] + height) - it->coords[1]);
				}
				coords[1] -= abs((coords[1] + height) - it->coords[1]);
				break;
			}
		}
	} **/
	
}

