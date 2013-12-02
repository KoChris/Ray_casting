#include <windows.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Math3DLib.h"
#include "Particle.h"
#include "TrigLib.h"

//Time declaration for update func
#define SECS_PER_TICK 0.0083
clock_t lastUpdateTime;

using namespace std;

//Declaring Variables

//Screen size for glut
int screenSize[2] = {800,800};

//Initial camera position
int cameraPos[3] = {300, 300, 300};
float cameraAngle = 0;

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
float lastx, lasty;
bool mouseActive;

//Scene Rotation angle
float sceneRotation[2];

//Initial light source variable
float lightsource1[4] = {0.0,0.0,150.0,1.0};
float lightsource2[4] = {0,-100.0,110.0,1.0};
float lightsourceSize = 5.0;

//Initial particle size
int particleSizeDefault = 5;

//Variables for keyboard
bool keyboard[4]; //Arrow Keys
bool* keySpecialStates = new bool[256];

//Index for different shapes
int shapeSelectIndex = 0;

//Index for different materials
int materialSelectIndex = 0;

//Index for selected object
int selectedObjectIndex = -1;

//Declaring lists
std::vector<particle> particleList;
std::vector<particle> lightList;
std::vector<string> shapeList;
std::vector<string> materialList;

//Light variables
float amb[4] = {1.0, 1, 1, 1};
float diff[4] = {1,1,1, 1};
float spec[4] = {1,1,1, 0};

//Material Variables

//Cyan Plastic
float cp_amb[] = {0.0,0.1,0.06,1.0};
float cp_dif[] = {0.0,0.50980392,0.50980392, 1.0};
float cp_spec[] = {0.50196078,0.50196078,.50196078,1.0};
float cp_shiny = 0.25;

//Copper
float c_amb[] = {0.19125,0.0735,0.0225, 1.0};
float c_dif[] = {0.7038,0.27048,0.0828, 1.0};
float c_spec[] = {0.256777,0.137622,0.086014, 1.0};
float c_shiny = 0.1;

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
	printf("Program\n");
	printf("Q - Quit program\n");
	printf("R - Reset program\n");
	printf("O - Save\n");
	printf("L - Load\n");
	printf("\n");
	printf("Placing Objects\n");
	printf("Spacebar - Place objects\n");
	printf("Mouse Wheel - Cycle through shapes\n");
	printf("PAGE_UP - Cycle through shapes\n");
	printf("PAGE_DOWN - Cycle through shapes\n");
	printf("\n");
	printf("Material\n");
	printf("] - Apply current selected material to pointed object\n");
	printf("NumberKey (1-5) - Different Materials\n");
	printf("	1: Cyan Plastic\n");
	printf("	2: Copper\n");
	printf("	3: Emerald\n");
	printf("	4: Red Plastic\n");
	printf("	5: White Rubber\n");	
	printf("\n");
	printf("Object Selection\n");
	printf("LMB - Select an object to transform\n");
	printf("[ - Deselects the selected object\n");
	printf("RMB - Delete the pointed object\n");
	printf("\n");
	printf("Object Scaling\n");
	printf("< - Scale down object in x axis\n");
	printf("> - Scale up object in x axis\n");
	printf("Ctrl + < - Scale down object in y axis\n");
	printf("Ctrl + > - Scale up object in y axis\n");
	printf("Alt + < - Scale down object in z axis\n");
	printf("Alt + > - Scale up object in z axis\n");
	printf("\n");
	printf("Scene Rotation\n");
	printf("9 & 0 - Rotate around x axis\n");
	printf("- & = - Rotate around y axis\n");
	printf("\n");
	printf("Object Rotation\n");
	printf("Z - Positive rotation around z axis\n");
	printf("X - Positive rotation around x axis\n");
	printf("C - Positive rotation around y axis\n");
	printf("Ctrl + Z - Negative rotation around z axis\n");
	printf("Ctrl + X - Negative rotation around x axis\n");
	printf("Ctrl + C - Negative rotation around y axis\n");
	printf("\n");
	printf("Object Translation\n");
	printf("Arrow Keys\n");
	printf("Left  - Translate along positive x axis\n");
	printf("Right - Translate along Negative x axis\n");
	printf("Up    - Translate along Negative y axis\n");
	printf("Down  - Translate along positive y axis\n");
	printf("Ctrl + Up    - Translate along Negative z axis\n");
	printf("Ctrl + Down  - Translate along positive z axis\n");
	printf("\n");
	printf("Camera Control\n");
	printf("~ - Toggle camera control\n");
	printf("WASD Keys\n");
	printf("W - Move forwards\n");
	printf("A - Move left\n");
	printf("S - Move backwards\n");
	printf("D - Move right\n");
	printf("\n");
	printf("Lighting Control\n");
	printf("Camera 1\n");
	printf("U - Translate Lightsource 1 along Positive x axis\n");
	printf("I - Translate Lightsource 1 along Negative x axis\n");
	printf("J - Translate Lightsource 1 along Positive y axis\n");
	printf("K - Translate Lightsource 1 along Negative y axis\n");
	printf("N - Translate Lightsource 1 along Positive z axis\n");
	printf("M - Translate Lightsource 1 along Negative z axis\n");
	printf("Camera 2\n");
	printf("Ctrl + U - Translate Lightsource 2 along Positive x axis\n");
	printf("Ctrl + I - Translate Lightsource 2 along Negative x axis\n");
	printf("Ctrl + J - Translate Lightsource 2 along Positive y axis\n");
	printf("Ctrl + K - Translate Lightsource 2 along Negative y axis\n");
	printf("Ctrl + N - Translate Lightsource 2 along Positive z axis\n");
	printf("Ctrl + M - Translate Lightsource 2 along Negative z axis\n");
}

