/*
	Joey Gelpi - jbg29
	Assignment 5 - Intersect
	Visual Studio 2012
*/


#include "Shape.h"

//For cylinder, sphere, cone, cube, house
Shape::Shape(int _type, double _maxHeight, double _ray, int _rs, int _vs)
{
	//Identity matrix to start
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				ctm[i][j] = 1;
				invCtm[i][j] = 1;
				dirInv[i][j] = 1;
			}
			else
			{
				ctm[i][j] = 0;
				invCtm[i][j] = 0;
				dirInv[i][j] = 0;
			}
		}
	}

	pos[0] = 0; pos[1] = 0; pos[2] = 0;
	u[0] = 1; u[1] = 0; u[2] = 0;
	v[0] = 0; v[1] = 1; v[2] = 0;
	w[0] = 0; w[1] = 0; w[2] = -1;
	isPlayer = false;
	renderMode = GL_POLYGON;
	type = _type;
	if (type == SPHERE)
	{
		calcSphere(_ray, _rs, _vs);
	}
	else if (type == CYLINDER)
	{
		calcCylinder(_maxHeight, _ray, _rs, _vs);
	}	
	else if (type == CONE)
	{
		calcCone(.5, _maxHeight, _rs, _vs);
	}
	else if (type == CUBE)
	{
		calcCube(_maxHeight);
	}
	else if (type == HOUSE)
	{
		calcHouse();
	}
}

//For torus, takes 2 radii
Shape::Shape(float r1, float r2, int _rs, int _vs)
{
	//Identity matrix
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				ctm[i][j] = 1;
				invCtm[i][j] = 1;
				dirInv[i][j] = 1;
			}
			else
			{
				ctm[i][j] = 0;
				invCtm[i][j] = 0;
				dirInv[i][j] = 0;
			}
		}
	}

	pos[0] = 0; pos[1] = 0; pos[2] = 0;
	u[0] = 1; u[1] = 0; u[2] = 0;
	v[0] = 0; v[1] = 1; v[2] = 0;
	w[0] = 0; w[0] = 0; w[2] = 1;
	isPlayer = false;
	renderMode = GL_POLYGON;
	type = TORUS;
	calcTorus(r1, r2, _rs, _vs);
}

//Rotates UVW accordingly, not the actual shape
//main is in charge of translating the shape to the origin, rotating, and then translating back
void Shape::rotate(GLfloat deg, int x, int y, int z)
{
	deg = deg * (M_PI / 180);

	if (x == 1)
	{
		GLfloat rotateMatrixX[4][4] = 
		{
			{1,	0,			0,			0},
			{0, cos(deg),	-sin(deg),	0},
			{0, sin(deg),	cos(deg),	0},
			{0, 0,			0,			1}
		};
		GLfloat rrot[4][4] = {
			{u[0], u[1], u[2], 0},
			{v[0], v[1], v[2], 0},
			{w[0], w[1], w[2], 0},
			{0, 0, 0, 1},
		};
		GLfloat** newUVW =  matrixMult4d(rotateMatrixX, rrot);
		u[0] = newUVW[0][0];
		u[1] = newUVW[0][1];
		u[2] = newUVW[0][2];

		v[0] = newUVW[1][0];
		v[1] = newUVW[1][1];
		v[2] = newUVW[1][2];

		w[0] = newUVW[2][0];
		w[1] = newUVW[2][1];
		w[2] = newUVW[2][2];

		for (int i = 0; i < 4; i++)
		{
			delete[] newUVW[i];
		}
		delete[] newUVW;
	}
	else if (y == 1)
	{
		GLfloat rotateMatrixY[4][4] = 
		{
			{cos(deg),	0,	sin(deg),	0},
			{0,			1,	0,			0},
			{-sin(deg), 0,	cos(deg),	0},
			{0,			0,	0,			1}
		};
		GLfloat rrot[4][4] = {
			{u[0], u[1], u[2], 0},
			{v[0], v[1], v[2], 0},
			{w[0], w[1], w[2], 0},
			{0, 0, 0, 1},
		};
		GLfloat** newUVW =  matrixMult4d(rotateMatrixY, rrot);
		u[0] = newUVW[0][0];
		u[1] = newUVW[0][1];
		u[2] = newUVW[0][2];

		v[0] = newUVW[1][0];
		v[1] = newUVW[1][1];
		v[2] = newUVW[1][2];

		w[0] = newUVW[2][0];
		w[1] = newUVW[2][1];
		w[2] = newUVW[2][2];

		for (int i = 0; i < 4; i++)
		{
			delete[] newUVW[i];
		}
		delete[] newUVW;

	}
	else if (z == 1)
	{
		GLfloat rotateMatrixZ[4][4] = 
		{
			{cos(deg),	-sin(deg),0,	0},
			{sin(deg),	cos(deg),	0,	0},
			{0,				0,			1,	0},
			{0,				0,			0,	1}
		};	
		GLfloat rrot[4][4] = {
			{u[0], u[1], u[2], 0},
			{v[0], v[1], v[2], 0},
			{w[0], w[1], w[2], 0},
			{0, 0, 0, 1},
		};
		GLfloat** newUVW =  matrixMult4d(rotateMatrixZ, rrot);
		u[0] = newUVW[0][0];
		u[1] = newUVW[0][1];
		u[2] = newUVW[0][2];

		v[0] = newUVW[1][0];
		v[1] = newUVW[1][1];
		v[2] = newUVW[1][2];

		w[0] = newUVW[2][0];
		w[1] = newUVW[2][1];
		w[2] = newUVW[2][2];

		for (int i = 0; i < 4; i++)
		{
			delete[] newUVW[i];
		}
		delete[] newUVW;
	}
	
}

