/*
	Joey Gelpi - jbg29
	Assignment 3 - Transformer
*/


/**********
notes:
VS users: include gl/glut.h below instead of glut.h
OSX users: include glut/glut.h below instead of glut.j
**********/

//VisS will not let through math.h constants such as M_PI w/out the line below
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>
#include <glut.h>

//#include "cs1566globals.h"
//#include "stitcher.h"
//#include "transformer.h"
#include "glmain.h"
#include "Shape.h"

double cyl_height=1;
double cyl_ray=1;
double sph_ray=1;
double x_camera = 0, y_camera = 1, z_camera = 30;
double x_look = 0, y_look = 0, z_look = 0;

bool tDown = false;
int crt_render_mode;
int crt_shape, crt_rs, crt_vs;
int crt_transform_mode;
int spin_val = SPIN_DISABLED;
GLdouble wave = 0;			//Used for head bob
bool drawFloor = true;

Shape *shapeM = new Shape(CONE, 1, 0, 10, 10);

/*******************************************************
FUNCTION: main
ARGS: argc, argv
RETURN: 0
DOES: main function (duh!); starts GL, GLU, GLUT, then loops 
********************************************************/
int main(int argc, char** argv)

{	

	glutInit(&argc, argv);
	glut_setup();
	gl_setup();
	my_setup();

	glutMainLoop();

	return(0);

}




/*******************************************************
FUNCTION: glut_setup
ARGS: none
RETURN: none
DOES: sets up GLUT; done here because we like our 'main's tidy
********************************************************/
/* This function sets up the windowing related glut calls */
void glut_setup (){

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(700,700);
	glutInitWindowPosition(20,20);
	glutCreateWindow("Transformer");


	/* set up callback functions */
	glutDisplayFunc(my_display);
	glutReshapeFunc(my_reshape);
	glutMouseFunc(my_mouse);
	glutKeyboardFunc(my_keyboard);
	glutKeyboardUpFunc( my_keyboard_up );	
	glutTimerFunc( 20, my_TimeOut, 0 );/* schedule a my_TimeOut call with the ID 0 in 20 seconds from now */
	return;

}


/*******************************************************
FUNCTION: gl_setup
ARGS: none
RETURN: none
DOES: sets up OpenGL (name starts with gl but not followed by capital letter, so it's ours)
********************************************************/
void gl_setup(void) {
	// enable depth handling (z-buffer)
	glEnable(GL_DEPTH_TEST);

	// define the background color 
	glClearColor(0,0,0,1);

	glMatrixMode(GL_PROJECTION) ;

	glLoadIdentity() ;

	gluPerspective( 20, 1.0, 15, 100.0);

	glMatrixMode(GL_MODELVIEW) ;

	glLoadIdentity() ;  // init modelview to identity

	return ;

}


/*******************************************************
FUNCTION: my_setup
ARGS: 
RETURN:
DOES: pre-computes stuff and presets some values
********************************************************/
/*TODO add make_cone, make_torus, make_your_shape etc.   */
void my_setup()
{
	crt_render_mode = GL_POLYGON;//GL_LINE_LOOP;
	crt_shape = CONE;
	crt_transform_mode = NONE;
	crt_rs = 20;
	crt_vs = 10;
	return;

}



/*******************************************************
FUNCTION: my_reshape
ARGS: new window width and height 
RETURN:
DOES: remaps viewport to the Window Manager's window
********************************************************/
void my_reshape(int w, int h) 
{
	// ensure a square view port
	glViewport(0,0,min(w,h),min(w,h)) ;
	return ;

}

