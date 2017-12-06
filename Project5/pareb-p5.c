/***************************************************************
*                                                              *
*                           Project 4                          *
*                          Brett Pare                          *
*                                                              *
*              Use shading on a 3D spaceship model             *
*                                                              *
****************************************************************/

#include <GL/glut.h>
#include <stdio.h>
#include "SOIL.h"
#include <time.h>

#define CAMERA_DISTANCE 2000

long Timer, ResetTimer;
float y_rot = 0.0;
float z_rot = 0.0;
float cam_zoom = 2.0;
int mousePressed = 0;
int startXPos = 0;
float startRot = 0.0;
int currentMouseXPos = 0.0;
int theSunLight = 1;
int satelliteLight = 1;
int ringLights = 1;
int spotLight = 0;

GLfloat brass[3][4] = {{.329412, .223529, .027451, 1.0}, {.780392, .568627, .113725, 1.0}, {.992157, .941176, .807843, 1.0}};
GLfloat steel[3][4] = {{0.1, 0.1, 0.1, 1.0}, {.55, .55, .55, 1.0}, {.85, .85, .85, 1.0}};
GLfloat chrome[3][4] = {{0.25, 0.25, 0.25, 1.0}, {.90, .90, .90, 1.0}, {.95, .95, .95, 1.0}};
GLfloat obsidian[3][4] = {{.05375, 0.05, 0.06625, 0.82}, {.18275, .17, .22525, 0.82}, {.332741, .328634, .346435, 0.82}};
GLfloat gold[3][4] = {{0.24725, 0.19995, 0.0745, 1.0}, {.75164, .60648, .22648, 1.0}, {.628281, .555802, .366065, 1.0}};
GLfloat glass[3][4] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
GLfloat blackColor[] = {0.0, 0.0, 0.0, 1.0};

GLfloat satellitePos[] = {-130, -60.0, 965.0, 1.0};
GLfloat ringLightPos1[] = {-270.0, -120.0, 155.0, 1.0};
GLfloat ringLightPos2[] = {-170.0, -240.0, 155.0, 1.0};
GLfloat spotPos[] = {1000.0, 1000.0, 1000.0, 1.0};
GLfloat spotDirection[] = {-1.0, -1.0, -1.0};

GLUquadricObj* quad;

GLuint thrusterFrontTex;
GLuint hexagonTex;

/*****************************************************************
*                               init                             *
*****************************************************************/
//Purpose: Initializes clear color and other options, including light
void init(void)
{
	glClearColor (28.0/255.0, 114.0/255.0, 223.0/255.0, 0.0);

	GLfloat ambCol1[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat diffCol1[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat specCol1[] = {1.0, 1.0, 1.0, 1.0};

	GLfloat diffCol2[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat specCol2[] = {0.2, 0.2, 0.2, 1.0};

	GLfloat amb[] = {0.3, 0.3, 0.3, 1.0};

	glShadeModel(GL_SMOOTH);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambCol1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffCol1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specCol1);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambCol1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffCol2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specCol2);
	glLightfv(GL_LIGHT1, GL_POSITION, satellitePos);

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambCol1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffCol2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specCol2);
	glLightfv(GL_LIGHT2, GL_POSITION, ringLightPos1);

	glLightfv(GL_LIGHT3, GL_AMBIENT, ambCol1);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffCol2);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specCol2);
	glLightfv(GL_LIGHT3, GL_POSITION, ringLightPos2);

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambCol1);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffCol1);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specCol1);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 5.0);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 100.0);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	//glEnable(GL_LIGHT4);

	quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	gluQuadricTexture(quad, GLU_TRUE);
}

