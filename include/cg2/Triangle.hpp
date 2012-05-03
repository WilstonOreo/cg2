struct Triangle : public RaytracableObject
{
	Triangle(Vertex* _v0, Vertex* _v1, Vertex* _v2)
	{
		build(_v0,_v1,_v2);
	}
	float n_u, n_v, n_d;
	int   kuv;

	float b_nu, b_nv;
	float c_nu, c_nv, av, au;

	Vertex *v0, *v1, *v2;

	bool intersect(Ray &ray);

	int splitPlaneIntersect(float splitPos, int axis);

	vec3f getNormal(const vec3f& iPoint) {};
	pair<float,float> getTexCoords(const vec3f& iPoint) {};

	void draw() {}

	vec3f getNormal(Ray& ray)
	{
		vec3f n = (1.0f - ray.u - ray.v)*v0->n + ray.u*v1->n + ray.v*v2->n;
		return n.normalized();
	}

private:
	void build(Vertex* _v0, Vertex* _v1, Vertex* _v2);
};
