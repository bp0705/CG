/***************************************************************
*                                                              *
*                           Project 3                          *
*                          Brett Pare                          *
*                                                              *
*               Model a wireframe spaceship in 3D              *
*                                                              *
****************************************************************/

/* Skeleton for displaying glut 3D primitives */

#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

#define CAMERA_DISTANCE 2000

long Timer, ResetTimer;
float y_rot = 0.0;
float z_rot = 0.0;
float cam_zoom = 1.0;
int showHelpMenu = 0;
int presentationMode = 0;
int mousePressed = 0;
int startXPos = 0;
float startRot = 0.0;
int currentMouseXPos = 0.0;
int showAxes = 1;

GLUquadricObj* quad;

/*****************************************************************
*                               init                             *
*****************************************************************/
//Purpose: Initializes clear color and other options
void init(void)
{
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	quad = gluNewQuadric();

	//******* Set 2nd argument
	gluQuadricDrawStyle(quad, GLU_LINE);
	gluQuadricOrientation(quad, GLU_OUTSIDE);
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
			showHelpMenu = 1;
			break;
		
		case 2:
			y_rot = 0.0;
			z_rot = 0.0;
			cam_zoom = 1.0;
			break;

		case 3:
			presentationMode = 1;
			break;

		case 4:
			presentationMode = 0;
			break;
		case 5:
			if(showAxes == 0)
			{
				showAxes = 1;
			}
			else
			{
				showAxes = 0;
			}
			break;

		case 6:
			exit(-1);
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
	if(state == GLUT_DOWN)
	{
		showHelpMenu = 0;
	}

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

	if(button == 3 && cam_zoom < 8)
	{
		cam_zoom++;
	}

	if(button == 4 && cam_zoom > 1)
	{
		cam_zoom--;
	}
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
*                            drawAxes                            *
*****************************************************************/
//Purpose: draws the axis for testing purposes
void drawAxes(void)
{
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(1.0);
	glBegin(GL_LINES);
		//x axis
		glVertex3f(-500.0, 0.0, 0.0);
		glVertex3f(500.0, 0.0, 0.0);
		glVertex3f(500.0, 0.0, -50.0);
		glVertex3f(500.0, 0.0, 50.0);
		glVertex3f(-500.0, 0.0, -50.0);
		glVertex3f(-500.0, 0.0, 50.0);

		//y axis
		glVertex3f(0.0, -1000.0, 0.0);
		glVertex3f(0.0, 1000.0, 0.0);

		//z axis
		glVertex3f(0.0, 0.0, -500.0);
		glVertex3f(0.0, 0.0, 500.0);
		glVertex3f(-50.0, 0.0, 500.0);
		glVertex3f(50.0, 0.0, 500.0);
		glVertex3f(-50.0, 0.0, -500.0);
		glVertex3f(50.0, 0.0, -500.0);
	glEnd();
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		glVertex3f(baseWidth/2.0,verticalOffset,0.0);
		glVertex3f(baseWidth/2.0,verticalOffset,depth);
		//2
		glVertex3f(middleWidth/2.0,height*.7+verticalOffset,0.0);
		glVertex3f(middleWidth/2.0,height*.7+verticalOffset,depth);
		//3
		glVertex3f(topWidth/2.0,height+verticalOffset,0.0);
		glVertex3f(topWidth/2.0,height+verticalOffset,depth);
		//4
		glVertex3f(-topWidth/2.0,height+verticalOffset,0.0);
		glVertex3f(-topWidth/2.0,height+verticalOffset,depth);
		//5
		glVertex3f(-middleWidth/2.0,height*.7+verticalOffset,0.0);
		glVertex3f(-middleWidth/2.0,height*.7+verticalOffset,depth);
		//6
		glVertex3f(-baseWidth/2.0,verticalOffset,0.0);
		glVertex3f(-baseWidth/2.0,verticalOffset,depth);
		//1
		glVertex3f(baseWidth/2.0,verticalOffset,0.0);
		glVertex3f(baseWidth/2.0,verticalOffset,depth);
	glEnd();

	//draw the front face
	glBegin(GL_POLYGON);
		//1
		glVertex3f(baseWidth/2.0,verticalOffset,0.0);
		//2
		glVertex3f(middleWidth/2.0,height*.7+verticalOffset,0.0);
		//3
		glVertex3f(topWidth/2.0,height+verticalOffset,0.0);
		//4
		glVertex3f(-topWidth/2.0,height+verticalOffset,0.0);
		//5
		glVertex3f(-middleWidth/2.0,height*.7+verticalOffset,0.0);
		//6
		glVertex3f(-baseWidth/2.0,verticalOffset,0.0);
	glEnd();

	//draw the rear face
	glBegin(GL_POLYGON);
		//1
		glVertex3f(baseWidth/2.0,verticalOffset,depth);
		//2
		glVertex3f(middleWidth/2.0,height*.7+verticalOffset,depth);
		//3
		glVertex3f(topWidth/2.0,height+verticalOffset,depth);
		//4
		glVertex3f(-topWidth/2.0,height+verticalOffset,depth);
		//5
		glVertex3f(-middleWidth/2.0,height*.7+verticalOffset,depth);
		//6
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
	glPushMatrix();
		glColor3f(1.0,0.0,0.0);
		glTranslatef(0.0,70.0,0.0);
		glRotatef(90.0,1.0,0.0,0.0);
		gluCylinder(quad, 6.0, 6.0, 45.0, 15, 2);
	glPopMatrix();
	glPushMatrix();
		glColor3f(0.0,0.0,0.0);
		glTranslatef(0.0,100.0,0.0);
		gluSphere(quad, 55.0, 20, 10);
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
	glPushMatrix();
		glTranslatef(0.0, 0.0, 980.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glPushMatrix();
			glRotatef(-60.0, 0.0, 1.0, 0.0);
			glTranslatef(0.0, 0.0, 55.0);
			gluCylinder(quad, 15.0, 10.0, 90.0, 25, 5);
			glTranslatef(0.0, 0.0, 90.0);
			gluCylinder(quad, 20.0, 18.0, 10.0, 4, 1);
		glPopMatrix();
	glPopMatrix();
}

/*****************************************************************
*                          drawThruster                          *
*****************************************************************/
//Purpose: Draws the thruster of the ship
void drawThruster(void)
{
	glColor3f(0.0,0.0,0.0);
	glPushMatrix();
		gluCylinder(quad, 150.0, 120.0, 80.0, 30.0, 10.0);
		glPushMatrix();
			glTranslatef(0.0, 0.0, 80.0);
			gluCylinder(quad, 120.0, 90.0, 30.0, 30.0, 8.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 0.0, 110.0);
			gluCylinder(quad, 90.0, 90.0, 20.0, 30.0, 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 0.0, 130.0);
			gluCylinder(quad, 90.0, 120.0, 30.0, 30.0, 8.0);
			glTranslatef(0.0, 0.0, 30.0);
			glutWireTorus(8.0, 125.0, 6.0, 60.0);
			gluCylinder(quad, 120.0, 120.0, 90.0, 30.0, 2.0);
			glTranslatef(0.0, 0.0, 90.0);
			glutWireTorus(8.0, 125.0, 6.0, 60.0);
			gluCylinder(quad, 120.0, 20.0, 50.0, 30.0, 10.0);
		glPopMatrix();

		//the actual engines on the bottom
		glColor3f(1.0,0.0,1.0);
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
	glPopMatrix();
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
			gluCylinder(quad, 20.0, 20.0, 175.0, 30.0, 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(110.0, 0.0, 0.0);
			glRotatef(90.0, 0.0, 0.0, 1.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			gluCylinder(quad, 20.0, 20.0, 175.0, 30.0, 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-110.0, 0.0, 0.0);
			glRotatef(270.0, 0.0, 0.0, 1.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			gluCylinder(quad, 20.0, 20.0, 175.0, 30.0, 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 110.0, 0.0);
			glRotatef(180.0, 0.0, 0.0, 1.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			gluCylinder(quad, 20.0, 20.0, 175.0, 30.0, 2.0);
		glPopMatrix();
	glPopMatrix();
	
	//draw the centrifuge
	glColor3f(0.0,0.0,1.0);
	glPushMatrix();
		glTranslatef(0.0, 0.0, 205.0);
		glutWireTorus(45.0, 320.0, 30.0, 60.0);
		glColor3f(0.0,0.0,0.0);
		glutWireTorus(5.0, 365.0, 10.0, 60.0);
		glTranslatef(0.0, 0.0, -45.0);
		glutWireTorus(5.0, 320.0, 12.0, 60.0);
		glTranslatef(0.0, 0.0, 90.0);
		glutWireTorus(5.0, 320.0, 12.0, 60.0);
	glPopMatrix();
}

/*****************************************************************
*                          drawFuselage                          *
*****************************************************************/
//Purpose: Draws the body of the ship
void drawFuselage(void)
{
	int i;
	glColor3f(0.0,1.0,0.0);
	glPushMatrix();
		glTranslatef(0.0, 0.0, 300.0);
		gluCylinder(quad, 20.0, 20.0, 1870.0, 30.0, 5.0);
		glTranslatef(0.0,0.0,1870.0);
		glutWireCone(20.0, 15.0, 20, 10);
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
	glColor3f(0.0,0.0,0.0);
	glPushMatrix();
		glRotatef(y_rot,0.0,1.0,0.0);
		glRotatef(z_rot,0.0,0.0,1.0);
		glTranslatef(0.0, -1000.0, 0.0);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		glRotatef(-30.0, 0.0, 0.0, 1.0);
		
		drawThruster();
		drawFuselage();
		drawCentrifuge();
		drawSatellite();
	glPopMatrix();
}

/*****************************************************************
*                          presentation                          *
*****************************************************************/
//Purpose: Draws the scene in presentation mode (all four viewports)
void presentation(void)
{
	int i;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glOrtho (-4000.0/cam_zoom, 4000.0/cam_zoom, -4000.0/cam_zoom, 4000.0/cam_zoom, -3000.0, 3000.0);
	//gluPerspective(100.0,1.0,0.0,6000);
	//draw each of the viewports on the screen
	for(i = 0; i < 4; i++)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		switch(i)
		{
			case 0:
				//top view
				glOrtho (-4000.0/cam_zoom, 4000.0/cam_zoom, -4000.0/cam_zoom, 4000.0/cam_zoom, -3000.0, 3000.0);
				gluLookAt(0.000001, 3000.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glViewport(0, 500, 500, 500);
				break;

			case 1:
				//isometric/perspective view
				gluPerspective(100.0,1.0,0.0,6000);
				gluLookAt(3000.0/cam_zoom, 3000.0/cam_zoom, 3000.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glViewport(500, 500, 500, 500);
				break;

			case 2:
				//front view
				glOrtho (-4000.0/cam_zoom, 4000.0/cam_zoom, -4000.0/cam_zoom, 4000.0/cam_zoom, -3000.0, 3000.0);
				gluLookAt(0.0, 0.0, 3000.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glViewport(0, 0, 500, 500);
				break;

			case 3:
				//right side view
				glOrtho (-4000.0/cam_zoom, 4000.0/cam_zoom, -4000.0/cam_zoom, 4000.0/cam_zoom, -3000.0, 3000.0);
				gluLookAt(3000.0/cam_zoom,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glViewport(500, 0, 500, 500);
				break;
		}

		if(showAxes)
		{
			drawAxes();
		}
		drawShip();
	}
}

/*****************************************************************
*                          modelingMode                          *
*****************************************************************/
//Purpose: Draws the scene in modeling mode to make it easier to build the ship
void modelingMode(void)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glOrtho(-4000.0/cam_zoom, 4000.0/cam_zoom, -4000.0/cam_zoom, 4000.0/cam_zoom, -3000.0, 3000.0);
	gluPerspective(100.0,1.0,0.0,6000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//isometric/perspective view
	gluLookAt(3000.0/cam_zoom, 3000.0/cam_zoom, 3000.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glViewport(0, 0, 1000, 1000);
	if(showAxes)
	{
		drawAxes();
	}
	drawShip();
	//drawSatellite();
}

/*****************************************************************
*                          drawHelpMenu                          *
*****************************************************************/
//Purpose: Draws the help menu
void drawHelpMenu()
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (-500.0, 500.0, -500.0, 500.0, -200.0, 3000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 50000.0, 100.0, 0.0, 50000.0, 0.0, 0.0, 1.0, 0.0);
	glViewport(250, 250, 500, 500);

	//draw the black box of the help menu
	glColor3f(0.0,0.0,0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		glVertex3f(500.0,50500.0,0.0);
		glVertex3f(-500.0,50500.0,0.0);
		glVertex3f(-500.0,49500.0,0.0);
		glVertex3f(500.0,49500.0,0.0);
	glEnd();

	//draw the text in the help menu
	glColor3f(1.0,1.0,1.0);
	renderStrokeFontString(-350.0, 50375.0, GLUT_STROKE_MONO_ROMAN, "Help Menu", 3.0, 6.0);
	renderStrokeFontString(-275.0, 50250.0, GLUT_STROKE_MONO_ROMAN, "In Presentation mode:", 2.0, 2.0);
	renderStrokeFontString(-315.0, 50175.0, GLUT_STROKE_MONO_ROMAN, "A and D: Rotate about the Y axis", 2.0, 1.5);
	renderStrokeFontString(-315.0, 50125.0, GLUT_STROKE_MONO_ROMAN, "W and S: Rotate about the Z axis", 2.0, 1.5);
	renderStrokeFontString(-230.0, 50000.0, GLUT_STROKE_MONO_ROMAN, "In Modeling mode:", 2.0, 2.0);
	renderStrokeFontString(-330.0, 49930.0, GLUT_STROKE_MONO_ROMAN, "Y axis: Click and drag left mouse", 2.0, 1.5);
	renderStrokeFontString(-270.0, 49880.0, GLUT_STROKE_MONO_ROMAN, "Controls above work as well", 2.0, 1.5);
	
	renderStrokeFontString(-310.0, 49525.0, GLUT_STROKE_MONO_ROMAN, "Click anywhere to close", 2.0, 2.0);
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

		printf("y_rot: %.2f, z_rot: %.2f\n", y_rot, z_rot);
	}

	if(presentationMode == 0 && mousePressed == 1)
	{
		y_rot = startRot + .3*(currentMouseXPos - startXPos);
		while(y_rot < 0.0)
			y_rot += 360.0;
		while(y_rot > 360.0)
			y_rot -= 360;
	}
}

/*****************************************************************
*                             display                            *
*****************************************************************/
//Purpose: GLUT display function
void display (void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_FILL);
	
	if(presentationMode)
	{
		//for demonstration
		presentation();
	}
	else
	{
		//for modeling the ship
		modelingMode();
	}

	if(showHelpMenu)
	{
		//help menu for controls
		drawHelpMenu();
	}

	glFlush ();
}

/*****************************************************************
*                               main                             *
*****************************************************************/
//Purpose: Main function that initializes everything
int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize (1000, 1000);
	glutInitWindowPosition (0, 0);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(camera_control);
	glutIdleFunc(idle);
	init();

	//make the menu
	int menu = glutCreateMenu(mainMenu);
	glutAddMenuEntry("Help", 1);
	glutAddMenuEntry("Reset", 2);
	glutAddMenuEntry("Present", 3);
	glutAddMenuEntry("Model", 4);
	glutAddMenuEntry("Toggle Axes", 5);
	glutAddMenuEntry("Exit", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc (display);
	glutMouseFunc(mouse);
	glutMotionFunc(mousePosition);
	glutMainLoop ();
	return 0;
}