/*****************************************************************
*                            reshape                             *
*****************************************************************/
//Purpose: Reshape the window
void reshape(int w, int h)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*****************************************************************
*                            mainMenu                            *
*****************************************************************/
//Purpose: Menu handler. Controls showing of the help menu, resetting the program, and exiting the program
void mainMenu(int selection)
{
	switch(selection)
	{
		
		case 1:
			y_rot = 0.0;
			z_rot = 0.0;
			cam_zoom = 1.0;
			spotDirection[0] = -1.0;
			spotDirection[1] = -1.0;
			spotDirection[2] = -1.0;
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHT2);
			glEnable(GL_LIGHT3);
			glDisable(GL_LIGHT4);
			theSunLight = 1;
			satelliteLight = 1;
			ringLights = 1;
			spotLight = 0;
			glClearColor (28.0/255.0, 114.0/255.0, 223.0/255.0, 0.0);
			break;

		case 3:
			exit(-1);
			break;

		default:
			break;
	}

	glutPostRedisplay();
}

/*****************************************************************
*                          LightOptions                          *
*****************************************************************/
//Purpose: Allows toggling of lights
void LightOptions(int selection)
{
	switch(selection)
	{
		case 1:
			if(theSunLight == 0)
			{
				theSunLight = 1;
				glClearColor (28.0/255.0, 114.0/255.0, 223.0/255.0, 0.0);
				glEnable(GL_LIGHT0);
			}
			else
			{
				theSunLight = 0;
				glClearColor (0.0, 0.0, 0.0, 0.0);
				glDisable(GL_LIGHT0);
			}
			break;

		case 2:
			if(satelliteLight == 0)
			{
				satelliteLight = 1;
				glEnable(GL_LIGHT1);
			}
			else
			{
				satelliteLight = 0;
				glDisable(GL_LIGHT1);
			}
			break;

		case 3:
			if(ringLights == 0)
			{
				ringLights = 1;
				glEnable(GL_LIGHT2);
				glEnable(GL_LIGHT3);
			}
			else
			{
				ringLights = 0;
				glDisable(GL_LIGHT2);
				glDisable(GL_LIGHT3);
			}
			break;

		case 4:
			if(spotLight == 0)
			{
				spotLight = 1;
				glEnable(GL_LIGHT4);
			}
			else
			{
				spotLight = 0;
				glDisable(GL_LIGHT4);
			}
			break;

		default:
			break;
	}

	glutPostRedisplay();
}

/*****************************************************************
*                             mouse                              *
*****************************************************************/
//Purpose: Gets rid of the help menu that pops up
void mouse (int button, int state, GLint x, GLint y)
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mousePressed = 1;
		startXPos = x;
		startRot = y_rot;
		currentMouseXPos = x;
	}
	if(state == GLUT_UP && button == GLUT_LEFT_BUTTON)
	{
		mousePressed = 0;
	}

	if(button == 3 && cam_zoom < 16)
	{
		cam_zoom += .1;
	}

	if(button == 4 && cam_zoom > 1)
	{
		cam_zoom -= .1;
	}

	glutPostRedisplay();
}

/*****************************************************************
*                         mousePosition                          *
*****************************************************************/
//Purpose: Gives the position of the mouse
void mousePosition(int x, int y)
{
	currentMouseXPos = (float)x*(1000.0)/glutGet(GLUT_WINDOW_WIDTH);
}

/*****************************************************************
*                         camera_control                         *
*****************************************************************/
//Purpose: handle the rotation of the space ship
void camera_control(unsigned char key, int x, int y)
{
	if(key == 'a')
	{
		y_rot -= 5.0;
		if(y_rot < 360.0)
		{
			y_rot += 360;
		}
	}

	if(key == 'd')
	{
		y_rot += 5.0;
		if(y_rot > 360.0)
		{
			y_rot -= 360;
		}
	}

	if(key == 's')
	{
		z_rot += 5.0;
		if(z_rot > 360.0)
		{
			z_rot -= 360;
		}
	}

	if(key == 'w')
	{
		z_rot -= 5.0;
		if(z_rot < 0.0)
		{
			z_rot += 360;
		}
	}

	if(key == 'j')
	{
		spotDirection[0]-=.05/cam_zoom;
		spotDirection[2]+=.05/cam_zoom;
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirection);
	}

	if(key == 'l')
	{
		spotDirection[0]+=.05/cam_zoom;
		spotDirection[2]-=.05/cam_zoom;
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirection);
	}

	if(key == 'k')
	{
		spotDirection[1]-=.05/cam_zoom;
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirection);
	}

	if(key == 'i')
	{
		spotDirection[1]+=.05/cam_zoom;
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirection);
	}

	if(key >= '1' && key <= '8')
	{
		cam_zoom = (float)key - 48.0;
	}

	glutPostRedisplay();
}

