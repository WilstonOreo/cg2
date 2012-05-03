Ray Camera::generateRay(float x, float y)
{
	Ray ray; 
	ray.org.set(m.get(3,0),m.get(3,1),m.get(3,2));
	ray.dir = vec3f(x,y,1.0f) * m - ray.org;
	ray.setScreenPos(x,y);

	return ray;
}

vector<Ray> Camera::generatePrimaryRays(int scrWidth, int scrHeight)
{
	vector<Ray> rays(scrWidth*scrHeight);
/*
	for (int y = 0; y < scrHeight; y++)
		for (int x = 0; x < scrWidth; x++)
		{
			Ray ray = generateRay(x,y);
			rays[y*scrWidth+x] = ray;
		}*/
	return rays;
}