//Saves the current scene
void save()
{
	string file = "";
	printf("Enter the name of the file you would like to save to (ending in .txt)\n");
	getline(cin, file);

	ofstream saveTo (file);
	if(saveTo.is_open())
	{
		saveTo << cameraAngle;
		saveTo << "\n";
		saveTo << xpos;
		saveTo << "\n";
		saveTo << ypos;
		saveTo << "\n";
		saveTo << zpos;
		saveTo << "\n";
		saveTo << xrot;
		saveTo << "\n";
		saveTo << yrot;
		saveTo << "\n";
		saveTo << angle;
		saveTo << "\n";
		saveTo << mouseActive;
		saveTo << "\n";
		saveTo << sceneRotation[0];
		saveTo << "\n";
		saveTo << sceneRotation[1];
		saveTo << "\n";
		saveTo << lightsource1[0];
		saveTo << "\n";
		saveTo << lightsource2[0];
		saveTo << "\n";
		saveTo << lightsource1[1];
		saveTo << "\n";
		saveTo << lightsource2[1];
		saveTo << "\n";
		saveTo << lightsource1[2];
		saveTo << "\n";
		saveTo << lightsource2[2];
		saveTo << "\n";
		saveTo << shapeSelectIndex;
		saveTo << "\n";
		saveTo << materialSelectIndex;
		saveTo << "\n";
		saveTo << selectedObjectIndex;
		saveTo << "\n";
		
		for(int i=0; i < particleList.size(); i++)
		{
			saveTo << particleList[i].getPosition().x;
			saveTo << "\n";
			saveTo << particleList[i].getPosition().y;
			saveTo << "\n";
			saveTo << particleList[i].getPosition().z;
			saveTo << "\n";
			saveTo << particleList[i].getScale().x;
			saveTo << "\n";
			saveTo << particleList[i].getScale().y;
			saveTo << "\n";
			saveTo << particleList[i].getScale().z;
			saveTo << "\n";
			saveTo << particleList[i].getRotation().x;
			saveTo << "\n";
			saveTo << particleList[i].getRotation().y;
			saveTo << "\n";
			saveTo << particleList[i].getRotation().z;
			saveTo << "\n";
			saveTo << particleList[i].getShape();
			saveTo << "\n";
			saveTo << particleList[i].getMaterial();
			saveTo << "\n";
		}
		saveTo.close();
	}
	else 
	{
		printf("Unable to save to file\n");
	}
}