/*****************************************************************
*                    renderStrokeFontString                      *
*****************************************************************/
//Purpose: Writes a string out on the screen
void renderStrokeFontString(float x, float y, void *font, char *string, float thickness, float size)
{
	char *c;
	glPushMatrix();
	glTranslatef(x,y,0);
	glScalef(0.125*size, 0.125*size, 0.0);

	glLineWidth(thickness);
	for (c=string; *c != '\0'; c++)
	{
		glutStrokeCharacter(font, *c);
	}

	glPopMatrix();
}

/*****************************************************************
*                         drawHexagonThing                       *
*****************************************************************/
//Purpose: Draws one of the trapezoid shapes that goes along the ship's body
void drawHexagonThing()
{
	static float baseWidth = 36.0;
	static float middleWidth = 90.0;
	static float topWidth = 70.0;
	static float depth = 160.0;
	static float height = 40.0;
	static float verticalOffset = 18.0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, chrome[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chrome[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chrome[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0);

	glColor3f(0.0,0.0,0.0);

	/*
	
	      4 ------------ 3
               /            \
	    5 /              \ 2
              \             /	
	       \           /
              6  ---------  1
	*/

	//draw the outer faces
	glBegin(GL_QUAD_STRIP);
		//1
		glNormal3f(0.0, .5, 0.0);
		glVertex3f(baseWidth/2.0,verticalOffset,0.0);
		glNormal3f(0.0, .5, 0.0);
		glVertex3f(baseWidth/2.0,verticalOffset,depth);
		//2
		glNormal3f(-.5, 0.0, 0.0);
		glVertex3f(middleWidth/2.0,height*.7+verticalOffset,0.0);
		glNormal3f(-.5, 0.0, 0.0);
		glVertex3f(middleWidth/2.0,height*.7+verticalOffset,depth);
		//3
		glNormal3f(0.0, -.5, 0.0);
		glVertex3f(topWidth/2.0,height+verticalOffset,0.0);
		glNormal3f(0.0, -.5, 0.0);
		glVertex3f(topWidth/2.0,height+verticalOffset,depth);
		//4
		glNormal3f(0.0, -.5, 0.0);
		glVertex3f(-topWidth/2.0,height+verticalOffset,0.0);
		glNormal3f(0.0, -.5, 0.0);
		glVertex3f(-topWidth/2.0,height+verticalOffset,depth);
		//5
		glNormal3f(.5, 0.0, 0.0);
		glVertex3f(-middleWidth/2.0,height*.7+verticalOffset,0.0);
		glNormal3f(.5, 0.0, 0.0);
		glVertex3f(-middleWidth/2.0,height*.7+verticalOffset,depth);
		//6
		glNormal3f(.5, .5, 0.0);
		glVertex3f(-baseWidth/2.0,verticalOffset,0.0);
		glNormal3f(.5, .5, 0.0);
		glVertex3f(-baseWidth/2.0,verticalOffset,depth);
		//1
		glNormal3f(0.0, .5, 0.0);
		glVertex3f(baseWidth/2.0,verticalOffset,0.0);
		glNormal3f(0.0, .5, 0.0);
		glVertex3f(baseWidth/2.0,verticalOffset,depth);
	glEnd();

	//draw the front face
	glBegin(GL_POLYGON);
		//1
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(baseWidth/2.0,verticalOffset,0.0);
		//2
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(middleWidth/2.0,height*.7+verticalOffset,0.0);
		//3
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(topWidth/2.0,height+verticalOffset,0.0);
		//4
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-topWidth/2.0,height+verticalOffset,0.0);
		//5
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-middleWidth/2.0,height*.7+verticalOffset,0.0);
		//6
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-baseWidth/2.0,verticalOffset,0.0);
	glEnd();

	//draw the rear face
	glBegin(GL_POLYGON);
		//1
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(baseWidth/2.0,verticalOffset,depth);
		//2
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(middleWidth/2.0,height*.7+verticalOffset,depth);
		//3
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(topWidth/2.0,height+verticalOffset,depth);
		//4
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-topWidth/2.0,height+verticalOffset,depth);
		//5
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-middleWidth/2.0,height*.7+verticalOffset,depth);
		//6
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-baseWidth/2.0,verticalOffset,depth);
	glEnd();
}

