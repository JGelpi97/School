//Joey Gelpi - jbg29@pitt.edu
//CS1566 - Assignment 1


#include "Main.h"
#include "Shape.h"
#include <math.h>
#include <vector>
#include <glut.h>
#include <stdio.h>
#include <list>



/* define and initialize some global variables */
double my_x = 0, my_y = 0; /* current position */
int window_w = 400, window_h = 300; /* window height and width */
double my_rect_w = 0.1, my_rect_h = 0.1; /* rectangle width and height */
list<Shape> shapeList;
int turn = 0;	//draw a square or circle
int keyDir = 1; //0 = left, 1 = nothing, 2 = right


/*Typical OpenGL/GLUT Main function */ 
int main(int argc, char **argv) { /* program arguments */

	/* initialize GLUT and OpenGL; Must be called first */
	glutInit( &argc, argv ) ;

	/* our own initializations; we'll define these setup procedures */
	glut_setup() ;  
	gl_setup() ;
	my_setup();

	/* turn control over to GLUT */
	glutMainLoop() ;

	return(0) ; /* make the compiler happy */
}



/* Typical function to set up GLUT*/
void glut_setup(void) {

	/* specify display mode -- here we ask for double buffering and RGB coloring */
	glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB);

	/* make a window of size window_w by window_h; title of "GLUT Basic Interaction" placed at top left corner */
	glutInitWindowSize(window_w, window_h);
	glutInitWindowPosition(0,0);
	glutCreateWindow("GLUT Basic Interaction");

	/*initialize typical callback functions */
	glutDisplayFunc( draw );
	glutReshapeFunc( my_reshape ); 
	glutIdleFunc( my_idle );
	glutMouseFunc( my_mouse );	
	glutKeyboardFunc( my_keyboard );	
	glutKeyboardUpFunc(keyBoard_up);
	glutTimerFunc(20, my_TimeOut, 0);/* schedule a my_TimeOut call with the ID 0 in 20 seconds from now */
	return ;
}

/* Typical function to set up OpenGL */
/* For now, ignore the contents of function */
void gl_setup(void) {

	/* specifies a background color: black in this case */
	glClearColor(0,0,0,0) ;

	/* setup for simple 2d projection */
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	/* map unit square to viewport window */
	gluOrtho2D(0.0, 1.0, 0.0, 1.0); 

	return ;
}


/* drawing function */
/* TODO: draw some other shape, use some other colors */
/* TODO part 2: draw at the location of the mouse click */
void draw(void) {
	/* clear the buffer */
	glClear(GL_COLOR_BUFFER_BIT);

	//Have every shape draw itself
	for (list<Shape>::iterator it=shapeList.begin(); it != shapeList.end(); ++it)
	{
		it->Draw(window_w, window_h);
	}

	//for (unsigned int i = 0; i < shapeList.size(); i++)
	//{
	//	shapeList[i].Draw(window_w, window_h);
	//}
	/* buffer is ready; show me the money! */

	glutSwapBuffers();

	return ;
}



/* called if mouse event (i.e., mouse click) */
/* TODO: correct my_y so we draw exactly where the mouse clicked */
/* TODO for EC: have more fun in here */
void my_mouse(int b, int s, int x, int y) {
	switch (b) 
	{            /* b indicates the button */
	case GLUT_LEFT_BUTTON:
		if (s == GLUT_DOWN)  
		{    
			int rgb[3];
			if (turn == 0)			//create square
			{			
				rgb[0] = 0;
				rgb[1] = 0;
				rgb[2] = 1;
			}
			else if (turn == 1)		//create circ
			{			
				rgb[0] = 1;
				rgb[1] = 0;
				rgb[2] = 0;
			}

			double coords[2] = {x, window_h - y};	//screen coords we clicked at

			//Add one to list
			if (shapeList.size() < 25)
			{
				shapeList.push_back(Shape(rgb, turn, 30, 30, 20, coords));
			}
			//Delete oldest shape and add new one
			else
			{
				shapeList.pop_front();
				shapeList.push_back(Shape(rgb, turn, 30, 30, 20, coords));
			}

			//Switch to drawing other shape for next click
			if (turn == 0)
				turn = 1;
			else
				turn = 0;
		}

	}
}

/* called if timer event */
/* TODO for EC: do something fun in here */
void my_TimeOut(int id) {
	
	//Update(move) all shapes
	for (list<Shape>::iterator it = shapeList.begin(); it != shapeList.end(); ++it)
	{
		it->Update(window_w, window_h, &shapeList, keyDir);
	}

	glutTimerFunc(20, my_TimeOut, 0);
	draw();
}

/* called if keyboard event (keyboard key hit) */
/* TODO for EC: do something fun in here */
void my_keyboard(unsigned char c, int x, int y) {
	//Used to start moving shapes
	switch (c) 
	{
	case 'a': {
				keyDir = 0;
			  }
			  break;
	case 'd': {
				keyDir = 2;
			  }
			  break;			  
	}
}

void keyBoard_up(unsigned char c, int x, int y)
{
	//Used to stop shapes from moving
	switch(c)
	{
	case 'a':
		{
			keyDir = 1;
			break;
		}
	case 'd':
		{
			keyDir = 1;
			break;
		}
	}
}

void my_idle(void) {
	return ;
}

void my_setup(void) {
	return;
}

/* called if resize event (i.e., when we create the window and later whenever we resize the window) */ 
void my_reshape(int w, int h) {
	/* define the viewport to be the entire window */
	glViewport (0, 0, w, h); 
	window_w = w;
	window_h = h;
	return;	
}
