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

long Timer, ResetTimer;
float y_rot = 0.0;
float z_rot = 0.0;
float cam_zoom = 1.0;
int showHelpMenu = 0;
int presentationMode = 1;
int mousePressed = 0;
int startXPos = 0;
float startRot = 0.0;
float currentMouseXPos = 0.0;

/*****************************************************************
*                               init                             *
*****************************************************************/
//Purpose: Initializes clear color and other options
void init(void)
{
	glClearColor (1.0, 1.0, 1.0, 0.0);
	//glEnable(GL_DEPTH_TEST);

}

/*****************************************************************
*                            reshape                             *
*****************************************************************/
//Purpose: Reshape the window
void reshape(int w, int h)
{
	gluLookAt(500.0/cam_zoom, 500.0/cam_zoom, 500.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (-1000.0/cam_zoom, 1000.0/cam_zoom, -1000.0/cam_zoom, 1000.0/cam_zoom, -200.0, 3000.0);
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
*                            drawShip                            *
*****************************************************************/
//Purpose: Makes calls to draw individual parts of the space ship
void drawShip(void)
{
	glLineWidth(1.0);
	//**** Call to glut 3D functions go here
	glColor3f(0.0,0.0,0.0);
	glPushMatrix();
		glRotatef(z_rot,0.0,0.0,1.0);
		glRotatef(y_rot,0.0,1.0,0.0);
		glutWireTorus(25.0,150.0,30,30);
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
	glOrtho (-1000.0/cam_zoom, 1000.0/cam_zoom, -1000.0/cam_zoom, 1000.0/cam_zoom, -200.0, 3000.0);
	//draw each of the viewports on the screen
	for(i = 0; i < 4; i++)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		switch(i)
		{
			case 0:
				//top view
				gluLookAt(1.0, 500.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glViewport(0, 500, 500, 500);
				break;

			case 1:
				//isometric/perspective view
				gluLookAt(500.0/cam_zoom, 500.0/cam_zoom, 500.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glViewport(500, 500, 500, 500);
				break;

			case 2:
				//front view
				gluLookAt(0.0, 0.0, 500.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glViewport(0, 0, 500, 500);
				break;

			case 3:
				//right side view
				gluLookAt(500.0/cam_zoom,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glViewport(500, 0, 500, 500);
				break;
		}

		drawAxes();
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
	glOrtho (-1000.0/cam_zoom, 1000.0/cam_zoom, -1000.0/cam_zoom, 1000.0/cam_zoom, -200.0, 3000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//isometric/perspective view
	gluLookAt(500.0/cam_zoom, 500.0/cam_zoom, 500.0/cam_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glViewport(0, 0, 1000, 1000);
	drawAxes();
	drawShip();
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
	renderStrokeFontString(-250.0, 49930.0, GLUT_STROKE_MONO_ROMAN, "Click and drag left mouse", 2.0, 1.5);
	
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
	glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB);
	//glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize (1000, 1000);
	glutInitWindowPosition (0, 0);
	glutCreateWindow (argv[0]);
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
	glutAddMenuEntry("Exit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc (display);
	glutMouseFunc(mouse);
	glutMotionFunc(mousePosition);
	glutMainLoop ();
	return 0;
}
