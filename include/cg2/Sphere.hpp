
struct Sphere : public Object
{
    vec3f _c;
    float _r;

    Sphere(const vec3f& center, float radius)
     : _c(center), _r(radius)
    {
    }

	bool intersect(Ray& ray);
	
	vec3f getNormal(const vec3f& iPoint)
	{
		vec3f n = iPoint - _c;
		return n.normalized();
	}

	pair<float,float> getTexCoords(const vec3f& iPoint)
	{
		// from http://www.cse.msu.edu/~cse872/tutorial4.html
		vec3f n = getNormal(iPoint);
		float u = atan2(n.x,n.z) / (2.0*M_PI) + 0.5;
		float v = acosf(n.y)/M_PI;
	
		pair<float,float> texCoord;
		texCoord.first = u;
		texCoord.second = v;
		return texCoord;
	}


    void draw();
};