//Translates the object in local space
//Uses UVW and changes pos[] as well as actually vertices obviously
void Shape::translate(GLfloat t, int x, int y, int z)
{
	//Translate vertices
	if (x == 1)
	{

	}
	else if (y == 1)
	{
		GLfloat translateMatrix[4][4] = 
		{
			{1, 0, 0, t * v[0]},
			{0, 1, 0, t * v[1]},
			{0, 0, 1, t * v[2]},
			{0, 0, 0, 1}
		};

		for (int r = 0; r < vertices.size(); r++)
		{
			for (int c = 0; c < vertices[0].size(); c++)
			{
				GLfloat vertex[4] = {vertices[r][c].x, vertices[r][c].y, vertices[r][c].z, vertices[r][c].w};
				GLfloat *tNew = multiply(translateMatrix, vertex);
				vertices[r][c].x = tNew[0];
				vertices[r][c].y = tNew[1];
				vertices[r][c].z = tNew[2];			
				delete[] tNew;
			}
		}

	}
	else if (z == 1)
	{
		GLfloat translateMatrix[4][4] = 
		{
			{1, 0, 0, t * w[0]},
			{0, 1, 0, t * w[1]},
			{0, 0, 1, t * w[2]},
			{0, 0, 0, 1}
		};

		for (int r = 0; r < vertices.size(); r++)
		{
			for (int c = 0; c < vertices[0].size(); c++)
			{
				GLfloat vertex[4] = {vertices[r][c].x, vertices[r][c].y, vertices[r][c].z, vertices[r][c].w};
				GLfloat *tNew = multiply(translateMatrix, vertex);
				vertices[r][c].x = tNew[0];
				vertices[r][c].y = tNew[1];
				vertices[r][c].z = tNew[2];			
				delete[] tNew;
			}
		}
	}


	//Change pos[]
	bool tPos;
	if (t == 0)
		return;
	else if (t < 0)
		tPos = false;
	else if (t > 0)
		tPos = true;
	t = abs(t);
	if (x == 1)
	{
		if (tPos)
		{
			pos[0] += t * u[0];
			pos[1] += t * u[1];
			pos[2] += t * u[2];
		}
		else
		{
			pos[0] -= t * u[0];
			pos[1] -= t * u[1];
			pos[2] -= t * u[2];
		}
	}
	else if (y == 1)
	{
		if (tPos)
		{
			pos[0] += t * v[0];
			pos[1] += t * v[1];
			pos[2] += t * v[2];
		}
		else
		{
			pos[0] -= t * v[0];
			pos[1] -= t * v[1];
			pos[2] -= t * v[2];
		}
	}
	else if (z == 1)
	{
		if (tPos)
		{
			pos[0] += t * w[0];
			pos[1] += t * w[1];
			pos[2] += t * w[2];
		}
		else
		{
			pos[0] -= t * w[0];
			pos[1] -= t * w[1];
			pos[2] -= t * w[2];
		}
	}
}

