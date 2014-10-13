/*
	Joey Gelpi - jbg29
	Assignment 5 - Intersect
	Visual Studio 2012
*/

#include "glmain.h"
#define my_assert(X,Y) ((X)?(void) 0:(printf("error:%s in %s at %d", Y, __FILE__, __LINE__), myabort()))

#define min(a,b) ((a) < (b)? a:b)
#define FALSE 0 
#define TRUE  1
#define MAX_LIGHTS  8
#define NUM_OBJECTS 1000


vector<Shape *> shapeList;
vector<Shape *> hitMarkers;
Camera *cam;
Shape *player = new Shape(CONE, 1, 1, 10, 10);

//Movement keys, used as flags so moving can be implemented in the timer
//Allows for smoother movement and moving while turning
bool wDown = false;
bool aDown = false;
bool sDown = false;
bool dDown = false;
bool rDown = false;
bool fDown = false;

bool iDown = false;
bool jDown = false;
bool kDown = false;
bool lDown = false;
bool uDown = false;
bool hDown = false;

bool turnRight = false;
bool turnLeft = false;
GLdouble turnRightLen = 0;
GLdouble turnLeftLen = 0;

int wWidth = 700;
int wHeight = 700;

typedef struct _Object {
	int sid;

	// example object storage arrays for vertex and normals
	GLfloat vertices_cube_smart[8][4];
	GLfloat normals_cube_smart[8][3];

	GLfloat shine;
	GLfloat emi[4];
	GLfloat amb[4];
	GLfloat diff[4];
	GLfloat spec[4];

	GLfloat translate[4];
	GLfloat scale[4];
	GLfloat rotate[4];

}OBJECT;

typedef struct _CAM{
	GLfloat pos[4];
	GLfloat at[4];
	GLfloat up[4];

	GLfloat dir[4];
}CAM;

typedef struct _LITE{
	GLfloat amb[4];
	GLfloat diff[4];
	GLfloat spec[4];
	GLfloat pos[4];
	GLfloat dir[3];
	GLfloat angle;
}LITE;

GLfloat colors [][3] = {
	{0.0, 0.0, 0.0},  /* black   */
	{1.0, 0.0, 0.0},  /* red     */
	{1.0, 1.0, 0.0},  /* yellow  */
	{1.0, 0.0, 1.0},  /* magenta */
	{0.0, 1.0, 0.0},  /* green   */
	{0.0, 1.0, 1.0},  /* cyan    */
	{0.0, 0.0, 1.0},  /* blue    */
	{0.5, 0.5, 0.5},  /* 50%grey */
	{1.0, 1.0, 1.0}   /* white   */
};

GLfloat vertices_axes[][4] = {
	{0.0, 0.0, 0.0, 1.0},  /* origin */ 
	{5.0, 0.0, 0.0, 1.0},  /* maxx */ 
	{0.0, 5.0, 0.0, 1.0}, /* maxy */ 
	{0.0, 0.0, 5.0, 1.0}  /* maxz */ 

};

void real_translation(Shape *po, GLfloat x, GLfloat y, GLfloat z);
void real_scaling(Shape *po, GLfloat sx, GLfloat sy, GLfloat sz);
void real_rotation(Shape *po, GLfloat deg, GLfloat x, GLfloat y, GLfloat z);

OBJECT my_objects[NUM_OBJECTS];
LITE my_lights[MAX_LIGHTS];
int num_objects;
int  num_lights;

// camera variables
CAM my_cam;

//TRUE or FALSE
int firstPersonView;

int crt_render_mode;

void myabort(void) {
	abort();
	exit(1); /* exit so g++ knows we don't return. */
} 

int main(int argc, char** argv)
{ 
	setbuf(stdout, NULL);   /* for writing to stdout asap */
	glutInit(&argc, argv);

	my_setup(argc, argv);  
	glut_setup();
	gl_setup();

	glutMainLoop();
	return(0);
}


