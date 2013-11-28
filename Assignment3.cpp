#include <windows.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>

#include "Math3DLib.h"
#include "Particle.h"
#include "TrigLib.h"

using namespace std;

#define SECS_PER_TICK 0.0083
clock_t lastUpdateTime;

using namespace std;

//Declaring Variables
int screenSize[2] = {500,500};
int cameraPos[3] = {150, 150, 150};
int startPos[3] = {0,0,0};

int timer = 0;
bool keyboard[4];
bool* keySpecialStates = new bool[256];

float angle = 0;

std::vector<particle> particleList;

//Draws the axis
void axis(int sizeX, int sizeY, int sizeZ){

	glBegin(GL_LINES);
		//y-axis
		glColor3f(1, 0, 0);
		glVertex3f(0, sizeY, 0);
		glVertex3f(0, -sizeY, 0);
		//x-axis
		glColor3f(0, 1, 0);
		glVertex3f(-sizeX, 0, 0);
		glVertex3f(sizeX, 0, 0);
		//z-axis
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, -sizeZ);
		glVertex3f(0, 0, sizeZ);
	glEnd();

}

//Draws walls for the room
void walls(float size)
{
	glBegin(GL_QUADS);
		glColor3f(0.20,0.80,0.65);
		glVertex3f(size, size, size);
		glVertex3f(size, -size, size);
		glVertex3f(size, -size, 0);
		glVertex3f(size, size, 0);

		glVertex3f(-size, -size, size);
		glVertex3f(size, -size, size);
		glVertex3f(size, -size, 0);
		glVertex3f(-size, -size, 0);

		glVertex3f(-size, -size, size);
		glVertex3f(-size, size, size);
		glVertex3f(-size, size, 0);
		glVertex3f(-size, -size, 0);
	glEnd();
}

//Draws the floor at the x-y plane
void floor(int x, int z)
{
	glBegin(GL_QUADS);
		glColor3f(0.65,0.65,0.65);
		glVertex3f(x, x, z);
		glVertex3f(-x,x,z);
		glVertex3f(-x,-x,z);
		glVertex3f(x,-x,z);
	glEnd();
}

void objectDraw()
{
	for(int i=0; i<particleList.size(); i++){
		//set up particle colour
		/*glBegin(GL_POINTS);
			glColor3f(particleList[i].particleColour.r,particleList[i].particleColour.g,particleList[i].particleColour.b);
		glEnd();*/

		glPushMatrix();
			//set particle position
			/*glTranslatef(particleList[i].particlePosition.x,particleList[i].particlePosition.y,particleList[i].particlePosition.z);*/
			glTranslated(particleList[i].getPosition().x,particleList[i].getPosition().y,particleList[i].getPosition().z);
			glPushMatrix();
				//rotate particle and draw
				/*glRotatef(particleList[i].rot[0],1,0,0);
				glRotatef(particleList[i].rot[1],0,1,0);
				glRotatef(particleList[i].rot[2],0,0,1);*/

				//shapes the particle
				/*shapeParticle(particleList[i]);*/
				glutSolidCube(particleList[i].getSize());
			glPopMatrix();
		glPopMatrix();
	}
}

void createCube(GLdouble x, GLdouble y, GLdouble z)
{
	particleList.push_back(particle(point3D(x,y,z),colour(180,120,240),5,point3D(),vec3D(0,0,0),"cube"));
}

//Handling ASCII keys
void kbd(unsigned char key, int x, int y)
{
	//if the "q" key is pressed, quit the program
	if(key == 'q' || key == 'Q')
	{
		exit(0);
	}
	if(key == 'f' || key == 'F')
	{

	}
	if(key == 'r' || key == 'R')
	{
		particle::particleList.clear();
	}
	if(key == 32)
	{

	}
}

//Camera movement
void SpecialKeyInput(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT)
	{
		keyboard[0] = true;
	}
	if(key == GLUT_KEY_RIGHT)
	{
		keyboard[1] = true;
	}
	if(key == GLUT_KEY_UP)
	{
		keyboard[2] = true;
	}
	if(key == GLUT_KEY_DOWN)
	{
		keyboard[3] = true;
	}

}

void SpecialUpFunc(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT)
	{
		keyboard[0] = false;
	}
	if(key == GLUT_KEY_RIGHT)
	{
		keyboard[1] = false;
	}
	if(key == GLUT_KEY_UP)
	{
		keyboard[2] = false;
	}
	if(key == GLUT_KEY_DOWN)
	{
		keyboard[3] = false;
	}
}

void MouseClick(int btn, int state, int x, int y)
{
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		winX = (float)x;
		winY = (float)viewport[3] - (float)y;
		glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		cout << posX;
		printf(", ");
		cout << posY;
		printf(", ");
		cout << posZ;
		printf("\n");

		createCube(posX, posY, posZ);

	}
}

//updates camera
void updateParticle(double deltaTime)
{
	//Camera Angles
	if(keyboard[0])
	{
		angle+=0.5;
	}
	else if(keyboard[1])
	{
		angle-=0.5;
	}
	else if(keyboard[2])
	{
		cameraPos[2] += 1;
	}
	else if(keyboard[2])
	{
		cameraPos[2] += 0.05;
	}
	else if(keyboard[3])
	{
		cameraPos[2] -= 0.05;
	}

	glutPostRedisplay();
}

void printManual()
{

}

//updating particles
void update(void)
{
	while((lastUpdateTime + CLOCKS_PER_SEC * SECS_PER_TICK < clock()) )//& pauseGame)
	{
		lastUpdateTime += CLOCKS_PER_SEC*SECS_PER_TICK;
		updateParticle(SECS_PER_TICK);
	}
}

//Main display function
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); //Clear the colour buffer (more buffers later on)
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (60.0, 1, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);

	gluLookAt(cameraPos[0]*sind(angle), cameraPos[1]*cosd(angle),cameraPos[2],0,0,0,0,0,1);
	
	//Drawing the scene
	axis(screenSize[0],screenSize[1], screenSize[0]);
	floor(70, 0);
	walls(70);

	objectDraw();

	glutSwapBuffers();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	printManual();

	srand(time(0));
	glutInit(&argc, argv);		//starts up GLUT

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(screenSize[0], screenSize[1]);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Simpler Modler");

	glClearColor(0, 0, 0, 0);

	glutKeyboardFunc(kbd);
	//glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(SpecialKeyInput);
	glutSpecialUpFunc(SpecialUpFunc);
	glutMouseFunc(MouseClick);

	glEnable (GL_DEPTH_TEST);

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutIdleFunc(update);

	//Starts program
	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
	
}