//Loads the current scene
void load()
{
	string file = "";
	printf("Enter the name of the file you would like to load from (ending in .txt)\n");
	getline(cin, file);

	string line;
	ifstream loadFrom (file);
	if(loadFrom.is_open())
	{
		getline(loadFrom,line);
		cameraAngle = ::atof(line.c_str());
		getline(loadFrom,line);
		xpos = ::atof(line.c_str());
		getline(loadFrom,line);
		ypos = ::atof(line.c_str());
		getline(loadFrom,line);
		zpos = ::atof(line.c_str());
		getline(loadFrom,line);
		xrot = ::atof(line.c_str());
		getline(loadFrom,line);
		yrot = ::atof(line.c_str());
		getline(loadFrom,line);
		angle = ::atof(line.c_str());
	
		getline(loadFrom,line);
		//saveTo << mouseActive;
		
		getline(loadFrom,line);
		sceneRotation[0] = ::atof(line.c_str());
		getline(loadFrom,line);
		sceneRotation[1] = ::atof(line.c_str());
		getline(loadFrom,line);
		lightsource1[0] = ::atof(line.c_str());
		getline(loadFrom,line);
		lightsource2[0] = ::atof(line.c_str());
		getline(loadFrom,line);
		lightsource1[1] = ::atof(line.c_str());
		getline(loadFrom,line);
		lightsource2[1] = ::atof(line.c_str());
		getline(loadFrom,line);
		lightsource1[2] = ::atof(line.c_str());
		getline(loadFrom,line);
		lightsource2[2] = ::atof(line.c_str());
		getline(loadFrom,line);
		shapeSelectIndex = ::atoi(line.c_str());
		getline(loadFrom,line);
		materialSelectIndex = ::atoi(line.c_str());
		getline(loadFrom,line);
		selectedObjectIndex = ::atoi(line.c_str());
		while ( getline (myfile,line) )
		{
			float posX, posY, posZ, scaleX, scaleY, scaleZ, rotX, rotY, rotZ;
			string shape, material;
			posX = ::atof(line.c_str());
			getline(loadFrom,line);
			posY = ::atof(line.c_str());
			getline(loadFrom,line);
			posZ = ::atof(line.c_str());
			getline(loadFrom,line);
			scaleX = ::atof(line.c_str());
			getline(loadFrom,line);
			scaleY = ::atof(line.c_str());
			getline(loadFrom,line);
			scaleZ = ::atof(line.c_str());
			getline(loadFrom,line);
			rotX = ::atof(line.c_str());
			getline(loadFrom,line);
			rotY = ::atof(line.c_str());
			getline(loadFrom,line);
			rotZ = ::atof(line.c_str());
			getline(loadFrom,line);
			shape = line;
			particleList.push_back(particle(point3D(posX,posY,posZ),colour(0,0,0),particleSizeDefault,point3D(scaleX,scaleY,scaleZ),vec3D(rotX,rotY,rotZ),shape,line));
		}
		loadFrom.close();
	}
	else
	{
		printf("Unable to load file\n");
	}
}

//Draws the axis
void axis(int sizeX, int sizeY, int sizeZ)
{
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

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cp_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cp_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cp_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, cp_shiny);
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

//Selection of objects
void selectObject(point3D pos)
{
	if(!particleList.empty())
	{
		for(int i=0; i < particleList.size(); i++)
		{
			if(Math3DLib::distance(particleList[i].getPosition(),pos)<=particleList[i].getSize())
			{
				selectedObjectIndex = i;
				break;
			}
		}
		glutPostRedisplay();
	}
}

void drawBoundingBox(particle p)
{
	float box_col[] = {0,1,0, 1.0};
	float box_shine = 1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, box_col);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, box_col);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, box_col);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, box_shine);

	if(p.getShape()=="cube")
	{
		glutWireCube(p.getSize()+0.5);
	}
	else if (p.getShape()=="sphere")
	{
		glutWireSphere(p.getSize()+0.5,30,30);
	}
	else if (p.getShape()=="cone")
	{
		glutWireCone((p.getSize()+0.5)/2,p.getSize()+0.5,30,30);
	}
	else if (p.getShape()=="torus")
	{
		glutWireTorus((p.getSize()+0.5)/2,p.getSize()+0.5,10,10);
	}
	else if (p.getShape()=="teapot")
	{
		glPushMatrix();
		glRotatef(90,1,0,0);
		glutWireTeapot(p.getSize()+0.5);
		glPopMatrix();
	}	
}