void glut_setup (){

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);

	glutInitWindowSize(wWidth, wHeight);
	glutInitWindowPosition(20,20);
	glutCreateWindow("CS1566 Project 4");

	glutSetCursor(GLUT_CURSOR_NONE); 

	/* set up callback functions */
	glutDisplayFunc(my_display);
	glutReshapeFunc(my_reshape);
	glutMouseFunc(my_mouse);
	glutKeyboardFunc(my_keyboard);
	glutKeyboardUpFunc( my_keyboard_up );
	glutIdleFunc( my_idle );	
	glutTimerFunc(20, my_TimeOut, 0);
	glutPassiveMotionFunc(mouseMove);
	

	return;
}

void gl_setup(void) {

	// enable depth handling (z-buffer)
	glEnable(GL_DEPTH_TEST);

	// enable auto normalize
	glEnable(GL_NORMALIZE);

	// define the background color 
	glClearColor(0,0,0,1);

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;  // init modelview to identity

	// toggle to smooth shading (instead of flat)
	//glShadeModel(GL_SMOOTH); 
	
	//lighting_setup();	//I dont even know how to do lighting! and this makes everything dark as shit


	return;
}

void my_setup(int argc, char **argv){

	firstPersonView = 1;
	num_objects = num_lights = 0;

	// initialize global shape defaults and mode for drawing
	crt_render_mode = GL_POLYGON;

	real_translation(player, -5, 0, 0);	
	player->isPlayer = true;

	//If you want to allow the user to type in the spec file
	//then modify the following code.
	//Otherwise, the program will attempt to load the file as specified
	//on the command line:
	//EX: ./glmain spec3
	my_assert(argc >1, "need to supply a spec file");
	read_spec(argv[1]);

	return;
}

void parse_floats(char *buffer, GLfloat nums[]) {
	int i;
	char *ps;

	ps = strtok(buffer, " ");
	for (i=0; ps; i++) {
		nums[i] = atof(ps);
		ps = strtok(NULL, " ");
		//printf("read %f ",nums[i]);
	}

}

void parse_obj(char *buffer){
	OBJECT *po;
	char *pshape, *pshine, *pemi, *pamb, *pdiff, *pspec, *ptranslate, *pscale, *protate;
	Shape *s = NULL;


	my_assert ((num_objects < NUM_OBJECTS), "too many objects");
	po = &my_objects[num_objects++];

	pshape  = strtok(buffer, " ");
	//printf("pshape is %s\n",pshape);

	ptranslate    = strtok(NULL, "()");  strtok(NULL, "()");
	pscale        = strtok(NULL, "()");  strtok(NULL, "()"); 
	protate       = strtok(NULL, "()");  strtok(NULL, "()");  

	pshine  = strtok(NULL, "()");strtok(NULL, "()");
	//printf("pshine is %s\n",pshine);

	pemi    = strtok(NULL, "()");  strtok(NULL, "()"); 
	pamb    = strtok(NULL, "()");  strtok(NULL, "()"); 
	pdiff   = strtok(NULL, "()");  strtok(NULL, "()"); 
	pspec   = strtok(NULL, "()");  strtok(NULL, "()"); 


	po->sid  = atoi(pshape);
	po->shine = atof(pshine);

	parse_floats(ptranslate, po->translate);
	parse_floats(pscale, po->scale);
	parse_floats(protate, po->rotate);

	parse_floats(pemi, po->emi);
	parse_floats(pamb, po->amb);
	parse_floats(pdiff, po->diff);
	parse_floats(pspec, po->spec);

	//Create the shape
	switch (po->sid)
	{
		case 1: //cube
		{
			s = new Shape(CUBE, 1, 1, 1, 1);
			break;
		}
		case 2:	//house
		{
			s = new Shape(HOUSE, 1, 1, 1, 1);
			break;
		}
		case 3:
		{
			s = new Shape(SPHERE, 1, 1, 25, 25);
			break;
		}
		case 4:
		{
			s = new Shape(CYLINDER, 1, 1, 10, 10);
			break;
		}
		case 5:
		{
			s = new Shape(CONE, 1, 1, 10, 10);
			break;
		}
		case 6:	//Torus
		{
			s = new Shape(1.2, .2, 15, 40);
			break;
		}

	}
	if (s != NULL)
		s->switchRenderMode();

	// scale, rotate, translate the shape using the struct values
	real_scaling(s, po->scale[0], po->scale[1], po->scale[2]);  
	real_rotation(s, po->rotate[0], 1, 0, 0);
	real_rotation(s, po->rotate[1], 0, 1, 0);
	real_rotation(s, po->rotate[2], 0, 0, 1);
	real_translation(s, po->translate[0], po->translate[1], po->translate[2]);
	
	//Add to list
	shapeList.push_back(s);

	printf("read object\n");
}

