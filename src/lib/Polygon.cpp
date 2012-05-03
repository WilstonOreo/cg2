
vec3f Polygon::getNormal()
{
	vec3f n; int count = 0;
	for (int i = 0; i < size()-2; i++)
	{
		vec3f a = at(i)->v,b = at(i+2)->v,c = at(i+1)->v;
		n += (a-b).cross(c-b);		
		count++;
	}
	if (count > 0) n.normalize();
	return n;
}