//Rotation of objects
void rotateObject(string s)
{
	if (selectedObjectIndex != -1)
	{
		if(s == "x")
		{
			particleList[selectedObjectIndex].setRotation(vec3D(particleList[selectedObjectIndex].getRotation().x + 0.5,particleList[selectedObjectIndex].getRotation().y,particleList[selectedObjectIndex].getRotation().z));
		}
		else if (s=="y")
		{
			particleList[selectedObjectIndex].setRotation(vec3D(particleList[selectedObjectIndex].getRotation().x,particleList[selectedObjectIndex].getRotation().y + 0.5,particleList[selectedObjectIndex].getRotation().z));
		}
		else if (s=="z")
		{
			particleList[selectedObjectIndex].setRotation(vec3D(particleList[selectedObjectIndex].getRotation().x,particleList[selectedObjectIndex].getRotation().y,particleList[selectedObjectIndex].getRotation().z + 0.5));
		}
		else if(s == "-x")
		{
			particleList[selectedObjectIndex].setRotation(vec3D(particleList[selectedObjectIndex].getRotation().x - 0.5,particleList[selectedObjectIndex].getRotation().y,particleList[selectedObjectIndex].getRotation().z));
		}
		else if (s=="-y")
		{
			particleList[selectedObjectIndex].setRotation(vec3D(particleList[selectedObjectIndex].getRotation().x,particleList[selectedObjectIndex].getRotation().y - 0.5,particleList[selectedObjectIndex].getRotation().z));
		}
		else if (s=="-z")
		{
			particleList[selectedObjectIndex].setRotation(vec3D(particleList[selectedObjectIndex].getRotation().x,particleList[selectedObjectIndex].getRotation().y,particleList[selectedObjectIndex].getRotation().z - 0.5));
		}
	}
}

//Translation of objects
void translateObject(string s)
{
	if (selectedObjectIndex != -1)
	{
		if(s == "x")
		{
			particleList[selectedObjectIndex].setPosition(particleList[selectedObjectIndex].getPosition().x + 0.5,particleList[selectedObjectIndex].getPosition().y,particleList[selectedObjectIndex].getPosition().z);
		}
		else if (s=="y")
		{
			particleList[selectedObjectIndex].setPosition(particleList[selectedObjectIndex].getPosition().x,particleList[selectedObjectIndex].getPosition().y + 0.5,particleList[selectedObjectIndex].getPosition().z);
		}
		else if (s=="z")
		{
			particleList[selectedObjectIndex].setPosition(particleList[selectedObjectIndex].getPosition().x,particleList[selectedObjectIndex].getPosition().y,particleList[selectedObjectIndex].getPosition().z + 0.5);
		} 
		else if(s == "-x")
		{
			particleList[selectedObjectIndex].setPosition(particleList[selectedObjectIndex].getPosition().x - 0.5,particleList[selectedObjectIndex].getPosition().y,particleList[selectedObjectIndex].getPosition().z);
		}
		else if (s=="-y")
		{
			particleList[selectedObjectIndex].setPosition(particleList[selectedObjectIndex].getPosition().x,particleList[selectedObjectIndex].getPosition().y - 0.5,particleList[selectedObjectIndex].getPosition().z);
		} 
		else if (s=="-z")
		{
			particleList[selectedObjectIndex].setPosition(particleList[selectedObjectIndex].getPosition().x,particleList[selectedObjectIndex].getPosition().y,particleList[selectedObjectIndex].getPosition().z - 0.5);
		}
	}
}