void parse_camera(char *buffer){
	CAM *pc;
	char *ppos, *plook, *pup;

	pc = &my_cam;

	strtok(buffer, "()");
	ppos  = strtok(NULL, "()");  strtok(NULL, "()"); 
	plook  = strtok(NULL, "()");  strtok(NULL, "()"); 
	pup  = strtok(NULL, "()");  strtok(NULL, "()"); 

	parse_floats(ppos, pc->pos);
	parse_floats(plook, pc->at);
	parse_floats(pup, pc->up);

	pc->at[0] += pc->pos[0];
	pc->at[1] += pc->pos[1];
	pc->at[2] += pc->pos[2];

	pc->dir[0] = pc->at[0] - pc->pos[0];
	pc->dir[1] = pc->at[1] - pc->pos[1];
	pc->dir[2] = pc->at[2] - pc->pos[2];
	normalize(pc->dir);

	//Set up camera values
	cam = new Camera();
	cam->at[0] = pc->at[0];
	cam->at[1] = pc->at[1];
	cam->at[2] = pc->at[2];

	cam->dir[0] = pc->dir[0];
	cam->dir[1] = pc->dir[1];
	cam->dir[2] = pc->dir[2];

	cam->pos[0] = pc->pos[0];
	cam->pos[1] = pc->pos[1];
	cam->pos[2] = pc->pos[2];
	
	cam->up[0] = pc->up[0];
	cam->up[1] = pc->up[1];
	cam->up[2] = pc->up[2];

	//Create u,v,w for the first and only time. The rest of the time they just get altered
	cam->setUVW();

	printf("read camera\n");
}

void parse_light(char *buffer){
	LITE *pl;
	char *pamb, *pdiff, *pspec, *ppos, *pdir, *pang;
	my_assert ((num_lights < MAX_LIGHTS), "too many lights");
	pl = &my_lights[++num_lights];

	strtok(buffer, "()");
	pamb  = strtok(NULL, "()");  strtok(NULL, "()"); 
	pdiff = strtok(NULL, "()");  strtok(NULL, "()"); 
	pspec = strtok(NULL, "()");  strtok(NULL, "()"); 
	ppos  = strtok(NULL, "()");  strtok(NULL, "()"); 
	pdir  = strtok(NULL, "()");  strtok(NULL, "()"); 
	pang  = strtok(NULL, "()");

	parse_floats(pamb, pl->amb);
	parse_floats(pdiff, pl->diff);
	parse_floats(pspec, pl->spec);
	parse_floats(ppos, pl->pos);
	if (pdir) {
		parse_floats(pdir, pl->dir);
		pl->angle = atof(pang);
		//printf("angle %f\n", pl->angle);
	}
	else
		pl->dir[0]= pl->dir[1]= pl->dir[2] =0;
	printf("read light\n");

}

/* assuming the spec is going to be properly written
not error-checking here */
void read_spec(char *fname) {
	char buffer[300];
	FILE *fp;

	fp = fopen(fname, "r");
	my_assert(fp, "can't open spec");
	while(!feof(fp))
	{
		buffer[0] = '#';
		fgets(buffer, 300, fp);		
		//printf("read line: %s\n", buffer);
		switch (buffer[0]) 
		{
			case '#':
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			{				//You dont need seperate cases, you always just call parse_obj()
			
				parse_obj(buffer);
				break;
			}

			case 'l':
				parse_light(buffer);
				break;

			case 'c':
				parse_camera(buffer);
				break;

			default:
				break;
		}
	}
}

