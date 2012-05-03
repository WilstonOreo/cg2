
void Sphere::draw()
{
	glPushMatrix();
	glTranslatef(_c[0], _c[1], _c[2]);
	glutSolidSphere(_r, 32, 32);
	glPopMatrix();
}

bool Sphere::intersect(Ray& ray)
{
	vec3f o = ray.org - _c;
	float a = ray.dir * ray.dir;
	float b = 2.0f * (ray.dir * o);
	float c = o*o - _r*_r;
	float disc = b*b - 4*a*c;

	if (disc < 0) return false;

	float discSqrt = sqrtf(disc);
	float q = (b < 0) ? (-b - discSqrt) : (-b + discSqrt);
	q *= 0.5f;

	float t0 = q/a, t1 = c/q;
	if (t0 > t1) swap(t0,t1);
	if (t1 < 0) return false;

	if (!ray.t((t0 < 0) ? t1 : t0)) return false;

	ray.obj = this;
	ray.normal = getNormal(ray.getIntersectionPoint());
	return true;
}


