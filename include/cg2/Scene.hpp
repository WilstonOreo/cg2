#ifndef _SCENE_HPP
#define _SCENE_HPP
#include <list>
#include <stack>

#include "vec.hpp"
#include "raycast.hpp"
#include "shader.hpp"
#include "image.hpp"

using namespace std;

struct Mesh : public ShadableObject
{

public:
	Mesh() {};

	void displayNormals();

	vec3f getNormal(const vec3f& iPoint)
	{
		vec3f n = iPoint;
		return n.normalized();
	}
	pair<float,float> getTexCoords(const vec3f& iPoint)
	{
		pair<float,float> texCoord;

		return texCoord;
	}


private:
	void calcNormals();
	void calcBoundingBox();

	vector<Vertex> vertices;
	vector<Polygon> polygons;

	BoundingBox box;
};


struct TriangleMesh : public IntersectableObject
{
	KDTree kdtree;
	vector<Triangle> triangles;
	bool intersect(Ray& ray); 

	void calcTriangles()
	{
		for (size_t i = 0; i < polygons.size(); i++)
		{
			Polygon* p = &polygons[i];
			if (p->size()==3)
			{
				Triangle tri(p->at(0),p->at(1),p->at(2));
				tri.shader = shader;
				triangles.push_back(tri);
			}
		}
	}

};





struct Scene
{
	Scene() { maxBounce = 15; }

	void addObject(Object* obj) { objects.push_back(obj); }

	void castRays(vector<Ray>& rays, Image& outImage);
	void castRays(vector<Ray>& rays);

	vec3f traceRay(Ray& ray, Object* curObj = NULL);
	bool traceShadowRay(Ray& shadowRay, Object* curObj = NULL);

	unsigned maxBounce;

	vector<Object*> objects;
	vector<Light*>   lights;
	Camera* camera;
	FrameBuffer frameBuf;
};

#endif
