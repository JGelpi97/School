/*
	Joey Gelpi - jbg29
	Assignment 5 - Intersect
	Visual Studio 2012
*/

#ifndef __CS1566_GLOBALS
#define __CS1566_GLOBALS

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <malloc.h>
#include <crtdbg.h>


#include <stdio.h>
#include <string.h>
#include <math.h>
// may need to change GLUT/glut.h to GL/glut.h on PC or Linux
#include <glut.h>
#include "Shape.h"
#include "Camera.h"


#ifndef min //In VC++ 2008, Ryan got a warning about min redefinition, so let's not redefine it if something else already did; sigh
#define min(a,b) ((a) < (b)? a:b)
#endif

#ifndef M_PI //In VS 2010 M_PI seems to be missing, so we will put it here
#define M_PI 3.14159265
#endif

#define FALSE 0 
#define TRUE  1

#define CUBE		1
#define HOUSE		2
#define SPHERE		3
#define CYLINDER	4
#define CONE		5
#define TORUS		6

/* define index constants into the colors array */
#define BLACK   0
#define RED     1
#define YELLOW  2
#define MAGENTA 3
#define GREEN   4
#define CYAN    5
#define BLUE    6
#define GREY    7
#define WHITE   8

#define PLAYER_MOVE_SPEED .09
#define PLAYER_TURN_SPEED 1

#define CAM_MOVE_SPEED .09
#define CAM_TURN_SPEED 2

void glut_setup(void) ;
void gl_setup(void) ;
void my_init(int argc, char **argv);
void my_setup(int argc, char **argv);
void myabort(void);
void my_display(void) ;
void my_mouse(int button, int state, int mousex, int mousey) ;
void my_reshape(int w, int h) ;
void my_keyboard_up( unsigned char key, int x, int y ) ;
void my_keyboard( unsigned char key, int x, int y ) ;
void my_idle(void) ;
void my_TimeOut(int id) ;
void mouseMove(int x, int y);

void normalize(GLfloat *p);

void lighting_setup();
void print_matrix(float my_matrix[]);

void read_spec(char *fname) ;
void parse_nums(char *buffer, GLfloat nums[]) ;
void parse_bldg(char *buffer);
void parse_light(char *buffer);

void gen_vertices(void);
void my_mult_pt(GLfloat *p);

void draw_axes( void );
#endif