//Scaling of objects
void scaleObject(string s)
{
	if (selectedObjectIndex != -1)
	{
		if(s == "x")
		{
			particleList[selectedObjectIndex].setScale(particleList[selectedObjectIndex].getScale().x + 0.5,particleList[selectedObjectIndex].getScale().y,particleList[selectedObjectIndex].getScale().z);
		} 
		else if (s=="y")
		{
			particleList[selectedObjectIndex].setScale(particleList[selectedObjectIndex].getScale().x,particleList[selectedObjectIndex].getScale().y + 0.5,particleList[selectedObjectIndex].getScale().z);
		}
		else if (s=="z")
		{
			particleList[selectedObjectIndex].setScale(particleList[selectedObjectIndex].getScale().x,particleList[selectedObjectIndex].getScale().y,particleList[selectedObjectIndex].getScale().z + 0.5);
		}
		else if(s == "-x")
		{
			particleList[selectedObjectIndex].setScale(particleList[selectedObjectIndex].getScale().x - 0.5,particleList[selectedObjectIndex].getScale().y,particleList[selectedObjectIndex].getScale().z);
		}
		else if (s=="-y")
		{
			particleList[selectedObjectIndex].setScale(particleList[selectedObjectIndex].getScale().x,particleList[selectedObjectIndex].getScale().y - 0.5,particleList[selectedObjectIndex].getScale().z);
		}
		else if (s=="-z")
		{
			particleList[selectedObjectIndex].setScale(particleList[selectedObjectIndex].getScale().x,particleList[selectedObjectIndex].getScale().y,particleList[selectedObjectIndex].getScale().z - 0.5);
		}
	}
}

//Creating and Deleting of objects
void createObject(point3D pos)
{
	particleList.push_back(particle(pos,colour(180,120,240),particleSizeDefault,point3D(1,1,1),vec3D(0,0,0),shapeList[shapeSelectIndex],materialList[materialSelectIndex]));
}

void deleteObject(point3D pos)
{
	if(!particleList.empty())
	{
		for(int i=0; i < particleList.size(); i++)
		{
			if(Math3DLib::distance(particleList[i].getPosition(),pos)<=particleList[i].getSize())
			{
				particleList.erase(particleList.begin()+i);
				if(i = selectedObjectIndex)
				{
					selectedObjectIndex = -1;
				}
				break;
			}
		}
		glutPostRedisplay();
	}
}

//Modifying object material
void modifyObjectMaterial(point3D pos)
{
		if(!particleList.empty())
		{
		for(int i=0; i < particleList.size(); i++)
		{
			if(Math3DLib::distance(particleList[i].getPosition(),pos)<=particleList[i].getSize())
			{
				particleList[i].setMaterial(materialList[materialSelectIndex]);
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
	printf("Current shape: ");
	cout<<shapeList[shapeSelectIndex];
	printf("\n");
}

void prevShape()
{
	shapeSelectIndex--;
	if(shapeSelectIndex<0)
	{
		shapeSelectIndex = shapeList.size()-1;
	}
	printf("Current shape: ");
	cout<<shapeList[shapeSelectIndex];
	printf("\n");
}

void selectShape(particle p)
{
	if(p.getShape()=="cube")
	{
		glutSolidCube(p.getSize());
	}
	else if (p.getShape()=="sphere")
	{
		glutSolidSphere(p.getSize(),30,30);
	}
	else if (p.getShape()=="cone")
	{
		glutSolidCone(p.getSize()/2,p.getSize(),30,30);
	}
	else if (p.getShape()=="torus")
	{
		glutSolidTorus(p.getSize()/2,p.getSize(),10,10);
	}
	else if (p.getShape()=="teapot")
	{
		glPushMatrix();
		glRotatef(90,1,0,0);
		glutSolidTeapot(p.getSize());
		glPopMatrix();
	}
}

//Functions for alternating materials
void generateMaterialList()
{
	materialList.push_back("cyanplastic");
	materialList.push_back("copper");
	materialList.push_back("emerald");
	materialList.push_back("redplastic");
	materialList.push_back("whiterubber");
}

void selectMaterial(particle p)
{
	if(p.getMaterial()=="cyanplastic")
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cp_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cp_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cp_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, cp_shiny);
	}
	else if(p.getMaterial()=="copper")
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, c_shiny);
	}
	else if(p.getMaterial()=="emerald")
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, e_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, e_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, e_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, e_shiny);
	}
	else if(p.getMaterial()=="redplastic")
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rp_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rp_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rp_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, rp_shiny);
	}
	else if(p.getMaterial()=="whiterubber")
	{
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
				//scale particle
				glScalef(particleList[i].getScale().x,particleList[i].getScale().y,particleList[i].getScale().z);
				glPushMatrix();
					//rotate particle and draw
					glRotatef(particleList[i].getRotation().x,1,0,0);
					glRotatef(particleList[i].getRotation().y,0,1,0);
					glRotatef(particleList[i].getRotation().z,0,0,1);

					//selects the particle material
					selectMaterial(particleList[i]);

					//shapes and draws the particle
					selectShape(particleList[i]);

					if(i == selectedObjectIndex){
						drawBoundingBox(particleList[i]);
					}
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rp_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rp_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rp_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, rp_shiny);

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
point3D fetchLocation(int x, int y)
{
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
	//if the "q" or esc key is pressed, quit the program
	if(key == 'q' || key == 'Q' || key == 27)
	{
		exit(0);
	}
	if(key == 'f' || key == 'F')
	{
		
	}
	if(key == 'r' || key == 'R')
	{
		selectedObjectIndex = -1;
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

	if(key == '`')
	{
		mouseActive = !mouseActive;
	}

	//Camera movement
	    if (key=='w')
    {
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f); 
		xpos += float(sin(yrotrad)) * 1.5;
		zpos -= float(cos(yrotrad)) * 1.5;
		ypos -= float(sin(xrotrad)) * 1.5;
    }

    if (key=='s')
    {
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f); 
		xpos -= float(sin(yrotrad)) * 1.5;
		zpos += float(cos(yrotrad)) * 1.5;
		ypos += float(sin(xrotrad)) * 1.5;
    }

    if (key=='d')
    {
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos += float(cos(yrotrad)) * 2;
		zpos += float(sin(yrotrad)) * 2;
    }

    if (key=='a')
    {
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos -= float(cos(yrotrad)) * 2;
		zpos -= float(sin(yrotrad)) * 2;
    }
	
	//Object deselect
	if(key=='[')
	{
		selectedObjectIndex = -1;
	}
	
	//Material modification
	if(key==']')
	{
		modifyObjectMaterial(fetchLocation(x, y));
	}

	//Transformations
	if (key=='z')
	{
		rotateObject("z");
	}
	if (key=='x')
	{
		rotateObject("x");
	}
	if (key=='c')
	{
		rotateObject("y");
	}
}