/*****************************************************************
*                           drawHexagonSet                       *
*****************************************************************/
//Purpose: Draws a set of three hexagon shapes for the fuselage
void drawHexagonSet(float distance)
{
	glPushMatrix();
		glTranslatef(0.0,0.0,distance);
		drawHexagonThing();
		glRotatef(120.0,0.0,0.0,1.0);
		drawHexagonThing();
		glRotatef(120.0,0.0,0.0,1.0);
		drawHexagonThing();
	glPopMatrix();
}

/*****************************************************************
*                          drawSphereThing                       *
*****************************************************************/
//Purpose: Draws one of the sphere shapes that goes at the front of the ship
void drawSphereThing()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obsidian[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obsidian[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obsidian[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 38.4);
	glPushMatrix();
		glTranslatef(0.0,70.0,0.0);
		glRotatef(90.0,1.0,0.0,0.0);
		gluCylinder(quad, 6.0, 6.0, 100.0, 15, 2);
	glPopMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, steel[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, steel[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, steel[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);
	glPushMatrix();
		glTranslatef(0.0,100.0,0.0);
		gluSphere(quad, 50.0, 60, 60);
	glPopMatrix();
	
}

/*****************************************************************
*                           drawSphereSet                        *
*****************************************************************/
//Purpose: Draws a set of spheres on the end of the ship
void drawSphereSet(float distance)
{
	glPushMatrix();
		glTranslatef(0.0,0.0,distance);
		drawSphereThing();
		glRotatef(60.0,0.0,0.0,1.0);
		drawSphereThing();
		glRotatef(60.0,0.0,0.0,1.0);
		drawSphereThing();
		glRotatef(60.0,0.0,0.0,1.0);
		drawSphereThing();
		glRotatef(60.0,0.0,0.0,1.0);
		drawSphereThing();
		glRotatef(60.0,0.0,0.0,1.0);
		drawSphereThing();
	glPopMatrix();
}

/*****************************************************************
*                         drawSatellite                          *
*****************************************************************/
//Purpose: Draws the satellite on the fuselage
void drawSatellite(void)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, steel[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, steel[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, steel[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);
	glPushMatrix();
		glTranslatef(0.0, 0.0, 980.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glPushMatrix();
			glRotatef(-60.0, 0.0, 1.0, 0.0);
			glTranslatef(0.0, 0.0, 55.0);
			gluCylinder(quad, 15.0, 12.0, 30.0, 25, 5);
			glTranslatef(0.0, 0.0, 30.0);
			gluCylinder(quad, 12.0, 8.0, 20.0, 25, 5);
			glTranslatef(0.0, 0.0, 20.0);
			gluCylinder(quad, 8.0, 12.0, 5.0, 15, 5);
			glTranslatef(0.0, 0.0, 5.0);
			glColor3f(0.0, 0.0, 0.0);
			gluCylinder(quad, 20.0, 18.0, 8.0, 4, 1);
			glutSolidCone(20.0, .001, 4, 1);
			glTranslatef(0.0, 0.0, 8.0);
			glutSolidCone(18.0, .001, 4, 1);
			glTranslatef(0.0, 0.0, -12.0);
			glPushMatrix();
				glColor3f(1.0, 0.4, 0.0);
				glTranslatef(0.0, 0.0, 10.0);
				gluCylinder(quad, 3.0, 3.0, 20.0, 20.0, 3);
				glTranslatef(0.0, 0.0, 20.0);
				glRotatef(123.0, 0.0, 0.0, 1.0);
				glPushMatrix();
					glutSolidCube(8.0);
					glRotatef(30.0, 1.0, 0.0, 0.0);
					gluCylinder(quad, 3.0, 3.0, 10.0, 20.0, 3);
					glTranslatef(0.0, 0.0, 10.0);
					glutSolidCube(5.0);
					glRotatef(50.0, 1.0, 0.3, 0.0);
						glColor3f(0.0, 0.0, 0.0);
						//the cylinder in the middle of the dish
						glTranslatef(0.0, 0.0, -5.0);
						glPushMatrix();
							glTranslatef(0.0,0.0,10.0);
							glutSolidCone(15.0, .001, 20, 1);
							glTranslatef(0.0,0.0,-10.0);
							glutSolidCone(15.0, .001, 20, 1);
						glPopMatrix();
						gluCylinder(quad, 15.0, 15.0, 10.0, 30, 5);
						glTranslatef(0.0, 0.0, 5.0);
					glColor3f(1.0, 0.4, 0.0);
					gluCylinder(quad, 0, 50.0, 5.0, 30, 10);
					glTranslatef(0.0, 0.0, 5.0);
					gluCylinder(quad, 50.0, 75.0, 7.0, 30, 10);
					glTranslatef(0.0, 0.0, 7.0);
					gluCylinder(quad, 75.0, 85, 8.0, 30, 8);
					glColor3f(0.0, 0.0, 0.0);
					glTranslatef(0.0, 0.0, 8.0);
					gluCylinder(quad, 85.0, 86.0, 4.0, 30, 5);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

/*****************************************************************
*                          drawThruster                          *
*****************************************************************/
//Purpose: Draws the thruster of the ship
void drawThruster(void)
{
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ONE_MINUS_SRC_COLOR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, thrusterFrontTex);

	glColor3f(1.0,1.0,1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, chrome[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chrome[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chrome[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);

	glPushMatrix();
		gluCylinder(quad, 150.0, 120.0, 80.0, 30.0, 10.0);
		glutSolidCone(150.0, .001, 100, 3);
		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obsidian[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obsidian[1]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obsidian[2]);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 38.4);
			glTranslatef(0.0, 0.0, 80.0);
			gluCylinder(quad, 120.0, 90.0, 30.0, 30.0, 8.0);
		glPopMatrix();
		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, chrome[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chrome[1]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chrome[2]);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0);
			glTranslatef(0.0, 0.0, 110.0);
			gluCylinder(quad, 90.0, 90.0, 20.0, 30.0, 2.0);
		glPopMatrix();
		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obsidian[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obsidian[1]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obsidian[2]);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 38.4);
			glTranslatef(0.0, 0.0, 130.0);
			gluCylinder(quad, 90.0, 120.0, 30.0, 30.0, 8.0);
			glTranslatef(0.0, 0.0, 30.0);
			glutSolidTorus(8.0, 125.0, 6.0, 60.0);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, chrome[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chrome[1]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chrome[2]);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0);
			gluCylinder(quad, 120.0, 120.0, 90.0, 30.0, 2.0);
			glTranslatef(0.0, 0.0, 90.0);
			glutSolidTorus(8.0, 125.0, 6.0, 60.0);
			glEnable(GL_TEXTURE_2D);
			gluCylinder(quad, 120.0, 20.0, 50.0, 30.0, 10.0);
			glEnable(GL_TEXTURE_2D);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold[1]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold[2]);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 51.2);
			glTranslatef(0.0, 0.0, -20.0);
			glEnable(GL_TEXTURE_2D);
			gluCylinder(quad, 121.0, 121.0, 20.0, 30.0, 8.0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, brass[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, brass[1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, brass[2]);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);
		
		//the actual engines on the bottom
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
			glTranslatef(70.0, 0.0, -40.0);
			gluCylinder(quad, 50.0, 35.0, 50.0, 15.0, 5.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-70.0, 0.0, -40.0);
			gluCylinder(quad, 50.0, 35.0, 50.0, 15.0, 5.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 70.0,-40.0);
			gluCylinder(quad, 50.0, 35.0, 50.0, 15.0, 5.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -70.0,-40.0);
			gluCylinder(quad, 50.0, 35.0, 50.0, 15.0, 5.0);
		glPopMatrix();

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackColor);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);

		//the holes on the bottom
		glPushMatrix();
			glTranslatef(70.0, 0.0, -40.0);
			glutSolidCone(50.0, .001, 75, 3);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-70.0, 0.0, -40.0);
			glutSolidCone(50.0, .001, 75, 3);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 70.0,-40.0);
			glutSolidCone(50.0, .001, 75, 3);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -70.0,-40.0);
			glutSolidCone(50.0, .001, 75, 3);
		glPopMatrix();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