/*******************************************************
FUNCTION: real_translation
ARGS: translation amount along x, y, z
RETURN:
DOES: translates shape for real
********************************************************/
void real_translation(GLfloat x, GLfloat y, GLfloat z) 
{
	//Translate the position
	shapeM->position[0] += x;
	shapeM->position[1] += y;
	shapeM->position[2] += z;

	//cout << "x=" << shapeM->position[0] << " y=" << shapeM->position[1] << " z=" << shapeM->position[2] << "\n";
	GLfloat translateMatrix[4][4] = 
	{
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1}
	};

	//Translate the axes - doesnt work
	/*
	GLfloat *axisNew = multiply(translateMatrix, shapeM->xVector);
	shapeM->xVector[0] = axisNew[0];
	shapeM->xVector[1] = axisNew[1];
	shapeM->xVector[2] = axisNew[2];

	axisNew = multiply(translateMatrix, shapeM->yVector);
	shapeM->yVector[0] = axisNew[0];
	shapeM->yVector[1] = axisNew[1];
	shapeM->yVector[2] = axisNew[2];

	axisNew = multiply(translateMatrix, shapeM->zVector);
	shapeM->zVector[0] = axisNew[0];
	shapeM->zVector[1] = axisNew[1];
	shapeM->zVector[2] = axisNew[2];
	
	delete axisNew;
	*/

	for (int r = 0; r < shapeM->vertices.size(); r++)
	{
		for (int c = 0; c < shapeM->vertices[0].size(); c++)
		{
			GLfloat vertex[4] = {shapeM->vertices[r][c].x, shapeM->vertices[r][c].y, shapeM->vertices[r][c].z, shapeM->vertices[r][c].w};
			GLfloat *tNew = multiply(translateMatrix, vertex);
			shapeM->vertices[r][c].x = tNew[0];
			shapeM->vertices[r][c].y = tNew[1];
			shapeM->vertices[r][c].z = tNew[2];			
			delete tNew;
		}
	}
}


/*******************************************************
FUNCTION: real_scaling
ARGS: scaling amount along x, y, z
RETURN:
DOES: scales shape, for real
********************************************************/
void real_scaling(GLfloat sx, GLfloat sy, GLfloat sz) 
{
	GLfloat scalingMatrix[4][4] = 
	{
		{sx, 0, 0, 0},
		{0, sy, 0, 0},
		{0, 0, sz, 0},
		{0, 0, 0,  1}
	};
	for (int r = 0; r < shapeM->vertices.size(); r++)
	{
		for (int c = 0; c < shapeM->vertices[0].size(); c++)
		{
			GLfloat vertex[4] = {shapeM->vertices[r][c].x, shapeM->vertices[r][c].y, shapeM->vertices[r][c].z, shapeM->vertices[r][c].w};
			GLfloat *tNew = multiply(scalingMatrix, vertex);
			shapeM->vertices[r][c].x = tNew[0];
			shapeM->vertices[r][c].y = tNew[1];
			shapeM->vertices[r][c].z = tNew[2];			
			delete tNew;
		}
	}
}

