#include "Particle.h"

std::list<particle> particle::particleList;

particle::particle(float xp, float yp, float zp)
{
	point3D pPosition = point3D(xp,yp,zp);
	cColour = colour();
	iSize = rand()%3;
	point3D pScale = point3D(1,1,1);
	vec3D vDirection = Math3DLib::normalize(vec3D());
	vec3D vRotation = Math3DLib::normalize(vec3D());

	sShape = "cube";
}

particle::particle(point3D inPosition, colour inColour, int inSize, point3D inScale, vec3D inRotation, string inShape)
{
	this->pPosition = inPosition;
	this->cColour = inColour;
	this->iSize = inSize;
	this->pScale = inScale;
	this->vRotation = inRotation;
	this->sShape = inShape;
}

//Set Functions
void particle::setPosition(float x, float y, float z)
{
	this->pPosition.x = x;
	this->pPosition.y = y;
	this->pPosition.z = z;
}
void particle::setColour(float r, float g, float b)
{
	this->cColour.r = r;
	this->cColour.g = g;
	this->cColour.b = b;
}
void particle::setSize(int newSize)
{
	this->iSize = newSize;
}
void particle::setScale(float x, float y, float z)
{
	this->pScale.x = x;
	this->pScale.y = y;
	this->pScale.z = z;
}
void particle::setShape(string newShape)
{
	this->sShape = newShape;
}

//Get Function
point3D particle::getPosition()
{
	return this->pPosition;
}
colour particle::getColour()
{
	return this->cColour;
}
int particle::getSize()
{
	return this->iSize;
}
point3D particle::getScale()
{
	return this->pScale;
}
string particle::getShape()
{
	return this->sShape;
}