//Draw all faces of the shape
//Draw special for certain shapes (house, cyl)
void Shape::draw()
{
	//Always draw ray
	if (isPlayer)
	{
		glLineWidth(5);
		glBegin(GL_LINES);
		glVertex3f(pos[0], pos[1], pos[2]);
		glVertex3f(pos[0] + (u[0] * 20), pos[1] + (u[1] * 20), pos[2] + (u[2] * 20));
		glEnd();
		glLineWidth(1);
	} 

	int c = 0;

	glColor3f(1, 0, 0);
	//draw faces
	for (int r = 0; r < faces.size(); r++)
	{
		for (int c = 0; c < faces[0].size(); c++)
		{
			glBegin(renderMode);
			glVertex4f(faces[r][c].v1->x, faces[r][c].v1->y, faces[r][c].v1->z, faces[r][c].v1->w);
			glVertex4f(faces[r][c].v2->x, faces[r][c].v2->y, faces[r][c].v2->z, faces[r][c].v2->w);
			glVertex4f(faces[r][c].v3->x, faces[r][c].v3->y, faces[r][c].v3->z, faces[r][c].v3->w);
			glVertex4f(faces[r][c].v4->x, faces[r][c].v4->y, faces[r][c].v4->z, faces[r][c].v4->w);
			glEnd();
		}
		if (c == 1)
		{
			c = 0;
			glColor3f(1, 0, 0);
		}
		else
		{
			c = 1;
			glColor3f(0, 1, 0);
		}
	}

	//Top of house - this is a bad way
	if (type == HOUSE)
	{
		glBegin(renderMode);
		glVertex4f(vertices[0][0].x, vertices[0][0].y, vertices[0][0].z, 1);
		glVertex4f(vertices[0][4].x, vertices[0][4].y, vertices[0][4].z, 1);
		glVertex4f(vertices[0][1].x, vertices[0][1].y, vertices[0][1].z, 1);
		glEnd();

		glBegin(renderMode);
		glVertex4f(vertices[0][0].x, vertices[0][0].y, vertices[0][0].z, 1);
		glVertex4f(vertices[0][4].x, vertices[0][4].y, vertices[0][4].z, 1);
		glVertex4f(vertices[0][3].x, vertices[0][3].y, vertices[0][3].z, 1);
		glEnd();

		glBegin(renderMode);
		glVertex4f(vertices[0][1].x, vertices[0][1].y, vertices[0][1].z, 1);
		glVertex4f(vertices[0][4].x, vertices[0][4].y, vertices[0][4].z, 1);
		glVertex4f(vertices[0][2].x, vertices[0][2].y, vertices[0][2].z, 1);
		glEnd();

		glBegin(renderMode);
		glVertex4f(vertices[0][2].x, vertices[0][2].y, vertices[0][2].z, 1);
		glVertex4f(vertices[0][4].x, vertices[0][4].y, vertices[0][4].z, 1);
		glVertex4f(vertices[0][3].x, vertices[0][3].y, vertices[0][3].z, 1);
		glEnd();


	}

	//Draw top and bottom of cyl
	if (type == CYLINDER)
	{
		glBegin(renderMode);
		for (int i = 0; i < vertices[0].size(); i++)
		{
			glVertex4f(vertices[0][i].x, vertices[0][i].y, vertices[0][i].z, vertices[0][i].w);
		}
		glEnd();
		glBegin(renderMode);
		for (int i = 0; i < vertices[0].size(); i++)
		{
			glVertex4f(vertices[vertices.size() - 1][i].x, vertices[vertices.size() - 1][i].y, vertices[vertices.size() - 1][i].z, vertices[vertices.size() - 1][i].w);
		}
		glEnd();

	}
	
	glColor3f(0, 1, 1);

	//Dont even read any of this
	/*
	//draw normals
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		for (int c = 0; c < vertices[0].size() - 1; c++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex4f(vertices[r][c].x, vertices[r][c].y, vertices[r][c].z, vertices[r][c].w);
			glVertex4f(vertices[r][c].x + 0.2 * vNormals[r][c][0], vertices[r][c].x + 0.2 * vNormals[r][c][1], vertices[r][c].x + 0.2 * vNormals[r][c][2], 1);
			glEnd();
		}
	}
	*/
	/*
	for (int r = 0; r < faces.size(); r++)
	{
		for (int c = 0; c < faces[0].size(); c++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex4f(faces[r][c].v2->x, faces[r][c].v2->y, faces[r][c].v2->z, faces[r][c].v2->w);
			glVertex4f(faces[r][c].v2->x + faces[r][c].normal[0], faces[r][c].v2->y + faces[r][c].normal[1], faces[r][c].v2->z + faces[r][c].normal[2], faces[r][c].v2->w);
			glEnd();
		}
	}
	*/
	
	/* Heres some face normal displaying code that doesnt work because i cant calculate face normals apparently
	for (int i = 0; i < normCyl.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		glVertex4f(normCyl[i].x, normCyl[i].y, normCyl[i].z, normCyl[i].w);
		glVertex4f(normCyl[i].x * .2, normCyl[i].y *.2, normCyl[i].z *.2, normCyl[i].w);
		glEnd();
	}
	*/

	// This code will connect all vs and rs seperatly. Only makes sense in wireframe to see your points
	/*
		for (int r = 0; r < vertCone.size(); r++)
		{
			glBegin(GL_LINE_LOOP);
			for (int c = 0; c < vertCone[0].size(); c++)
			{
				glVertex4f(vertCone[r][c].x, vertCone[r][c].y, vertCone[r][c].z, 1);
			}
			glEnd();
		}
	
		for (int c = 0; c < vertCone[0].size(); c++)
		{
			glBegin(GL_LINE_LOOP);
			for (int r = 0; r < vertCone.size(); r++)
			{
				glVertex4f(vertCone[r][c].x, vertCone[r][c].y, vertCone[r][c].z, 1);
			}
			glEnd();
		}
	*/
	
}

