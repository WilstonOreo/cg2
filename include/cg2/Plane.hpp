
struct Plane : public Object
{
    vec3f _c;
    vec3f _n;
    float _w;
    
    Plane(const vec3f& center, const vec3f& normal, float width)
     : _c(center), _n(normal), _w(width)
    {

    }

	bool intersect(Ray& ray); 

	vec3f getNormal(const vec3f& iPoint)
	{
		return _n;
	}

	pair<float,float> getTexCoords(const vec3f& iPoint)
	{
		pair<float,float> texCoord;
		vec3f p = iPoint - _c;
		float v;
		texCoord.first 	= modf(p.x / _w,&v);
		texCoord.second = modf(p.z / _w,&v);
		return texCoord;
	}
    
	void draw();
};


