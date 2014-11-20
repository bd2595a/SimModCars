#include "vector.h"
#include <math.h>

Vector::Vector(float xx, float yy)
{
	x=xx; y=yy;
}

Vector* Vector::add(Vector* v)
{
	Vector *vnew=new Vector(x,y);
	vnew->x+=v->x;
	vnew->y+=v->y;
	return vnew;
}

Vector* Vector::sub(Vector* v)
{
	Vector *vnew=new Vector(x,y);
	vnew->x-=v->x;
	vnew->y-=v->y;
	return vnew;
}

Vector* Vector::mul(float f)
{
	Vector *vnew=new Vector(x,y);
	vnew->x*=f;
	vnew->y*=f;
	return vnew;
}

float Vector::dot(Vector* v)
{
	return x*v->x+y*v->y;
}

Vector* Vector::normalize()
{
	Vector *vnew=new Vector(x,y);
	vnew->x=x/sqrt(x*x+y*y);	
	vnew->y=y/sqrt(x*x+y*y);
	return vnew;
}

float Vector::length()
{
	return sqrt(x*x+y*y);
}

float Vector::lengthsquared()
{
	return (x*x+y*y);
}

float Vector::angle()
{
	return atan2(y,x);
}
