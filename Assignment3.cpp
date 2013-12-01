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

//Time declaration for update func
#define SECS_PER_TICK 0.0083
clock_t lastUpdateTime;

using namespace std;

//Declaring Variables

//Screen size for glut
int screenSize[2] = {500,500};

//Initial camera position
int cameraPos[3] = {150, 150, 150};
float cameraAngle = 0;

//Scene Rotation angle
float sceneRotation[2];

//Initial light source variable
float lightsource1[4] = {0.0,0.0,150.0,1.0};
float lightsource2[4] = {0,-100.0,110.0,1.0};
float lightsourceSize = 5.0;


//Timer for update func
int timer = 0;

//Variables for keyboard
bool keyboard[4]; //Arrow Keys
bool* keySpecialStates = new bool[256];

//Index for different shapes
int shapeSelectIndex = 0;

//Index for different materials
int materialSelectIndex = 0;

//Declaring lists
std::vector<particle> particleList;
std::vector<particle> lightList;
std::vector<string> shapeList;
std::vector<string> materialList;

//Light variables
float position[4] = {1.5,0,0, 0};
float amb[4] = {1.0, 1, 1, 1};
float diff[4] = {1,0,0, 1};
float spec[4] = {0,0,1, 1};

//Material Variables

//Ruby
float r_amb[] = {0.1745,0.01175,0.01175, 1.0};
float r_dif[] = {0.61424,0.04136,0.04136, 1.0};
float r_spec[] = {0.727811,0.626959,0.626959};
float r_shiny = 0.6;

//Chrome
float c_amb[] = {0.25,0.25,0.25, 1.0};
float c_dif[] = {0.4,0.4,0.4, 1.0};
float c_spec[] = {0.774597,0.774597,0.774597, 1.0};
float c_shiny = 0.6;

//Emerald
float e_amb[] = {0.0215,0.1745,0.0215, 1.0};
float e_dif[] = {0.07568,0.61424,0.07568, 1.0};
float e_spec[] = {0.633,0.727811,0.633, 1.0};
float e_shiny = 0.6;

//Red Plastic
float rp_amb[] = {0.0,0.0,0.0, 1.0};
float rp_dif[] = {0.5,0.0,0.0, 1.0};
float rp_spec[] = {0.7,0.6,0.6, 1.0};
float rp_shiny = 0.25;

//White Rubber
float w_amb[] = {0.05,0.05,0.05, 1.0};
float w_dif[] = {0.5,0.5,0.5, 1.0};
float w_spec[] = {0.7,0.7,0.7, 1.0};
float w_shiny = 0.078125;


//Prints manual to console
void printManual()
{

}



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
void drawScene(float size)
{
	glBegin(GL_QUADS);
		glColor3f(0.20,0.80,0.65);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, r_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, r_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, r_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, r_shiny);

		//Left cube face
		glNormal3d(-1, 0, 0);
		glVertex3f(size, size, size);
		glVertex3f(size, -size, size);
		glVertex3f(size, -size, 0);
		glVertex3f(size, size, 0);

		//Back cube face
		glNormal3d(0, 1, 0);
		glVertex3f(-size, -size, size);
		glVertex3f(size, -size, size);
		glVertex3f(size, -size, 0);
		glVertex3f(-size, -size, 0);

		//Right cube face
		glNormal3d(1, 0, 0);
		glVertex3f(-size, -size, size);
		glVertex3f(-size, size, size);
		glVertex3f(-size, size, 0);
		glVertex3f(-size, -size, 0);

		//Front cube face
		glNormal3d(0,-1,0);
		glVertex3f(size,size,size);
		glVertex3f(-size,size,size);
		glVertex3f(-size,size,0);
		glVertex3f(size,size,0);

		//Bottom cube face
		glNormal3d(0, 0, 1);
		glVertex3f(size, size, 0);
		glVertex3f(-size,size,0);
		glVertex3f(-size,-size,0);
		glVertex3f(size,-size,0);
	glEnd();
}



//Creating and Delting of objects
void createObject(point3D pos)
{
	particleList.push_back(particle(pos,colour(180,120,240),5,point3D(1,1,1),vec3D(0,0,0),shapeList[shapeSelectIndex],materialList[materialSelectIndex]));
}

void deleteObject(point3D pos)
{
	if(!particleList.empty()){
		for(int i=0; i < particleList.size(); i++){
			if(Math3DLib::distance(particleList[i].getPosition(),pos)<=particleList[i].getSize()){
				particleList.erase(particleList.begin()+i);
				break;
			}
		}
		glutPostRedisplay();
	}
}



//Functions for alternating shapes
void generateShapeList()
{
	shapeList.push_back("cube");
	shapeList.push_back("sphere");
	shapeList.push_back("cone");
	shapeList.push_back("torus");
	shapeList.push_back("teapot");
}

