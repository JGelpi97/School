/*
	Joey Gelpi - jbg29
	Assignment 4 - Camera
	Visual Studio 2012
*/

#include "Camera.h"


Camera::Camera(void)
{
	//Set defaults
	right = 5;
	left = -5;
	top = 5;
	bottom = -5;
	far = 200;
	near = 1;
	camType = PERSPECTIVE;
	fovy = 45;
	aspect = 1;
}

//Basically gluLookAt()
void Camera::CreateView()
{	
	glLoadIdentity();
	GLdouble camT[16] = {
		1.0,      0.0,       0.0,       0.0,
        0.0,       1.0,       0.0,       0.0,
        0.0,       0.0,       1.0,       0.0,
        (-pos[0]), (-pos[1]), (-pos[2]), 1.0
	};


	GLdouble camR[16] = {
		u[0], v[0], w[0], 0,
		u[1], v[1], w[1], 0,
		u[2], v[2], w[2], 0,
		0,	  0,    0,    1
	};

	glMultMatrixd(camR);
	glMultMatrixd(camT);

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity();
	width = right - left;
	height = top - bottom;

	//Decide if its ortho or persp
	if (camType == ORTHOGRAPHIC)
	{
		GLdouble camS[16] = {
			(2 / width),	0,				0,			0,
			0,				(2 / height),	0,			0,
			0,				0,				(1 / far),	0,
			0,				0,				0,			1
		};

		glMultMatrixd(camS);
	}
	else
	{
		GLfloat thetaW = aspect*fovy/180*M_PI;
		GLfloat thetaH = fovy /180*M_PI;
		GLdouble camS[16] = {
			(1 / (tan(thetaW/2) * far)),	0,				0,			0,
			0,				(1 / (tan(thetaH/2) * far)),	0,			0,
			0,				0,				(1 / far),	0,
			0,				0,				0,			1
		};

		GLfloat c = -near/far;
		GLdouble Mpp[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, (-1/(c+1)), -1,
			0, 0, (c / (c+1)), 0
		};
		glMultMatrixd(Mpp);
		glMultMatrixd(camS);
	}
	glMatrixMode(GL_MODELVIEW);

}

//Set up u,v,w for the first time
void Camera::setUVW()
{
	w[0] = -dir[0];
	w[1] = -dir[1];
	w[2] = -dir[2];
	

	normalize(w);
	GLfloat scal = ((up[0] * w[0]) + (up[1] * w[1]) + (up[2] * w[2]));
	v[0] = scal * w[0];
	v[1] = scal * w[1];
	v[2] = scal * w[2];
	v[0] = up[0] - v[0];
	v[1] = up[1] - v[1];
	v[2] = up[2] - v[2];
	normalize(v);
	GLfloat *u2 = cross(v, w);
	u[0] = u2[0];
	u[1] = u2[1];
	u[2] = u2[2];
	delete[] u2;
}

//Translate by the arg that is 1 by t units
void Camera::translate(GLfloat t, int x, int y, int z)
{
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

//Rotate by the arg that is 1 by deg
void Camera::rotate(GLfloat deg, int x, int y, int z)
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


void Camera::switchViewType()
{
	if (camType == ORTHOGRAPHIC)
		camType = PERSPECTIVE;
	else
		camType = ORTHOGRAPHIC;
}

Camera::~Camera(void)
{
}

void Camera::normalize(GLfloat *p) { 
	double d=0.0;
	int i;
	for(i=0; i<3; i++) d+=p[i]*p[i];
	d=sqrt(d);
	if(d > 0.0) for(i=0; i<3; i++) p[i]/=d;
}