#pragma region Calculations

void Shape::calcCube(int size)
{
	GLfloat r = size / 2.0;
	vertices.resize(2);

	//Top 4
	vertices[0].push_back(Point(-r, r, r, 1));
	vertices[0].push_back(Point(r, r, r, 1));
	vertices[0].push_back(Point(r, r, -r, 1));
	vertices[0].push_back(Point(-r, r, -r, 1));

	//Bottom 4
	vertices[1].push_back(Point(-r, -r, r, 1));
	vertices[1].push_back(Point(r, -r, r, 1));
	vertices[1].push_back(Point(r, -r, -r, 1));
	vertices[1].push_back(Point(-r, -r, -r, 1));

	faces.resize(1);
	faces[0].push_back(Face(&vertices[0][0], &vertices[1][0], &vertices[1][1], &vertices[0][1]));
	faces[0].push_back(Face(&vertices[0][1], &vertices[1][1], &vertices[1][2], &vertices[0][2]));
	faces[0].push_back(Face(&vertices[0][2], &vertices[1][2], &vertices[1][3], &vertices[0][3]));
	faces[0].push_back(Face(&vertices[0][3], &vertices[1][3], &vertices[1][0], &vertices[0][0]));

	faces[0].push_back(Face(&vertices[0][0], &vertices[0][1], &vertices[0][2], &vertices[0][3]));	//Top
	faces[0].push_back(Face(&vertices[1][0], &vertices[1][1], &vertices[1][2], &vertices[1][3]));	//Bottom
}

void Shape::calcHouse()
{
	GLfloat r = .5;
	vertices.resize(2);

	//Top 4
	vertices[0].push_back(Point(-r, r, r, 1));
	vertices[0].push_back(Point(r, r, r, 1));
	vertices[0].push_back(Point(r, r, -r, 1));
	vertices[0].push_back(Point(-r, r, -r, 1));

	//Bottom 4
	vertices[1].push_back(Point(-r, -r, r, 1));
	vertices[1].push_back(Point(r, -r, r, 1));
	vertices[1].push_back(Point(r, -r, -r, 1));
	vertices[1].push_back(Point(-r, -r, -r, 1));

	vertices[0].push_back(Point(0, .75, 0, 1));
	vertices[1].push_back(Point(0, 0, 0, 0));		//Everything depends on the number of vertices being square

	faces.resize(1);
	faces[0].push_back(Face(&vertices[0][0], &vertices[1][0], &vertices[1][1], &vertices[0][1]));
	faces[0].push_back(Face(&vertices[0][1], &vertices[1][1], &vertices[1][2], &vertices[0][2]));
	faces[0].push_back(Face(&vertices[0][2], &vertices[1][2], &vertices[1][3], &vertices[0][3]));
	faces[0].push_back(Face(&vertices[0][3], &vertices[1][3], &vertices[1][0], &vertices[0][0]));

	faces[0].push_back(Face(&vertices[0][0], &vertices[0][1], &vertices[0][2], &vertices[0][3]));	//Top
	faces[0].push_back(Face(&vertices[1][0], &vertices[1][1], &vertices[1][2], &vertices[1][3]));	//Bottom
}