/*******************************************************
FUNCTION: real_rotation
ARGS: angle and axis
RETURN:
DOES: rotates shape, for real 
********************************************************/
void real_rotation(GLfloat deg, GLfloat x, GLfloat y, GLfloat z) 
{

	GLfloat rotateMatrixX[4][4] = 
	{
		{1,	0,			0,			0},
		{0, cos(deg),	-sin(deg),	0},
		{0, sin(deg),	cos(deg),	0},
		{0, 0,			0,			1}
	};
	GLfloat rotateMatrixY[4][4] = 
	{
		{cos(deg),	0,	sin(deg),	0},
		{0,			1,	0,			0},
		{-sin(deg), 0,	cos(deg),	0},
		{0,			0,	0,			1}
	};
	GLfloat rotateMatrixZ[4][4] = 
	{
		{cos(deg),	-sin(deg),0,	0},
		{sin(deg),	cos(deg),	0,	0},
		{0,				0,			1,	0},
		{0,				0,			0,	1}
	};	
	GLfloat oldX = shapeM->vertices[shapeM->vertices.size() - 1][shapeM->vertices[0].size() - 1].x;
	GLfloat oldY = shapeM->vertices[shapeM->vertices.size() - 1][shapeM->vertices[0].size() - 1].y;
	GLfloat oldZ = shapeM->vertices[shapeM->vertices.size() - 1][shapeM->vertices[0].size() - 1].z;

	for (int r = 0; r < shapeM->vertices.size(); r++)
	{
		for (int c = 0; c < shapeM->vertices[0].size(); c++)
		{
			//Current vertex
			GLfloat vertex[4] = {shapeM->vertices[r][c].x, shapeM->vertices[r][c].y, shapeM->vertices[r][c].z, shapeM->vertices[r][c].w};
			GLfloat *tNew;		//New vertex position
			GLfloat *newAxis;	//New axis position
			if (x == 1)	
			{
				tNew = multiply(rotateMatrixX, vertex);		//Get tNew

				//Rotate all axes by x
				newAxis = multiply(rotateMatrixX, shapeM->xVector);
				shapeM->xVector[0] = newAxis[0];
				shapeM->xVector[1] = newAxis[1];
				shapeM->xVector[2] = newAxis[2];

				newAxis = multiply(rotateMatrixX, shapeM->yVector);
				shapeM->yVector[0] = newAxis[0];
				shapeM->yVector[1] = newAxis[1];
				shapeM->yVector[2] = newAxis[2];

				newAxis = multiply(rotateMatrixX, shapeM->zVector);	
				shapeM->zVector[0] = newAxis[0];
				shapeM->zVector[1] = newAxis[1];
				shapeM->zVector[2] = newAxis[2];

				delete newAxis;

				//Rotate position - makes it worse
				/*
				GLfloat *posNew = multiply(rotateMatrixX, shapeM->position);
				shapeM->position[0] = posNew[0];
				shapeM->position[1] = posNew[1];
				shapeM->position[2] = posNew[2];
				delete posNew; */


			}
			else if (y == 1)	
			{
				tNew = multiply(rotateMatrixY, vertex);

				//Rotate all axes by y
				newAxis = multiply(rotateMatrixY, shapeM->xVector);
				shapeM->xVector[0] = newAxis[0];
				shapeM->xVector[1] = newAxis[1];
				shapeM->xVector[2] = newAxis[2];

				newAxis = multiply(rotateMatrixY, shapeM->yVector);
				shapeM->yVector[0] = newAxis[0];
				shapeM->yVector[1] = newAxis[1];
				shapeM->yVector[2] = newAxis[2];

				newAxis = multiply(rotateMatrixY, shapeM->zVector);
				shapeM->zVector[0] = newAxis[0];
				shapeM->zVector[1] = newAxis[1];
				shapeM->zVector[2] = newAxis[2];

				delete newAxis;

				/*
				GLfloat *posNew = multiply(rotateMatrixY, shapeM->position);
				shapeM->position[0] = posNew[0];
				shapeM->position[1] = posNew[1];
				shapeM->position[2] = posNew[2];
				delete posNew; */

			}
			else if (z == 1)	
			{
				tNew = multiply(rotateMatrixZ, vertex);

				//rotate axes by z
				newAxis = multiply(rotateMatrixZ, shapeM->yVector);
				shapeM->yVector[0] = newAxis[0];
				shapeM->yVector[1] = newAxis[1];
				shapeM->yVector[2] = newAxis[2];

				newAxis = multiply(rotateMatrixZ, shapeM->xVector);
				shapeM->xVector[0] = newAxis[0];
				shapeM->xVector[1] = newAxis[1];
				shapeM->xVector[2] = newAxis[2];

				newAxis = multiply(rotateMatrixZ, shapeM->zVector);
				shapeM->zVector[0] = newAxis[0];
				shapeM->zVector[1] = newAxis[1];
				shapeM->zVector[2] = newAxis[2];

				delete newAxis;

				/*
				GLfloat *posNew = multiply(rotateMatrixZ, shapeM->position);
				shapeM->position[0] = posNew[0];
				shapeM->position[1] = posNew[1];
				shapeM->position[2] = posNew[2];
				delete posNew; */
			}

			//Update vertex with tNew after choosing which to rotate it by
			shapeM->vertices[r][c].x = tNew[0];
			shapeM->vertices[r][c].y = tNew[1];
			shapeM->vertices[r][c].z = tNew[2];			
			delete tNew;
		}
	}
		
	shapeM->position[0] -= oldX - shapeM->vertices[shapeM->vertices.size() - 1][shapeM->vertices[0].size() - 1].x;
	shapeM->position[1] -= oldY - shapeM->vertices[shapeM->vertices.size() - 1][shapeM->vertices[0].size() - 1].y;
	shapeM->position[2] -= oldZ - shapeM->vertices[shapeM->vertices.size() - 1][shapeM->vertices[0].size() - 1].z;


	//cout << "x=" << shapeM->position[0] << " y=" << shapeM->position[1] << " z=" << shapeM->position[2] << "\n";
}