void lighting_setup () {
	int i;
	GLfloat globalAmb[]     = {.1, .1, .1, .1};

	// create flashlight
	GLfloat amb[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat dif[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat spec[] = {5.0, 5.0, 5.0, 1.0};

	//enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	// reflective propoerites -- global ambiant light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	// this was for the flashlights

	// glLightfv(GL_LIGHT0, GL_POSITION, my_cam.at);
	// glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, my_cam.dir);

	// glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	// glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	// glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	// glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);

	// glEnable(GL_LIGHT0);

	// setup properties of lighting
	for (i=1; i<=num_lights; i++) {
		glEnable(GL_LIGHT0+i);
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, my_lights[i].amb);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, my_lights[i].diff);
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, my_lights[i].spec);
		glLightfv(GL_LIGHT0+i, GL_POSITION, my_lights[i].pos);
		if ((my_lights[i].dir[0] > 0) ||  (my_lights[i].dir[1] > 0) ||  (my_lights[i].dir[2] > 0)) {
			glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, my_lights[i].angle);
			glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, my_lights[i].dir);
		}
	}

}

void my_reshape(int w, int h) {
	// ensure a square view port
	glViewport(0,0,min(w,h),min(w,h)) ;
	return ;
}

void real_translation(Shape *s, GLfloat x, GLfloat y, GLfloat z) 
{
	GLfloat translateMatrix[4][4] = 
	{
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1}
	};

	//Multiply the translate matrix by the objects current CTM
	GLfloat **newCtm = matrixMult4d(translateMatrix, s->ctm);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			s->ctm[i][j] = newCtm[i][j];
		}
	}

	//Multiply the translate matrix by the objects current inverse CTM
	GLfloat translateMatrixInv[4][4] = 
	{
		{1, 0, 0, -x},
		{0, 1, 0, -y},
		{0, 0, 1, -z},
		{0, 0, 0, 1}
	};
	GLfloat **newInvCtm = matrixMult4d(s->invCtm, translateMatrixInv);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			s->invCtm[i][j] = newInvCtm[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		delete[] newCtm[i];
	}
	delete[] newCtm;

	for (int i = 0; i < 4; i++)
	{
		delete[] newInvCtm[i];
	}
	delete[] newInvCtm;


	//Change offset of shape for translating back to origin
	s->pos[0] += x;
	s->pos[1] += y;
	s->pos[2] += z;

	for (int r = 0; r < s->vertices.size(); r++)
	{
		for (int c = 0; c < s->vertices[0].size(); c++)
		{
			GLfloat vertex[4] = {s->vertices[r][c].x, s->vertices[r][c].y, s->vertices[r][c].z, s->vertices[r][c].w};
			GLfloat *tNew = multiply(translateMatrix, vertex);
			s->vertices[r][c].x = tNew[0];
			s->vertices[r][c].y = tNew[1];
			s->vertices[r][c].z = tNew[2];			
			delete[] tNew;
		}
	}
}

void real_scaling(Shape *s, GLfloat sx, GLfloat sy, GLfloat sz) 
{
	GLfloat scalingMatrix[4][4] = 
	{
		{sx, 0, 0, 0},
		{0, sy, 0, 0},
		{0, 0, sz, 0},
		{0, 0, 0,  1}
	};

	//Chang CTM
	GLfloat **newCtm = matrixMult4d(scalingMatrix, s->ctm);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			s->ctm[i][j] = newCtm[i][j];
		}
	}

	//Change inverseCTM
	GLfloat scalingMatrixInv[4][4] = 
	{
		{1/sx, 0, 0, 0},
		{0, 1/sy, 0, 0},
		{0, 0, 1/sz, 0},
		{0, 0, 0,  1}
	};
	GLfloat **newInvCtm = matrixMult4d(s->invCtm, scalingMatrixInv);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			s->invCtm[i][j] = newInvCtm[i][j];
		}
	}

	//Keep track of direction inv'CTM' for moving ray
	GLfloat **newInvDir = matrixMult4d(s->dirInv, scalingMatrixInv);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			s->dirInv[i][j] = newInvDir[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		delete[] newInvDir[i];
	}
	delete[] newInvDir;

	for (int i = 0; i < 4; i++)
	{
		delete[] newCtm[i];
	}
	delete[] newCtm;

	for (int i = 0; i < 4; i++)
	{
		delete[] newInvCtm[i];
	}
	delete[] newInvCtm;

	for (int r = 0; r < s->vertices.size(); r++)
	{
		for (int c = 0; c < s->vertices[0].size(); c++)
		{
			GLfloat vertex[4] = {s->vertices[r][c].x, s->vertices[r][c].y, s->vertices[r][c].z, s->vertices[r][c].w};
			GLfloat *tNew = multiply(scalingMatrix, vertex);
			s->vertices[r][c].x = tNew[0];
			s->vertices[r][c].y = tNew[1];
			s->vertices[r][c].z = tNew[2];			
			delete[] tNew;
		}
	}
}