void Shape::calcCylinder(double _maxHeight, double _ray, int _rs, int _vs)
{	
	vertices.clear();
	maxHeight = _maxHeight;
	ray = _ray;
	rs = _rs;
	vs = _vs;
	vertices.resize(vs);						//number of rows of vector arrays we need is the slices tall
	GLfloat vStart[4] = {.5, -.5, .5, 1};
	int row = 0;
	double height = 0;
	double theta = 0;
	for (int i = 0; i < vs; i++)
	{
		for (int k = 0; k < rs; k++)
		{
			//Rotation matrix for a cylinder
			GLfloat rotateMatrix[4][4] = 
			{
				{cos(theta),	0,	sin(theta),	0},
				{0,				1,	0,			0},
				{-sin(theta),	0,	cos(theta),	0},
				{0,				0,	0,			1}
			};

			//Get the rotated point into rTheta
			GLfloat *vTheta = multiply(rotateMatrix, vStart);
			
			//Translate matrix for cylinder
			GLfloat translateMatrix[4][4] = 
			{
				{1, 0, 0, 0},
				{0, 1, 0, height},
				{0, 0, 1, 0},
				{0, 0, 0, 1}
			};

			//Get translated point into vNew
			GLfloat *vNew = multiply(translateMatrix, vTheta);
		
			//Add the vertex
			vertices[row].push_back(Point(vNew[0], vNew[1], vNew[2], vNew[3]));

			delete[] vTheta;
			delete[] vNew;

			theta += ((2 * M_PI) / rs);
		}
		row++;
		height += (maxHeight / (vs - 1));		//vs -1 because we need to make it go to the height
	}	

	//create faces
	faces.clear();						//clear old faces
	faces.resize(vertices.size() - 1);	//num of rows of faces(#vertices - 1)
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		for (int c = 0; c < vertices[0].size() - 1; c++)
		{			
			faces[r].push_back(Face(&vertices[r][c], &vertices[r+1][c], &vertices[r+1][c+1], &vertices[r][c+1]));
		}
	}
	
	
	//Last column
	int c = vertices[0].size() - 1;
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		faces[r].push_back(Face(&vertices[r][c], &vertices[r+1][c], &vertices[r+1][0], &vertices[r][0]));
	}
	
	//I wish this worked but it doesnt
	/*
	//calulate vNormals
	
	vNormals.clear();
	vNormals.resize(vertices.size());	//num of rows of faces(#vertices - 1)

	//first column, with the exception of the first vertex
	for (int r = 1; r < vertices.size() - 1; r++)
	{
		GLfloat *vNorm = new GLfloat[3];
		vNorm[0] = (faces[r-1][0].normal[0] + faces[r][0].normal[0] + faces[r-1][faces[0].size() - 1].normal[0] + faces[r][faces[0].size() - 1].normal[0]) / 4;
		vNorm[1] = (faces[r-1][0].normal[1] + faces[r][0].normal[1] + faces[r-1][faces[0].size() - 1].normal[1] + faces[r][faces[0].size() - 1].normal[1]) / 4;
		vNorm[2] = (faces[r-1][0].normal[2] + faces[r][0].normal[2] + faces[r-1][faces[0].size() - 1].normal[2] + faces[r][faces[0].size() - 1].normal[2]) / 4;
		vNormals[r].push_back(vNorm);
		//delete[] vNorm;
	}

	//top left
	GLfloat *vNorm = new GLfloat[3];
	vNorm[0] = (faces[0][0].normal[0] + faces[0][faces[0].size() - 1].normal[0]) / 2;
	vNorm[1] = (faces[0][0].normal[1] + faces[0][faces[0].size() - 1].normal[1]) / 2;
	vNorm[2] = (faces[0][0].normal[2] + faces[0][faces[0].size() - 1].normal[2]) / 2;
	vNormals[0].push_back(vNorm);

	//top row
	for (int c = 1; c < vertices.size() - 1; c++)
	{
		GLfloat *vNorm = new GLfloat[3];
		vNorm[0] = (faces[0][c-1].normal[0] + faces[0][c].normal[0]) / 2;
		vNorm[1] = (faces[0][c-1].normal[1] + faces[0][c].normal[1]) / 2;
		vNorm[2] = (faces[0][c-1].normal[2] + faces[0][c].normal[2]) / 2;
		vNormals[0].push_back(vNorm);
		//delete[] vNorm;
	}

	//bottom left	
	vNorm[0] = (faces[faces.size() - 1][0].normal[0] + faces[faces.size() - 1][faces[0].size() - 1].normal[0]) / 2;
	vNorm[1] = (faces[faces.size() - 1][0].normal[1] + faces[faces.size() - 1][faces[0].size() - 1].normal[1]) / 2;
	vNorm[2] = (faces[faces.size() - 1][0].normal[2] + faces[faces.size() - 1][faces[0].size() - 1].normal[2]) / 2;
	vNormals[0].push_back(vNorm);
	//delete[] vNorm;

	//bottom row
	for (int c = 1; c < vertices.size() - 1; c++)
	{
		GLfloat *vNorm = new GLfloat[3];
		vNorm[0] = (faces[vertices.size() - 2][c-1].normal[0] + faces[vertices.size() - 2][c].normal[0]) / 2;
		vNorm[1] = (faces[vertices.size() - 2][c-1].normal[1] + faces[vertices.size() - 2][c].normal[1]) / 2;
		vNorm[2] = (faces[vertices.size() - 2][c-1].normal[2] + faces[vertices.size() - 2][c].normal[2]) / 2;
		vNormals[vertices.size() - 1].push_back(vNorm);
		//delete[] vNorm;
	}

	//middle
	for (int r = 1; r < vertices.size() - 1; r++)
	{
		for (int c = 1; c < vertices[0].size() - 1; c++)
		{
			GLfloat *vNorm = new GLfloat[3];
			vNorm[0] = (faces[r-1][c-1].normal[0] + faces[r][c].normal[0] + faces[r-1][c].normal[0] + faces[r][c-1].normal[0]) / 4;
			vNorm[1] = (faces[r-1][c-1].normal[1] + faces[r][c].normal[1] + faces[r-1][c].normal[1] + faces[r][c-1].normal[1]) / 4;
			vNorm[2] = (faces[r-1][c-1].normal[2] + faces[r][c].normal[2] + faces[r-1][c].normal[2] + faces[r][c-1].normal[2]) / 4;
			vNormals[r].push_back(vNorm);
			//delete[] vNorm;
		}
	}
	*/
	
}