/*******************************************************
FUNCTION: real_rot_arbitrary_axis_point
ARGS: angle and arbitrary axis ax, ay, az, arbitrary point cx, cy, cz 
RETURN:
DOES: makes shape spin around arbitrary axis passing through arbitrary point
Note: try a diagonal axis going through the back bottom left corner of the house
********************************************************/
/*TODO */
void real_rot_arbitrary_axis_point(GLfloat deg, GLfloat ax, GLfloat ay, GLfloat az, GLfloat cx, GLfloat cy, GLfloat cz ) 
{
	real_translation(-cx, -cy, -cz);
	GLfloat whoa[4][4] = 
	{ 
		{pow(ax, 2) + (cos(deg) * (1 - pow(ax, 2))),			(ax * (ay * (1 - cos(deg)))) - (az * sin(deg)),			(az * (ax * (1 - cos(deg)))) + (ay * sin(deg)),	0},
		{(ax * (ay * (1 - cos(deg)))) + (az * sin(deg)),		pow(ay, 2) + (cos(deg) * (1 - pow(ay, 2))),				(ay * (az * (1 - cos(deg)))) - (ax * sin(deg)),	0},
		{(az * (ax * (1 - cos(deg)))) - (ay * sin(deg)),		(ay * (az * (1 - cos(deg)))) + (ax * sin(deg)),			pow(az, 2) + (cos(deg) * (1 - pow(az, 2))),		0}, 
		{0,														0,														0,												1}, 
	};

	//Just trying random things, no idea what to do
	/*
	GLfloat *posNew = multiply(whoa, shapeM->position);
	shapeM->position[0] = posNew[0];
	shapeM->position[1] = posNew[1];
	shapeM->position[2] = posNew[2];
	delete posNew;

	GLfloat *axisNew = multiply(whoa, shapeM->xVector);
	shapeM->xVector[0] = axisNew[0];
	shapeM->xVector[1] = axisNew[1];
	shapeM->xVector[2] = axisNew[2];

	axisNew = multiply(whoa, shapeM->yVector);
	shapeM->yVector[0] = axisNew[0];
	shapeM->yVector[1] = axisNew[1];
	shapeM->yVector[2] = axisNew[2];

	axisNew = multiply(whoa, shapeM->zVector);
	shapeM->zVector[0] = axisNew[0];
	shapeM->zVector[1] = axisNew[1];
	shapeM->zVector[2] = axisNew[2];

	delete axisNew;
	*/

	for (int r = 0; r < shapeM->vertices.size(); r++)
	{
		for (int c = 0; c < shapeM->vertices[0].size(); c++)
		{
			GLfloat vertex[4] = {shapeM->vertices[r][c].x, shapeM->vertices[r][c].y, shapeM->vertices[r][c].z, shapeM->vertices[r][c].w};
			GLfloat *tNew = multiply(whoa, vertex);
			shapeM->vertices[r][c].x = tNew[0];
			shapeM->vertices[r][c].y = tNew[1];
			shapeM->vertices[r][c].z = tNew[2];			
			delete tNew;
		}
	}
	real_translation(cx, cy, cz);
}

/*******************************************************
FUNCTION: real_rev
ARGS: angle and axis x, y, z 
RETURN:
DOES: makes shape spin around its vertical axis
Note: 'vertical' axis means house keeps revolving around its roof-top
base-center axis, torus spins around axis passing through hole, 
sphere spins around North-South axis etc *at all times*.
********************************************************/
/*TODO */
void real_rev(GLfloat deg, GLfloat x, GLfloat y, GLfloat z) 
{
	real_rot_arbitrary_axis_point(deg, shapeM->yVector[0], shapeM->yVector[1], shapeM->yVector[2], shapeM->position[0], shapeM->position[1], shapeM->position[2]);
}

/*******************************************************
FUNCTION: reset
ARGS: 
RETURN:
DOES: resets shape and camera
********************************************************/
/*TODO reset shape post real transformations, as well etc*/
void reset()
{
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;
	x_camera = 0; y_camera = 1; z_camera =30;
	delete shapeM;
	shapeM = new Shape(CONE, 1, 0, 10, 10);

}