void real_rotation(Shape *s, GLfloat deg, GLfloat x, GLfloat y, GLfloat z) 
{
	deg = deg * (M_PI / 180);
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

	if (x == 1)
	{
		//CTM
		GLfloat **newCtm = matrixMult4d(rotateMatrixX, s->ctm);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->ctm[i][j] = newCtm[i][j];
			}
		}

		//Inverse CTM
		GLfloat rotateMatrixXInv[4][4] = 
		{
			{1,	0,			0,			0},
			{0, cos(-deg),	-sin(-deg),	0},
			{0, sin(-deg),	cos(-deg),	0},
			{0, 0,			0,			1}
		};
		GLfloat **newInvCtm = matrixMult4d(s->invCtm, rotateMatrixXInv);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->invCtm[i][j] = newInvCtm[i][j];
			}
		}

		//Inv dir for moving ray
		GLfloat **newInvDir = matrixMult4d(s->dirInv, rotateMatrixXInv);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->dirInv[i][j] = newInvDir[i][j];
			}
		}

		for (int i = 0; i < 4; i++)
		{
			delete[] newInvDir[i];
		}
		delete[] newInvDir;

		for (int i = 0; i < 4; i++)
		{
			delete[] newCtm[i];
		}
		delete[] newCtm;

		for (int i = 0; i < 4; i++)
		{
			delete[] newInvCtm[i];
		}
		delete[] newInvCtm;
	}
	else if (y == 1)
	{
		GLfloat **newCtm = matrixMult4d(rotateMatrixY, s->ctm);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->ctm[i][j] = newCtm[i][j];
			}
		}

		GLfloat rotateMatrixYInv[4][4] = 
		{
			{cos(-deg),	0,	sin(-deg),	0},
			{0,			1,	0,			0},
			{-sin(-deg), 0,	cos(-deg),	0},
			{0,			0,	0,			1}
		};
		GLfloat **newInvCtm = matrixMult4d(s->invCtm, rotateMatrixYInv);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->invCtm[i][j] = newInvCtm[i][j];
			}
		}

		GLfloat **newInvDir = matrixMult4d(s->dirInv, rotateMatrixYInv);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->dirInv[i][j] = newInvDir[i][j];
			}
		}

		for (int i = 0; i < 4; i++)
		{
			delete[] newInvDir[i];
		}
		delete[] newInvDir;

		for (int i = 0; i < 4; i++)
		{
			delete[] newCtm[i];
		}
		delete[] newCtm;

		for (int i = 0; i < 4; i++)
		{
			delete[] newInvCtm[i];
		}
		delete[] newInvCtm;
	}
	else if (z == 1)
	{
		GLfloat **newCtm = matrixMult4d(rotateMatrixZ, s->ctm);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->ctm[i][j] = newCtm[i][j];
			}
		}

		GLfloat rotateMatrixZInv[4][4] = 
		{
			{cos(-deg),	-sin(-deg),		0,	0},
			{sin(-deg),	cos(-deg),		0,	0},
			{0,				0,			1,	0},
			{0,				0,			0,	1}
		};
		GLfloat **newInvCtm = matrixMult4d(s->invCtm, rotateMatrixZInv);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->invCtm[i][j] = newInvCtm[i][j];
			}
		}

		GLfloat **newInvDir = matrixMult4d(s->dirInv, rotateMatrixZInv);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				s->dirInv[i][j] = newInvDir[i][j];
			}
		}

		for (int i = 0; i < 4; i++)
		{
			delete[] newInvDir[i];
		}
		delete[] newInvDir;

		for (int i = 0; i < 4; i++)
		{
			delete[] newCtm[i];
		}
		delete[] newCtm;

		for (int i = 0; i < 4; i++)
		{
			delete[] newInvCtm[i];
		}
		delete[] newInvCtm;
	}

	for (int r = 0; r < s->vertices.size(); r++)
	{
		for (int c = 0; c < s->vertices[0].size(); c++)
		{
			//Current vertex
			GLfloat vertex[4] = {s->vertices[r][c].x, s->vertices[r][c].y, s->vertices[r][c].z, s->vertices[r][c].w};
			GLfloat *tNew;		//New vertex position
			if (x == 1)	
			{
				tNew = multiply(rotateMatrixX, vertex);		//Get tNew
			}
			else if (y == 1)	
			{
				tNew = multiply(rotateMatrixY, vertex);
			}
			else if (z == 1)	
			{
				tNew = multiply(rotateMatrixZ, vertex);
			}

			//Update vertex with tNew after choosing which to rotate it by
			s->vertices[r][c].x = tNew[0];
			s->vertices[r][c].y = tNew[1];
			s->vertices[r][c].z = tNew[2];			
			delete[] tNew;
			//delete[] newAxis;
		}
	}
		
}