void Shape::calcSphere(double _ray, int _rs, int _vs)
{
	vertices.clear();
	ray = _ray;
	rs = _rs;
	vs = _vs;
	vertices.resize(vs);
	GLfloat vStart[4] = {1, 0, 0, 1};
	int row = 0;
	double height = 0;
	double theta = 0;
	double phi = 0;
	for (int i = 0; i < vs; i++)
	{
		for (int k = 0; k < rs; k++)
		{
			GLfloat rotateMatrixY[4][4] = 
			{
				{cos(phi),	0,	sin(phi),		0},
				{0,			1,	0,				0},
				{-sin(phi),	0,	cos(phi),		0},
				{0,			0,	0,				1}
			};

			GLfloat *vNew = multiply(rotateMatrixY, vStart);

			GLfloat rotateMatrixZ[4][4] = 
			{
				{cos(theta),	-sin(theta),0,	0},
				{sin(theta),	cos(theta),	0,	0},
				{0,				0,			1,	0},
				{0,				0,			0,	1}
			};

			GLfloat *vNew2 = multiply(rotateMatrixZ, vNew);

			vertices[row].push_back(Point(vNew2[0], vNew2[1], vNew2[2], vNew2[3]));

			delete[] vNew;
			delete[] vNew2;

			phi += ((2*M_PI) / (rs - 1));
		}
		theta += ((M_PI) / (vs - 1));
		phi = 0;
		row++;
	}

	//create faces
	faces.clear();						//clear old faces
	faces.resize(vertices.size() - 1);	//num of rows of faces(#vertices - 1)
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		for (int c = 0; c < vertices[0].size() - 1; c++)
		{			
			faces[r].push_back(Face(&vertices[r][c], &vertices[r+1][c], &vertices[r+1][c+1], &vertices[r][c+1]));
		}
	}
		
	//Last column
	int c = vertices[0].size() - 1;
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		faces[r].push_back(Face(&vertices[r][c], &vertices[r+1][c], &vertices[r+1][0], &vertices[r][0]));
	}
}

void Shape::calcCone(float _r, float _h, int _rs, int _vs)
{
	vertices.clear();
	radius1 = _r;
	rs = _rs;
	vs = _vs;
	vertices.resize(vs);
	maxHeight = _h;
	GLfloat vStart[4] = {0, 0, 0, 1};
	float theta = 0;
	double t = 0;
	int row = 0;
	for (int i = 0; i < vs; i++)
	{
		for (int k = 0; k < rs; k++)
		{
			GLfloat translateMatrix[4][4] = 
			{
				{1,	0,	0,	(radius1 * (1 - t))},
				{0,	1,	0,	(maxHeight - (maxHeight * t))},
				{0,	0,	1,	0},
				{0,	0,	0,	1}
			};
			GLfloat *vNew = multiply(translateMatrix, vStart);

			GLfloat rotateMatrixY[4][4] = 
			{
				{cos(theta),	0,	sin(theta),	0},
				{0,			1,	0,				0},
				{-sin(theta),	0,	cos(theta),	0},
				{0,			0,	0,				1}
			};


			GLfloat *vNew2 = multiply(rotateMatrixY, vNew);

			vertices[row].push_back(Point(vNew2[0], vNew2[1], vNew2[2], vNew2[3]));

			delete[] vNew;
			delete[] vNew2;

			theta += ((2*M_PI) / rs);
		}
		t += (maxHeight / (vs -1));
		theta = 0;
		row++;
	}

	//create faces
	faces.clear();						//clear old faces
	faces.resize(vertices.size() - 1);	//num of rows of faces(#vertices - 1)
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		for (int c = 0; c < vertices[0].size() - 1; c++)
		{		
			faces[r].push_back(Face(&vertices[r][c+1], &vertices[r+1][c+1], &vertices[r+1][c], &vertices[r][c]));
		}
	}

	//Last column
	int c = vertices[0].size() - 1;
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		faces[r].push_back(Face(&vertices[r][0], &vertices[r+1][0], &vertices[r+1][c], &vertices[r][c]));
	}

}

