#ifndef RAYCAST_HPP
#define RAYCAST_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
#include <GL/glut.h>

#include "vec.hpp"

typedef unsigned int u32;



class Object;
class Light;

struct Ray
{
	Ray(vec3f _org = vec3f(), vec3f _dir = vec3f(), float _scrPosX = 0.0, float _scrPosY = 0.0): 
		org(_org), dir(_dir) 
	{
		tmin = 0.002; tmax = 10000.0;
		obj = NULL;
		bounce = 0;
		u = 0.0; v = 0.0;
		scrPosX = _scrPosX; scrPosY = _scrPosY;
	}

	void init(vec3f _org = vec3f(), vec3f _dir = vec3f(), float _scrPosX = 0, float _scrPosY = 0)
	{
		org = _org; dir = _dir; setScreenPos(_scrPosX,_scrPosY);
	}

	bool t(float _t) 
	{ 
		if (_t >= tmin && _t < tmax)
		{
			tmax = _t;
			return true;
		}
		return false;
	}

	string toString() { stringstream ss; ss <<"Org: "<<org.toString();
						ss << "Dir: " << dir.toString();
						ss << "Tmin, Tmax" << tmin << "," << tmax;
						return ss.str(); }

	void drawPoint();
	Ray reflect();
	Ray refract(float index);

	void texCoord(float _u, float _v) { u =_u; v =_v; }
	void texCoord(pair<float,float> _t) { texCoord(_t.first,_t.second); }
	void setScreenPos(float _scrPosX, float _scrPosY)
	{
		scrPosX = _scrPosX; scrPosY = _scrPosY;
	}
	void drawIntersectionPoint();		
	point3f getIntersectionPoint() { return org + tmax*dir; }

	vec3f org, dir;
	float tmin, tmax;
	vec3f normal,color;
	Object* obj;
	unsigned bounce;

	float u,v;

	float scrPosX, scrPosY;
};





#endif