void my_keyboard( unsigned char key, int x, int y ) {

	switch( key ) 
	{
		case 'C':
		case 'c':
			cam->switchViewType();
			glutPostRedisplay();
			break;
		case '\\':		//Change render type	
			for(std::vector<Shape *>::size_type i = 0; i != shapeList.size(); i++) 
			{
				shapeList[i]->switchRenderMode();
			}
			glutPostRedisplay();
			break;


		//-------------------------Translating about local axes
		case 'w':
			wDown = true;
			break;		
		case 'a':
			aDown = true;
			break;	
		case 's':
			sDown = true;
			break;
		case 'd':
			dDown = true;
			break;
		case 'r':
			rDown = true;
			break;
		case 'f':
			fDown = true;
			break;

		case 'i':
			iDown = true;
			break;
		case 'j':
			jDown = true;
			break;
		case 'k':
			kDown = true;
			break;
		case 'l':
			lDown = true;
			break;
		case 'u':
			uDown = true;
			break;
		case 'h':
			hDown = true;
			break;

		case 'q': 
		case 'Q':
			//Delete everything on exit
			delete cam;
			delete player;
			for(std::vector<Shape *>::size_type i = 0; i != shapeList.size(); i++) 
			{
				delete shapeList[i];
			}
			for(std::vector<Shape *>::size_type i = 0; i != hitMarkers.size(); i++) 
			{
				delete hitMarkers[i];
			}
			_CrtDumpMemoryLeaks();
			exit(0) ;
			break ;	
		default: break;
	}

	return ;
}

void my_keyboard_up( unsigned char key, int x, int y ) 
{
	switch( key ) 
	{
		case 'w':
			wDown = false;
			break;
		case 'a':
			aDown = false;
			break;
		case 's':
			sDown = false;
			break;
		case 'd':
			dDown = false;
			break;
		case 'r':
			rDown = false;
			break;
		case 'f':
			fDown = false;
			break;

		case 'i':
			iDown = false;
			break;
		case 'j':
			jDown = false;
			break;
		case 'k':
			kDown = false;
			break;
		case 'l':
			lDown = false;
			break;
		case 'u':
			uDown = false;
			break;
		case 'h':
			hDown = false;
			break;
	}
}