void Shape::calcTorus(float r1, float r2, int _rs, int _vs)
{
	vertices.clear();
	rs = _rs;
	vs = _vs;
	radius1 = r1;
	radius2 = r2;
	vertices.resize(vs + 1);
	GLfloat vStart[4] = {radius2, 0, 0, 1};
	int row = 0;
	double theta = 0;
	double phi = 0;
	for (int i = 0; i < vs + 1; i++)
	{
		for (int k = 0; k < rs; k++)
		{
			GLfloat rotateMatrixZ[4][4] = 
			{
				{cos(theta),	-sin(theta),0,	0},
				{sin(theta),	cos(theta),	0,	0},
				{0,				0,			1,	0},
				{0,				0,			0,	1}
			};

			GLfloat *vNew = multiply(rotateMatrixZ, vStart);

			GLfloat translateMatrix[4][4] = 
			{
				{1, 0, 0, radius1},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1}
			};

			GLfloat *vNew2 = multiply(translateMatrix, vNew);


			GLfloat rotateMatrixY[4][4] = 
			{
				{cos(phi),	0,	sin(phi),	0},
				{0,			1,	0,			0},
				{-sin(phi),	0,	cos(phi),	0},
				{0,			0,	0,			1}
			};

			GLfloat *vNew3 = multiply(rotateMatrixY, vNew2);

			vertices[row].push_back(Point(vNew3[0], vNew3[1], vNew3[2], vNew3[3]));

			delete[] vNew;
			delete[] vNew2;
			delete[] vNew3;

			phi += ((2 * M_PI) / rs);
		}
		phi = 0;
		theta += ((2 * M_PI) / vs);
		row++;
	}

	//create faces
	faces.clear();						//clear old faces
	faces.resize(vertices.size() - 1);	//num of rows of faces(#vertices - 1)
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		for (int c = 0; c < vertices[0].size() - 1; c++)
		{			
			faces[r].push_back(Face(&vertices[r][c], &vertices[r+1][c], &vertices[r+1][c+1], &vertices[r][c+1]));
		}
	}

	
	//Last column
	int c = vertices[0].size() - 1;
	for (int r = 0; r < vertices.size() - 1; r++)
	{
		faces[r].push_back(Face(&vertices[r][c], &vertices[r+1][c], &vertices[r+1][0], &vertices[r][0]));
	}
	
}

#pragma endregion Shape Calculations

void Shape::applyInverseCTM()
{
	for (int r = 0; r < vertices.size(); r++)
	{
		for (int c = 0; c < vertices[0].size(); c++)
		{
			GLfloat vertex[4] = {vertices[r][c].x, vertices[r][c].y, vertices[r][c].z, vertices[r][c].w};
			GLfloat *tNew = multiply(invCtm, vertex);
			vertices[r][c].x = tNew[0];
			vertices[r][c].y = tNew[1];
			vertices[r][c].z = tNew[2];			
			delete[] tNew;
		}
	}
	
}

void Shape::applyCTM()
{
	for (int r = 0; r < vertices.size(); r++)
	{
		for (int c = 0; c < vertices[0].size(); c++)
		{
			GLfloat vertex[4] = {vertices[r][c].x, vertices[r][c].y, vertices[r][c].z, vertices[r][c].w};
			GLfloat *tNew = multiply(ctm, vertex);
			vertices[r][c].x = tNew[0];
			vertices[r][c].y = tNew[1];
			vertices[r][c].z = tNew[2];			
			delete[] tNew;
		}
	}
}