/*******************************************************
FUNCTION: my_keyboard
ARGS: key id, x, y
RETURN:
DOES: handles keyboard events
********************************************************/
/*TODO: expand to add hmwk 3 keyboard events */
void my_keyboard( unsigned char key, int x, int y ) {
	switch( key ) 
	{
		case 'O':
		case 'o': 
		{
			reset();
			glutPostRedisplay(); 
		}; break;

		//Transformations
		case 'y': 
		{
			switch( crt_transform_mode) 
			{
				case TRANSLATION_MODE: real_translation(0, -.1, 0); break;
				case ROTATION_MODE: real_rotation(-.1, 0, 1, 0); break;	
				case SCALING_MODE: real_scaling(1, .9, 1); break;
			}
			glutPostRedisplay(); 
		}; break;
		case 'Y': 
		{
			switch( crt_transform_mode) 
			{
				case TRANSLATION_MODE: real_translation(0, .1, 0); break;
				case ROTATION_MODE: real_rotation(.1, 0, 1, 0); break;
				case SCALING_MODE: real_scaling(1, 1.1, 1); break;
			}
			glutPostRedisplay(); 
		}; break;

		case 'x':
		{
			switch( crt_transform_mode) 
			{
				case TRANSLATION_MODE: real_translation(-.1, 0, 0); break;
				case ROTATION_MODE: real_rotation(-.1, 1, 0, 0); break;
				case SCALING_MODE: real_scaling(.9, 1, 1); break;
			}
			glutPostRedisplay(); 
		}; break;
		case 'X':
		{
			switch( crt_transform_mode) 
			{
				case TRANSLATION_MODE: real_translation(.1, 0, 0); break;
				case ROTATION_MODE: real_rotation(.1, 1, 0, 0); break;
				case SCALING_MODE: real_scaling(1.1, 1, 1); break;
			}
			glutPostRedisplay(); 
		}; break;

		case 'z':
		{
			switch( crt_transform_mode) 
			{
				case TRANSLATION_MODE: real_translation(0, 0, -.1); break;
				case ROTATION_MODE: real_rotation(-.1, 0, 0, 1); break;
				case SCALING_MODE: real_scaling(1, 1, .9); break;
			}
			glutPostRedisplay(); 
		}; break;
		case 'Z':
		{
			switch( crt_transform_mode) 
			{
				case TRANSLATION_MODE: real_translation(0, 0, .1); break;
				case ROTATION_MODE: real_rotation(.1, 0 ,0, 1); break;
				case SCALING_MODE: real_scaling(1, 1, 1.1); break;
			}
			glutPostRedisplay(); 
		}; break;			
		
		//Change shapes
		case '1': 
		{
			crt_shape = CONE;
			delete shapeM;
			x_camera = 0; y_camera = 1; z_camera =30;
			shapeM = new Shape(CONE, 1, 0, 10, 10);
			glutPostRedisplay();
		}; break;
		case '2':
		{
			crt_shape = TORUS;
			delete shapeM;
			x_camera = 0; y_camera = 1; z_camera =30;
			shapeM = new Shape(1.2, .2, 15, 30);
			glutPostRedisplay();
		}; break;
		case '0': 
		{
			crt_shape = HOUSE;
			x_camera = 0; y_camera = 1; z_camera =30;
			glutPostRedisplay();
		}; break;
		case '3':
		{
			crt_shape = CYLINDER;
			delete shapeM;
			x_camera = 0; y_camera = 1; z_camera =30;
			shapeM = new Shape(CYLINDER, 2, 0, 10, 10);
			glutPostRedisplay();
		}; break;
		case '4': 
		{
			crt_shape = SPHERE;
			delete shapeM;
			x_camera = 0; y_camera = 1; z_camera =30;
			shapeM = new Shape(SPHERE, 2, 0, 25, 25);
			glutPostRedisplay();
		}; break;

		//Change mode
		case 'S':
		case 's': 
		{
			crt_transform_mode = SCALING_MODE;
		}; break;

		case 'T':
		case 't': 
		{
			crt_transform_mode = TRANSLATION_MODE;
		}; break;

		case 'R':
		case 'r': 
		{
			crt_transform_mode = ROTATION_MODE;
		}; break;

		//Change camera
		//Increase/decrease y based on a shortened lowered sine wave
		case 'n':
		{
			z_camera -= .5;
			y_camera += .2 * sin(2 * wave);
			wave += .2;
			if (.2 * sin(2 * wave) > 3.14)
				wave = 0;
			glutPostRedisplay();
		}; break;
		case 'N':
		{
			z_camera += .5;
			y_camera += .2 * sin(2 * wave);
			wave += .2;
			if (.2 * sin(2 * wave) > 3.14)
				wave = 0;
			glutPostRedisplay();
		}; break;
			
		case 'p':
		{
			x_camera -= .5;
			glutPostRedisplay();
		}; break;
		case 'P':
		{
			x_camera += .5;
			glutPostRedisplay();
		}; break;

		case 'v':
		{
			y_camera -= .5;
			glutPostRedisplay();
		}; break;
		case 'V':
		{
			y_camera += .5;
			glutPostRedisplay();
		}; break;		

		case 'D':
		case 'd':
		{
			drawFloor = !drawFloor;	
			glutPostRedisplay();
		}; break;

		case 'q': 
		case 'Q':

			exit(0) ;

			break ;	

		default: break;

	}


	return ;

}