//Mouse passive movement
void mouseMovement(int x, int y) 
{
	if(mouseActive)
	{
		int diffx=x-lastx; //check the difference between the current x and the last x position
		int diffy=y-lasty; //check the difference between the current y and the last y position
		lastx=x; //set lastx to the current x position
		lasty=y; //set lasty to the current y position
		xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
		yrot += (float) diffx; //set the xrot to yrot with the addition of the difference in the x position
	}
}

//Mouse controls
void MouseClick(int btn, int state, int x, int y)
{
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		selectObject(fetchLocation(x, y));
	}
	if(btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		deleteObject(fetchLocation(x, y));
	}
	if(btn == 3 && state == GLUT_DOWN){
		prevShape();
	}
	if(btn == 4 && state == GLUT_DOWN){
		nextShape();
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

void camera (void)
{
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screento the position of our camera
}

//Initial function - Initializes lighting
void init(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1);

	//Adding lights to light list
	lightList.push_back(particle(
		point3D(lightsource1[0],lightsource1[1],lightsource1[2]),
		colour(1.0,1.0,1.0),
		lightsourceSize,
		point3D(1,1,1),
		vec3D(0,0,0),
		"sphere",
		"copper"
		)
	);
	lightList.push_back(particle(
		point3D(lightsource2[0],lightsource2[1],lightsource2[2]),
		colour(1.0,1.0,1.0),
		lightsourceSize,
		point3D(1,1,1),
		vec3D(0,0,0),
		"sphere",
		"copper"
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
	glMatrixMode(GL_MODELVIEW);

	camera();

	//Camera location & viewing
	gluLookAt(cameraPos[0]*sind(cameraAngle), cameraPos[1]*cosd(cameraAngle),cameraPos[2],0,0,0,0,0,1);
	
	glRotatef(sceneRotation[0],1,0,0);
	glRotatef(sceneRotation[1],0,1,0);

	//Drawing of scene
	//axis(screenSize[0],screenSize[1], screenSize[0]);
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
	glutPassiveMotionFunc(mouseMovement);
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