void Shape::intersect(GLfloat p[4], GLfloat d[4], GLfloat result[3])
{
	if (type == SPHERE)
	{
		//Move p
		GLfloat *newP = multiply(invCtm, p);
		newP[3] = 0;

		//Change direction
		GLfloat *newDir = multiply(dirInv, d);
		newDir[3] = 0;

		double a = dotProduct(newDir, newDir);
		double b = 2 * dotProduct(newP, newDir);
		double c = dotProduct(newP, newP) - 1;

		double ttest = (b*b-4*a*c);
		if (ttest < 0)			//No hits
		{
			result[0] = 0;
		}
		else if (ttest == 0)	//1 hit
		{
			result[0] = 1;	
		}
		else if (ttest > 0)		//2 hits
		{
			result[0] = 2;
		}

		double t1 = (-b + sqrt(b*b-4*a*c)) / (2*a);
		double t2 = (-b - sqrt(b*b-4*a*c)) / (2*a);

		//Store t1 and t2
		result[1] = t1;
		result[2] = t2;

		delete[] newP;
		delete[] newDir;
	}
	else if (type == CUBE)
	{
		//Move p
		GLfloat *newP = multiply(invCtm, p);
		newP[3] = 0;

		//Change direction
		GLfloat *newDir = multiply(dirInv, d);
		newDir[3] = 0;

		//This is basically a counter, you can add up to 2 t-vals to result[]
		//result[0] = how many hits, result[1/2] = t-vals
		result[0] = 0;

		double t = 0;
		
		GLfloat inter[3];	//Intersect ray/line

		if (newDir[0] != 0)						//Cant divide by 0
		{
			//Left Sire
			t = (-0.5 - newP[0]) / newDir[0];	//t-val
			inter[0] = newDir[0] * t;			//calulate ray
			inter[1] = newDir[1] * t;
			inter[2] = newDir[2] * t;
			inter[0] += newP[0];
			inter[1] += newP[1];
			inter[2] += newP[2];
			if (inter[1] <= 0.5 && inter[1] >= -0.5 && inter[2] <= 0.5 && inter[2] >= -0.5)		//if in bounds
			{		
				if (result[0] != 2)				//If there are not already 2 t-vals
				{
					result[0] += 1;				//Incremenent how many there are
					int x = result[0];			//Get that number
					result[x] = t;				//Store t
				}
			}

			//Right Side
			t = (0.5 - newP[0]) / newDir[0];
			inter[0] = newDir[0] * t;
			inter[1] = newDir[1] * t;
			inter[2] = newDir[2] * t;
			inter[0] += newP[0];
			inter[1] += newP[1];
			inter[2] += newP[2];
			if (inter[1] <= 0.5 && inter[1] >= -0.5 && inter[2] <= 0.5 && inter[2] >= -0.5)
			{		
				if (result[0] != 2)
				{
					result[0] += 1;
					int x = result[0];
					result[x] = t;
				}
			}
		}

		if (newDir[1] != 0)
		{
			//ToP
			t = (-0.5 - newP[1]) / newDir[1];
			inter[0] = newDir[0] * t;
			inter[1] = newDir[1] * t;
			inter[2] = newDir[2] * t;
			inter[0] += newP[0];
			inter[1] += newP[1];
			inter[2] += newP[2];
			if (inter[0] <= 0.5 && inter[0] >= -0.5 && inter[2] <= 0.5 && inter[2] >= -0.5)
			{		
				if (result[0] != 2)
				{
					result[0] += 1;
					int x = result[0];
					result[x] = t;
				}
			}

			//Bottom
			t = (0.5 - newP[1]) / newDir[1];
			inter[0] = newDir[0] * t;
			inter[1] = newDir[1] * t;
			inter[2] = newDir[2] * t;
			inter[0] += newP[0];
			inter[1] += newP[1];
			inter[2] += newP[2];
			if (inter[0] <= 0.5 && inter[0] >= -0.5 && inter[2] <= 0.5 && inter[2] >= -0.5)
			{		
				if (result[0] != 2)
				{
					result[0] += 1;
					int x = result[0];
					result[x] = t;
				}
			}
		}

		if (newDir[2] != 0)
		{
			//Front
			t = (-0.5 - newP[2]) / newDir[2];
			inter[0] = newDir[0] * t;
			inter[1] = newDir[1] * t;
			inter[2] = newDir[2] * t;
			inter[0] += newP[0];
			inter[1] += newP[1];
			inter[2] += newP[2];
			if (inter[1] <= 0.5 && inter[1] >= -0.5 && inter[0] <= 0.5 && inter[0] >= -0.5)
			{		
				if (result[0] != 2)
				{
					result[0] += 1;
					int x = result[0];
					result[x] = t;
				}
			}

			//Back
			t = (0.5 - newP[2]) / newDir[2];
			inter[0] = newDir[0] * t;
			inter[1] = newDir[1] * t;
			inter[2] = newDir[2] * t;
			inter[0] += newP[0];
			inter[1] += newP[1];
			inter[2] += newP[2];
			if (inter[1] <= 0.5 && inter[1] >= -0.5 && inter[0] <= 0.5 && inter[0] >= -0.5)
			{		
				if (result[0] != 2)
				{
					result[0] += 1;
					int x = result[0];
					result[x] = t;
				}
			}
		}
		delete[] newP;
		delete[] newDir;
	}
}

void Shape::setRenderMode(int mode)
{
	renderMode = mode;
}

void Shape::switchRenderMode()
{
	if (renderMode == GL_POLYGON)
		renderMode = GL_LINE_LOOP;
	else
		renderMode = GL_POLYGON;
}

void Shape::increaseTesselation(int kind)
{	
	//0 = stack, 1 = slice
	if (kind == 0)
	{
		vs++;
	}
	else if (kind == 1)
	{
		rs++;
	}

	if (type == CYLINDER)
	{
		calcCylinder(maxHeight, ray, rs, vs);
	}
	else if (type == SPHERE)
	{
		calcSphere(ray, rs, vs);
	}
	else if (type == CONE)	
	{
		calcCone(radius1, maxHeight, rs, vs);
	}
	else if (type == TORUS)
	{
		calcTorus(radius1, radius2, rs, vs);
	}
}

void Shape::decreaseTesselation(int kind)
{
	//0 = stack, 1 = slice
	if (kind == 0)
	{
		vs--;
		if (vs < 5)
			vs = 5;
	}
	else if (kind == 1)
	{
		rs--;
		if (rs < 5)
			rs = 5;
	}
	if (type == CYLINDER)
	{
		calcCylinder(maxHeight, ray, rs, vs);
	}
	else if (type == SPHERE)
	{
		calcSphere(ray, rs, vs);
	}
	else if (type == CONE)
	{
		calcCone(radius1, maxHeight, rs, vs);
	}
	else if (type == TORUS)
	{
		calcTorus(radius1, radius2, rs, vs);
	}

}

Shape::~Shape(void)
{

}