/*******************************************************
FUNCTION: my_keyboard_up
ARGS: key id, x, y
RETURN:
DOES: handles keyboard up events
********************************************************/
void my_keyboard_up( unsigned char key, int x, int y ) 
{
	switch( key ) 
	{
		case 'T': crt_transform_mode = NONE; break;
		case 't': crt_transform_mode = NONE; break;
		{
			crt_transform_mode = NONE; break;
		}
	}
}

/*******************************************************
FUNCTION: my_mouse
ARGS: button, state, x, y
RETURN:
DOES: handles mouse events
********************************************************/

void my_mouse(int button, int state, int mousex, int mousey) 
{
	switch( button ) 
	{	
		case GLUT_LEFT_BUTTON:
		{
			if( state == GLUT_DOWN ) 
			{
				if(spin_val == SPIN_ENABLED) spin_val = SPIN_DISABLED;
				else spin_val = SPIN_ENABLED;
				glutPostRedisplay();
			}

			if( state == GLUT_UP ) 
			{
			}
			break ;
		}
		case GLUT_RIGHT_BUTTON:
		{
			if( state == GLUT_DOWN ) 
			{
				//Change shape render mode
				if(crt_render_mode == GL_LINE_LOOP)
				{
					crt_render_mode = GL_POLYGON;
					shapeM->setRenderMode(GL_POLYGON);
				}
				else 
				{
					crt_render_mode = GL_LINE_LOOP;
					shapeM->setRenderMode(GL_LINE_LOOP);
				}
				glutPostRedisplay();
			}
			if( state == GLUT_UP ) 
			{
			}
			break ;
		}
	}

	return ;

}


/***********************************
FUNCTION: draw_quad 
ARGS: - a vertex array
- 4 indices into the vertex array defining a quad face
- an index into the color array.
RETURN: none
DOES:  helper drawing function; draws one quad. 
For the normal to work out, follow left-hand-rule (i.e., counter clock
wise) 
*************************************/
void draw_quad(GLfloat vertices[][4], int iv1, int iv2, int iv3, int iv4, int ic) {
	glBegin(crt_render_mode); 
	{
		glColor3fv(colors[ic]);
		/*note the explicit use of homogeneous coords below: glVertex4f*/
		glVertex4fv(vertices[iv1]);
		glVertex4fv(vertices[iv2]);
		glVertex4fv(vertices[iv3]);
		glVertex4fv(vertices[iv4]);
	}
	glEnd();
}

/***********************************
FUNCTION: draw_param_quad 
ARGS: - a 50 by 50 by 4 vertex array
- 2 indices into the vertex array defining the top left of a quad face
- an index into the color array.
RETURN: none
DOES:  helper drawing function; draws one quad. 
*************************************/
void draw_param_quad(GLfloat vertices[][50][4], int line, int col, int ic) {
	glBegin(crt_render_mode); 
	{
		glColor3fv(colors[ic]);
		/*note the explicit use of homogeneous coords below: glVertex4f*/
		glVertex4fv(vertices[line][col]);
		glVertex4fv(vertices[line+1][col]);
		glVertex4fv(vertices[line+1][col+1]);
		glVertex4fv(vertices[line][col+1]);
	}
	glEnd();
}

