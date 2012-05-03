void BoundingBox::intersect(Ray& ray, float& tnear, float& tfar)
{
	tnear = 1000.0;
	tfar = -1000.0;
	for (int i = 0; i < 3; i++)
	{
		if (abs(ray.dir[i]) < 0.001) continue;
		float minV = min.cell[i], maxV = max.cell[i];
		if (ray.dir[i] < 0) swap(minV,maxV);

		float tn = (minV - ray.org.cell[i]) / ray.dir.cell[i];
		float tf = (maxV - ray.org.cell[i] ) / ray.dir.cell[i];

		if (tn < tnear) tnear = tn;
		if (tf > tfar) tfar = tf;
	}
}