void intersect()
{
	GLfloat p[4] = {player->pos[0], player->pos[1], player->pos[2], 1};		//never change p
	GLfloat d[4] = {player->u[0], player->u[1], player->u[2], 0};			//never change d
	GLfloat result[3];

	for(int i = 0; i < shapeList.size(); i++) 
	{
		shapeList[i]->intersect(p, d, result);
		//result[] = {# of intersections, t1, t1}

		if (result[0] > 0)		//If there was a hit - (either 1 or 2 intersections)
		{
			if (result[1] >= 0)			//Dont draw intersects behind you
			{
				//x = p + dt
				GLfloat intersect1[3] = {d[0] * result[1], d[1] * result[1], d[2] *result[1]};	//Mult d by t1
				intersect1[0] += p[0];															//add p
				intersect1[1] += p[1];
				intersect1[2] += p[2];
				hitMarkers.push_back(new Shape(SPHERE, 1, 1, 2, 2));
				real_scaling(hitMarkers[hitMarkers.size() - 1], 0.1, 0.1, 0.1);					//Shrink sphere
				real_translation(hitMarkers[hitMarkers.size() - 1], intersect1[0], intersect1[1], intersect1[2]);	//Move it to the intersect point
			}
			
			//If there is 2 intersections, figure out the second
			//If there was 1, dont figure out this one. t1 and t2 are the same if there is 1 intersection so using t1 above if fine
			if (result[0] == 2)
			{
				if (result[2] >= 0)		//Dont draw ones behind you
				{
					GLfloat intersect2[3] = {d[0] * result[2], d[1] * result[2], d[2] *result[2]};
					intersect2[0] += p[0];
					intersect2[1] += p[1];
					intersect2[2] += p[2];
					hitMarkers.push_back(new Shape(SPHERE, 1, 1, 10, 10));
					real_scaling(hitMarkers[hitMarkers.size() - 1], 0.1, 0.1, 0.1);
					real_translation(hitMarkers[hitMarkers.size() - 1], intersect2[0], intersect2[1], intersect2[2]);
				}
			}
		}		
	}

	glutPostRedisplay();		
}

void my_mouse(int button, int state, int mousex, int mousey) {

	switch( button ) {

	case GLUT_LEFT_BUTTON:
		if( state == GLUT_DOWN ) 
		{
			intersect();
		}

		if( state == GLUT_UP ) {
		}
		break ;

	case GLUT_RIGHT_BUTTON:
		if ( state == GLUT_DOWN ) {
		}

		if( state == GLUT_UP ) {
		}
		break ;
	}



	return ;
}

#pragma region Math Stuff

float dotprod(float v1[], float v2[]) {
	float tot = 0;
	int i;
	for (i=0; i<4; i++)
		tot += v1[i]*v2[i];
	return tot;
}


void normalize(GLfloat *p) { 
	double d=0.0;
	int i;
	for(i=0; i<3; i++) d+=p[i]*p[i];
	d=sqrt(d);
	if(d > 0.0) for(i=0; i<3; i++) p[i]/=d;
}


void cross(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d) { 
	d[0]=(b[1]-a[1])*(c[2]-a[2])-(b[2]-a[2])*(c[1]-a[1]);
	d[1]=(b[2]-a[2])*(c[0]-a[0])-(b[0]-a[0])*(c[2]-a[2]);
	d[2]=(b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
	normalize(d);
}

void print_matrix(float my_matrix[])
{ 
	int i, j;

	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			printf ("%f ", my_matrix[i+j*4]);
		}
		printf ("\n");
	}
	printf ("\n");
}

#pragma endregion Matrix Math Stuff

void draw_axes() {
	glLineWidth( 5.0 );

	//glDisable(GL_LIGHTING);

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

	//glEnable(GL_LIGHTING);

}

//Draw each shape in list
void draw_objects() 
{
	player->draw();
	for(int i = 0; i < shapeList.size(); i++) 
	{
		shapeList[i]->draw();
	}
	for(int i = 0; i < hitMarkers.size(); i++) 
	{
		hitMarkers[i]->draw();
	}
}

void my_display() {

	// clear all pixels, reset depth 
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );

	//Camera view
	cam->CreateView();	

	draw_axes();

	//draw the objects
	draw_objects();

	// this buffer is ready
	glutSwapBuffers();
}