void nextShape()
{
	shapeSelectIndex++;
	if(shapeSelectIndex>shapeList.size()-1)
	{
		shapeSelectIndex = 0;
	}
}

void prevShape()
{
	shapeSelectIndex--;
	if(shapeSelectIndex<0)
	{
		shapeSelectIndex = shapeList.size()-1;
	}
}

void selectShape(particle p)
{
	if(p.getShape()=="cube"){
		glutSolidCube(p.getSize());
	} else if (p.getShape()=="sphere"){
		glutSolidSphere(p.getSize(),30,30);
	} else if (p.getShape()=="cone"){
		glutSolidCone(p.getSize()/2,p.getSize(),30,30);
	} else if (p.getShape()=="torus"){
		glutSolidTorus(p.getSize()/2,p.getSize(),10,10);
	} else if (p.getShape()=="teapot"){
		glPushMatrix();
		glRotatef(90,1,0,0);
		glutSolidTeapot(p.getSize());
		glPopMatrix();
	}
}


//Functions for alternating materials
void generateMaterialList()
{
	materialList.push_back("ruby");
	materialList.push_back("chrome");
	materialList.push_back("emerald");
	materialList.push_back("redplastic");
	materialList.push_back("whiterubber");
}

void selectMaterial(particle p)
{
	if(p.getMaterial()=="ruby"){
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, r_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, r_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, r_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, r_shiny);
	} else if(p.getMaterial()=="chrome"){
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, c_shiny);
	} else if(p.getMaterial()=="emerald"){
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, e_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, e_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, e_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, e_shiny);
	} else if(p.getMaterial()=="redplastic"){
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rp_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rp_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rp_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, rp_shiny);
	} else if(p.getMaterial()=="whiterubber"){
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, w_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, w_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, w_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, w_shiny);
	}
}



//Drawing objects
void objectDraw()
{
	for(int i=0; i<particleList.size(); i++)
	{
		glPushMatrix();
			//set particle position
			glTranslated(particleList[i].getPosition().x,particleList[i].getPosition().y,particleList[i].getPosition().z);
			glPushMatrix();
				//rotate particle and draw
				/*glRotatef(particleList[i].rot[0],1,0,0);
				glRotatef(particleList[i].rot[1],0,1,0);
				glRotatef(particleList[i].rot[2],0,0,1);*/

				//selects the particle material
				selectMaterial(particleList[i]);

				//shapes and draws the particle
				selectShape(particleList[i]);
			glPopMatrix();
		glPopMatrix();
	}

	for(int j=0; j<lightList.size(); j++)
	{
		glPushMatrix();	
			glTranslatef(lightList[j].getPosition().x,lightList[j].getPosition().y,lightList[j].getPosition().z);
			glutSolidSphere(7,8,8);
		glPopMatrix();
	}

	glutPostRedisplay();
}



//Ray-casting to fetch 3D location from 2D coordinates
point3D fetchLocation(int x, int y){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble objX, objY, objZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &objX,&objY,&objZ);

	return point3D(objX,objY,objZ);
}



//Handling keyboard input
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
		particleList.clear();
	}

	//Light source Controls  **CTRL handles light source 1
	if(key == 'u' || key == 'U')
	{
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL)
		{
			lightsource2[0] -= 10;
		}
		else
		{
			lightsource1[0] -= 10;
		}
	}
	if(key == 'i' || key == 'I')
	{
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL)
		{
			lightsource2[0] += 10;
		}
		else
		{
			lightsource1[0] += 10;
		}
	}
	if(key == 'j' || key == 'J')
	{
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL)
		{
			lightsource2[1] -= 10;
		}
		else
		{
			lightsource1[1] -= 10;
		}
	}
	if(key == 'k' || key == 'K')
	{
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL)
		{
			lightsource2[1] += 10;
		}
		else
		{
			lightsource1[1] += 10;
		}
	}
	if(key == 'n' || key == 'N')
	{
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL)
		{
			lightsource2[2] -= 10;
		}
		else
		{
			lightsource1[2] -= 10;
		}
	}
	if(key == 'm' || key == 'M')
	{
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL)
		{
			lightsource2[2] += 10;
		}
		else
		{
			lightsource1[2] += 10;
		}
	}



	//Placing blocks
	if(key==' ')
	{
		createObject(fetchLocation(x, y));
	}

	//Selecting Materials
	if(key=='1')
	{
		materialSelectIndex=0;
	}
	if(key=='2')
	{
		materialSelectIndex=1;
	}
	if(key=='3')
	{
		materialSelectIndex=2;
	}
	if(key=='4')
	{
		materialSelectIndex=3;
	}
	if(key=='5')
	{
		materialSelectIndex=4;
	}

	if(key == '9')
	{
		sceneRotation[0] += 1;
	}
	if(key == '0')
	{
		sceneRotation[0] -= 1;
	}
	if(key == '-')
	{
		sceneRotation[1] += 1;
	}
	if(key == '=')
	{
		sceneRotation[1] -= 1;
	}
}