/*****************************************************************
*                         drawCentrifuge                         *
*****************************************************************/
//Purpose: Draws the centrifugal part of the ship
void drawCentrifuge(void)
{
	//draw the support beams to the centrifuge
	glColor3f(0.0,1.0,0.0);
	glPushMatrix();
		glTranslatef(0.0, 0.0, 205.0);
		glPushMatrix();
			glTranslatef(0.0, -110.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			gluCylinder(quad, 20.0, 20.0, 175.0, 30.0, 20.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(110.0, 0.0, 0.0);
			glRotatef(90.0, 0.0, 0.0, 1.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			gluCylinder(quad, 20.0, 20.0, 175.0, 30.0, 20.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-110.0, 0.0, 0.0);
			glRotatef(270.0, 0.0, 0.0, 1.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			gluCylinder(quad, 20.0, 20.0, 175.0, 30.0, 20.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 110.0, 0.0);
			glRotatef(180.0, 0.0, 0.0, 1.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			gluCylinder(quad, 20.0, 20.0, 175.0, 30.0, 20.0);
		glPopMatrix();
	glPopMatrix();
	
	//draw the centrifuge
	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, chrome[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chrome[1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chrome[2]);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0);
		glTranslatef(0.0, 0.0, 205.0);
		glutSolidTorus(45.0, 320.0, 30.0, 60.0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obsidian[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obsidian[1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obsidian[2]);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 38.4);
		glutSolidTorus(5.0, 365.0, 10.0, 60.0);
		glTranslatef(0.0, 0.0, -45.0);
		glutSolidTorus(5.0, 320.0, 12.0, 60.0);
		glTranslatef(0.0, 0.0, 90.0);
		glutSolidTorus(5.0, 320.0, 12.0, 60.0);
	glPopMatrix();
}

/*****************************************************************
*                          drawFuselage                          *
*****************************************************************/
//Purpose: Draws the body of the ship
void drawFuselage(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, steel[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, steel[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, steel[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);

	int i;
	glColor3f(0.0,1.0,0.0);
	glPushMatrix();
		glTranslatef(0.0, 0.0, 300.0);
		gluCylinder(quad, 20.0, 20.0, 1870.0, 30.0, 100.0);
		glTranslatef(0.0,0.0,1870.0);
		glutSolidCone(20.0, 15.0, 20, 10);
	glPopMatrix();

	for(i = 0; i < 6; i++)
	{
		drawHexagonSet(300.0 + i*200);
	}
	
	for(i = 0; i < 6; i++)
	{
		drawSphereSet(1550+i*115);
	}
}

/*****************************************************************
*                            drawShip                            *
*****************************************************************/
//Purpose: Makes calls to draw individual parts of the space ship
void drawShip(void)
{
	glLineWidth(1.0);
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
		glRotatef(y_rot,0.0,1.0,0.0);
		glRotatef(z_rot,0.0,0.0,1.0);
		glTranslatef(0.0, -1000.0, 0.0);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		glRotatef(-30.0, 0.0, 0.0, 1.0);
		glLightfv(GL_LIGHT1, GL_POSITION, satellitePos);
		glLightfv(GL_LIGHT2, GL_POSITION, ringLightPos1);
		glLightfv(GL_LIGHT3, GL_POSITION, ringLightPos2);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glass[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glass[1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glass[2]);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0);

		//position the satellite light
		glPushMatrix();
			glTranslatef(satellitePos[0], satellitePos[1], satellitePos[2]);
		glPopMatrix();

		//position the first ring light
		glPushMatrix();
			glTranslatef(ringLightPos1[0], ringLightPos1[1], ringLightPos1[2]+15.0);
			glutSolidSphere(5.0, 20, 20);
		glPopMatrix();

		//position the second ring light
		glPushMatrix();
			glTranslatef(ringLightPos2[0], ringLightPos2[1], ringLightPos2[2]+15.0);
			glutSolidSphere(5.0, 20, 20);
		glPopMatrix();
		
		drawThruster();
		drawFuselage();
		drawCentrifuge();
		drawSatellite();
	glPopMatrix();
}

/*****************************************************************
*                          modelingMode                          *
*****************************************************************/
//Purpose: Draws the scene in modeling mode to make it easier to build the ship
void modelingMode(void)
{
	GLfloat theSunPos[] = {0.0, 30000.0, 0.0, 1.0};

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(100.0,1.0,0.1,6000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//isometric/perspective view
	gluLookAt(3000.0/cam_zoom, 3000.0/cam_zoom, 3000.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, theSunPos);
	glLightfv(GL_LIGHT4, GL_POSITION, spotPos);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spotDirection);
	glViewport(0, 0, 1000, 1000);

	drawShip();
}

/*****************************************************************
*                              idle                              *
*****************************************************************/
//Purpose: GLUT idle function
void idle(void)
{
	Timer = clock();

	if(Timer > ResetTimer)
	{
		ResetTimer = ResetTimer + (1.0/60.0) * CLOCKS_PER_SEC;
				
		glutPostRedisplay();
	}

	if(mousePressed == 1)
	{
		y_rot = startRot + .3*(currentMouseXPos - startXPos);
		while(y_rot < 0.0)
			y_rot += 360.0;
		while(y_rot > 360.0)
			y_rot -= 360;
	}
}

void loadTextures() {
	
	thrusterFrontTex = SOIL_load_OGL_texture("256.bmp", 4, 0, 0);
	if (!thrusterFrontTex) {
		printf("***NO BITMAP RETRIEVED thrusterFrontTex***\n");  //Check to see if successfully loaded
		exit(1);
	}

	hexagonTex = SOIL_load_OGL_texture("hexagon.jpg", 4, 0, 0);
	if (!hexagonTex) {
		printf("***NO BITMAP RETRIEVED hexagonTex***\n");  //Check to see if successfully loaded
		exit(1);
	}
}

/*****************************************************************
*                             display                            *
*****************************************************************/
//Purpose: GLUT display function
void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	modelingMode();

	glFlush ();
}

/*****************************************************************
*                               main                             *
*****************************************************************/
//Purpose: Main function that initializes everything
int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitWindowSize (1000, 1000);
	glutInitWindowPosition (0, 0);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(camera_control);
	glutIdleFunc(idle);
	init();

	//make the menu
	loadTextures();

	int lights = glutCreateMenu(LightOptions);
	glutAddMenuEntry("Sun", 1);
	glutAddMenuEntry("Satellite", 2);
	glutAddMenuEntry("Torus", 3);
	glutAddMenuEntry("Spotlight", 4);

	int menu = glutCreateMenu(mainMenu);
	glutAddMenuEntry("Reset", 1);
	glutAddSubMenu("Lights", lights);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc (display);
	glutMouseFunc(mouse);
	glutMotionFunc(mousePosition);
	glutMainLoop ();
	return 0;
}