/***********************************
FUNCTION: draw_triangle 
ARGS: - a vertex array
- 3 indices into the vertex array defining a triangular face
- an index into the color array.
RETURN: none
DOES:  helper drawing function; draws one triangle. 
For the normal to work out, follow left-hand-rule (i.e., ccw)
*************************************/
void draw_triangle(GLfloat vertices[][4], int iv1, int iv2, int iv3, int ic) {
	glBegin(crt_render_mode); 
	{
		glColor3fv(colors[ic]);
		/*note the explicit use of homogeneous coords below: glVertex4f*/
		glVertex4fv(vertices[iv1]);
		glVertex4fv(vertices[iv2]);
		glVertex4fv(vertices[iv3]);
	}
	glEnd();
}

/***********************************
FUNCTION: draw_house 
ARGS: none
RETURN: none
DOES: helper build function; 
defines a hollow cube with a bottomless pyramid as rooftop
************************************/
void draw_house()
{
	draw_triangle(vertices_house,0,1,2,RED);
	draw_triangle(vertices_house,0,2,3,GREEN);
	draw_triangle(vertices_house,0,3,4,WHITE);
	draw_triangle(vertices_house,0,4,1,GREY);


	draw_quad(vertices_house,2,1,5,6, BLUE);
	draw_triangle(vertices_house,2,6,3, CYAN);
	draw_triangle(vertices_house,3,6,7, CYAN);
	draw_triangle(vertices_house,3,7,8, YELLOW);
	draw_triangle(vertices_house,8,3,4, YELLOW);
	draw_triangle(vertices_house,4,8,1, MAGENTA);
	draw_triangle(vertices_house,1,8,5, MAGENTA);

}

/***********************************
FUNCTION: draw_axes
ARGS: none
RETURN: none
DOES: draws main X, Y, Z axes
************************************/
void draw_axes( void ) {
	glLineWidth( 5.0 );
	glBegin(GL_LINES); 
	{
		glColor3fv(colors[1]);
		glVertex4fv(vertices_axes[0]);
		glVertex4fv(vertices_axes[1]);

		glColor3fv(colors[4]);
		glVertex4fv(vertices_axes[0]);
		glVertex4fv(vertices_axes[2]);

		glColor3fv(colors[6]);
		glVertex4fv(vertices_axes[0]);
		glVertex4fv(vertices_axes[3]);
	}
	glEnd();
	glLineWidth( 1.0 );

}

/***********************************
FUNCTION: draw_object 
ARGS: shape to create (HOUSE, CUBE, CYLINDER, etc)
RETURN: none
DOES: draws an object from triangles
************************************/
/*TODO: expand according to assignment 3 specs*/
void draw_object(int shape) 
{

	switch(shape){
	case HOUSE: draw_house(); break;

	default: shapeM->draw();
	}

}


/***********************************
FUNCTION: my_display
ARGS: none
RETURN: none
DOES: main drawing function
************************************/
/*TODO add on*/
void my_display()
{

	// clear all pixels, reset depth 
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );

	// init to identity 

	glLoadIdentity() ;

	gluLookAt(x_camera, y_camera, z_camera,  // x,y,z coord of the camera 
		x_look, y_look, z_look,  // x,y,z LookAt
		0.0, 1.0, 0.0); // the direction of Up (default is y-axis)

	//Draws a floor to help with head bob illusion
	if (drawFloor)
	{
		glColor3fv(colors[WHITE]);
		glBegin(GL_POLYGON);
		glVertex4f(-30, -3, -30, 1);
		glVertex4f(-30, -3, 30, 1);
		glVertex4f(30, -3, 30, 1);
		glVertex4f(30, -3, -30, 1);
		glEnd();
	}

	draw_axes();

	draw_object(crt_shape);

	// this buffer is ready

	glutSwapBuffers();

}

/***********************************
FUNCTION: my_TimeOut
ARGS: timer id
RETURN: none
DOES: handles "timer" events
************************************/
void my_TimeOut(int id) 
{
	/* spin counter-clockwise*/
	/* schedule next timer event, 0.2 secs from now */ 
	if(spin_val == SPIN_ENABLED)
	{
		if (crt_shape != HOUSE)
		{
			real_rev(.05,0,1,0);
		}

		glutPostRedisplay();

	};
	glutTimerFunc(40, my_TimeOut, 0);

	return ;
}


/***********************************
FUNCTION: my_idle
ARGS: none
RETURN: none
DOES: handles the "idle" event
************************************/
void my_idle(void) 
{
}
