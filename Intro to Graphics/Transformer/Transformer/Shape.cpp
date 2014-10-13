/*
	Joey Gelpi - jbg29
	Assignment 3 - Transformer
*/


#include "Shape.h"

//For cylinder, sphere, cone
Shape::Shape(int _type, double _maxHeight, double _ray, int _rs, int _vs)
{
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
	yVector[0] = 0;
	yVector[1] = 1;
	yVector[2] = 0;
	yVector[3] = 1;

	xVector[0] = 1;
	xVector[1] = 0;
	xVector[2] = 0;
	xVector[3] = 1;

	zVector[0] = 0;
	zVector[1] = 0;
	zVector[2] = 1;
	zVector[3] = 1;
	renderMode = GL_LINE_LOOP;
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
}

//For torus
Shape::Shape(float r1, float r2, int _rs, int _vs)
{
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;

	yVector[0] = 0;
	yVector[1] = 1;
	yVector[2] = 0;
	yVector[3] = 1;

	xVector[0] = 1;
	xVector[1] = 0;
	xVector[2] = 0;
	xVector[3] = 1;

	zVector[0] = 0;
	zVector[1] = 0;
	zVector[2] = 1;
	zVector[3] = 1;
	renderMode = GL_LINE_LOOP;
	type = TORUS;
	calcTorus(r1, r2, _rs, _vs);
}

Shape::~Shape(void)
{

}

void Shape::draw()
{
	/*
	//Choose the list of faces you want to draw based on the shape type
	vector<Point *> *facesToDraw = NULL;
	if (type == CYLINDER)
	{
		facesToDraw = &faceCyl;
	}
	else if (type == SPHERE)
	{
		facesToDraw = &faceSph;
	}
	else if (type == CONE)
	{
		facesToDraw = &faceCone;
	}
	else if (type == TORUS)
	{
		facesToDraw = &faceTorus;
	}

	//Draw each face
	for (int i = 0; i < (*facesToDraw).size(); i+=4)
	{
		glBegin(renderMode);		
		glVertex4f((*facesToDraw)[i]->x,   (*facesToDraw)[i]->y,   (*facesToDraw)[i]->z,   (*facesToDraw)[i]->w);
		glVertex4f((*facesToDraw)[i+1]->x, (*facesToDraw)[i+1]->y, (*facesToDraw)[i+1]->z, (*facesToDraw)[i+1]->w);
		glVertex4f((*facesToDraw)[i+2]->x, (*facesToDraw)[i+2]->y, (*facesToDraw)[i+2]->z, (*facesToDraw)[i+2]->w);
		glVertex4f((*facesToDraw)[i+3]->x, (*facesToDraw)[i+3]->y, (*facesToDraw)[i+3]->z, (*facesToDraw)[i+3]->w);
		glEnd();
	}
	*/
	int c = 1;

	glColor3f(0, 1, 0);
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
	
	glColor3f(0, 1, 1);

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

			theta += ((2 * M_PI) / rs);
		}
		row++;
		height += (maxHeight /2 / (vs - 1));		//vs -1 because we need to make it go to the height
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
		//delete vNorm;
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
		//delete vNorm;
	}

	//bottom left	
	vNorm[0] = (faces[faces.size() - 1][0].normal[0] + faces[faces.size() - 1][faces[0].size() - 1].normal[0]) / 2;
	vNorm[1] = (faces[faces.size() - 1][0].normal[1] + faces[faces.size() - 1][faces[0].size() - 1].normal[1]) / 2;
	vNorm[2] = (faces[faces.size() - 1][0].normal[2] + faces[faces.size() - 1][faces[0].size() - 1].normal[2]) / 2;
	vNormals[0].push_back(vNorm);
	//delete vNorm;

	//bottom row
	for (int c = 1; c < vertices.size() - 1; c++)
	{
		GLfloat *vNorm = new GLfloat[3];
		vNorm[0] = (faces[vertices.size() - 2][c-1].normal[0] + faces[vertices.size() - 2][c].normal[0]) / 2;
		vNorm[1] = (faces[vertices.size() - 2][c-1].normal[1] + faces[vertices.size() - 2][c].normal[1]) / 2;
		vNorm[2] = (faces[vertices.size() - 2][c-1].normal[2] + faces[vertices.size() - 2][c].normal[2]) / 2;
		vNormals[vertices.size() - 1].push_back(vNorm);
		//delete vNorm;
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
			//delete vNorm;
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

			vNew = multiply(rotateMatrixZ, vNew);

			vertices[row].push_back(Point(vNew[0], vNew[1], vNew[2], vNew[3]));

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

			vNew = multiply(rotateMatrixY, vNew);

			vertices[row].push_back(Point(vNew[0], vNew[1], vNew[2], vNew[3]));

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

			vNew = multiply(translateMatrix, vNew);

			GLfloat rotateMatrixY[4][4] = 
			{
				{cos(phi),	0,	sin(phi),	0},
				{0,			1,	0,			0},
				{-sin(phi),	0,	cos(phi),	0},
				{0,			0,	0,			1}
			};

			vNew = multiply(rotateMatrixY, vNew);

			vertices[row].push_back(Point(vNew[0], vNew[1], vNew[2], vNew[3]));

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

void Shape::setRenderMode(int mode)
{
	renderMode = mode;
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