//Mouse controls
void MouseClick(int btn, int state, int x, int y)
{
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
	if(btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		deleteObject(fetchLocation(x, y));
	}
	if(btn == 3 && state == GLUT_DOWN){
		prevShape();
		cout<<shapeList[shapeSelectIndex];
		printf("\n");
	}
	if(btn == 4 && state == GLUT_DOWN){
		nextShape();
		cout<<shapeList[shapeSelectIndex];
		printf("\n");
	}
}



//Handles camera movement inputs
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
	if(key == GLUT_KEY_PAGE_UP)
	{
		nextShape();
	}
	if(key == GLUT_KEY_PAGE_DOWN)
	{
		prevShape();
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


//updates camera position
void updateCamera(double deltaTime)
{
	//Camera Angles
	if(keyboard[0])
	{
		cameraAngle+=0.5;
	}
	else if(keyboard[1])
	{
		cameraAngle-=0.5;
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

//Idle function - updates camera & lighting
void update(void)
{
	while((lastUpdateTime + CLOCKS_PER_SEC * SECS_PER_TICK < clock()) )
	{
		lastUpdateTime += CLOCKS_PER_SEC*SECS_PER_TICK;
		updateCamera(SECS_PER_TICK);
	}

	//Updates light source position
	GLfloat lightpos1[] = {lightsource1[0],lightsource1[1],lightsource1[2],lightsource1[3]};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);

	GLfloat lightpos2[] = {lightsource2[0],lightsource2[1],lightsource2[2],lightsource2[3]};
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);

	lightList[0].setPosition(lightsource1[0],lightsource1[1],lightsource1[2]);
	lightList[1].setPosition(lightsource2[0],lightsource2[1],lightsource2[2]);

}



//Initial function - Initializes lighting
void init(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	//glEnable(GL_LIGHT1);

	//Adding lights to light list
	lightList.push_back(particle(
		point3D(lightsource1[0],lightsource1[1],lightsource1[2]),
		colour(1.0,1.0,1.0),
		lightsourceSize,
		point3D(1,1,1),
		vec3D(0,0,0),
		"sphere",
		"chrome"
		)
	);
	lightList.push_back(particle(
		point3D(lightsource2[0],lightsource2[1],lightsource2[2]),
		colour(1.0,1.0,1.0),
		lightsourceSize,
		point3D(1,1,1),
		vec3D(0,0,0),
		"sphere",
		"chrome"
		)
	);


	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec);

	glShadeModel(GL_SMOOTH);
}



//Main display function
void display(void)
{
	//Clear the buffers and clears glMatrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
	
	//Sets up projection view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (60.0, 1, 0.1, 500.0);
	glRotatef(sceneRotation[0],1,0,0);
	glRotatef(sceneRotation[1],0,1,0);
	glMatrixMode(GL_MODELVIEW);

	//Camera location & viewing
	gluLookAt(cameraPos[0]*sind(cameraAngle), cameraPos[1]*cosd(cameraAngle),cameraPos[2],0,0,0,0,0,1);
	
	//Drawing of scene
	axis(screenSize[0],screenSize[1], screenSize[0]);
	drawScene(70);

	//Drawing of the objects
	objectDraw();

	//Double buffering
	glutSwapBuffers();
}

//Main function - Program entry point
int main(int argc, char** argv)
{
	//Prints manual to console
	printManual();

	//Needed for rand() to work
	srand(time(0));

	//Initialize GLUT
	glutInit(&argc, argv);

	//Declaring buffers
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	//Initializing screen & creates window
	glutInitWindowSize(screenSize[0], screenSize[1]);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Simpler Modeller");

	//Clearing screen
	glClearColor(0, 0, 0, 0);
	
	//Handles inputs
	glutKeyboardFunc(kbd);
	glutSpecialFunc(SpecialKeyInput);
	glutSpecialUpFunc(SpecialUpFunc);
	glutMouseFunc(MouseClick);

	//Enables depth test for proper z buffering
	glEnable (GL_DEPTH_TEST);

	//Enables backculling
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	//init to initialize lighting
	init();

	//Initialize materialList
	generateMaterialList();

	//Initialize shapelist
	generateShapeList();

	//Registers display callback func & idle callback func
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutIdleFunc(update);

	//Starts event loop
	glutMainLoop();

	//because why not
	return(0);

}