void my_TimeOut(int id) 
{	
	/* Camera controls
		Controls go in here so there is no delay when pressing keys to move
		It also allows us to move forward and backwards while turning
		WASD for forward/back + strafing left/right
		R/F for up/down
	*/
	if (wDown)
	{
		cam->translate(-CAM_MOVE_SPEED, 0, 0, 1);
		glutPostRedisplay();
	}
	if (aDown)
	{
		cam->translate(-CAM_MOVE_SPEED, 1, 0, 0);
		glutPostRedisplay();
	}
	if (sDown)
	{
		cam->translate(CAM_MOVE_SPEED, 0, 0, 1);
		glutPostRedisplay();
	}
	if (dDown)
	{
		cam->translate(CAM_MOVE_SPEED, 1, 0, 0);
		glutPostRedisplay();
	}
	if (rDown)	//go up
	{
		cam->pos[1] += CAM_MOVE_SPEED;
		glutPostRedisplay();
	}
	if (fDown)	//go down
	{
		cam->pos[1] -= CAM_MOVE_SPEED;
		glutPostRedisplay();		
	}

	/* Player(cone) controls
		I/K = forward/back
		J/L = turn left/right
		U/H = up/down

		For rotation, rotate the cones uvw axes with player->rotate()
		Then translate to origin, rotate by y, translate back
	*/
	if (iDown)
	{
		player->translate(PLAYER_MOVE_SPEED, 0, 0, 1);
		glutPostRedisplay();
	}
	if (jDown)	//Rotate left
	{
		GLfloat cx = player->pos[0];
		GLfloat cy = player->pos[1];
		GLfloat cz = player->pos[2];

		player->rotate(PLAYER_TURN_SPEED, 0, 1, 0);
		real_translation(player, -cx, -cy, -cz);
		real_rotation(player, PLAYER_TURN_SPEED, 0, 1, 0);
		real_translation(player, cx, cy, cz);
		glutPostRedisplay();
	}
	if (kDown)
	{
		player->translate(-PLAYER_MOVE_SPEED, 0, 0, 1);
		glutPostRedisplay();
	}
	if (lDown)	//Rotate right
	{
		GLfloat cx = player->pos[0];
		GLfloat cy = player->pos[1];
		GLfloat cz = player->pos[2];

		player->rotate(-PLAYER_TURN_SPEED, 0, 1, 0);
		real_translation(player, -cx, -cy, -cz);
		real_rotation(player, -PLAYER_TURN_SPEED, 0, 1, 0);
		real_translation(player, cx, cy, cz);
		glutPostRedisplay();
	}
	if (uDown)	//up
	{
		player->translate(PLAYER_MOVE_SPEED, 0, 1, 0);
		glutPostRedisplay();
	}
	if (hDown)	//down
	{
		player->translate(-PLAYER_MOVE_SPEED, 0, 1, 0);
		glutPostRedisplay();
	}


	/* Camera truning */
	if (turnRight)
	{
		cam->rotate(turnRightLen * .07, 0, 1, 0);	//Scale by .07, we can't rotate by num of pixels moved degrees
		turnRightLen = 0;
		glutPostRedisplay();
		turnRight = false;
		glutWarpPointer(wWidth / 2, wHeight / 2);	//Move pointer back to middle
	}
	else if (turnLeft)
	{
		cam->rotate(-turnLeftLen * .07, 0, 1, 0);
		turnLeftLen = 0;
		glutPostRedisplay();
		turnLeft = false;
		glutWarpPointer(wWidth / 2, wHeight / 2);
	}

	glutTimerFunc(20, my_TimeOut, 0);
}

//Keep track of how far the mouse moves from the center of the screen
void mouseMove(int x, int y)
{
	int xMid = wWidth / 2;
	int yMid = wHeight / 2;
	if (x > xMid + 5)
	{
		turnRight = true;
		turnRightLen += x - (xMid + 5);
	}
	else if (x < xMid - 5)
	{
		turnLeft = true;
		turnLeftLen += (xMid - 5) - x;
	}
}

void my_idle(void) {
	//EC idea: Make the flashlight flicker a bit (random flicker strength) when the user is idle.

	return ;
}
