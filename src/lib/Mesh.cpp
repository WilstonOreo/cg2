

#ifdef __APPLE__ 
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


bool Mesh::intersect(Ray& ray) 
{
	float tnear, tfar;
	box.intersect(ray,tnear,tfar);
	if (tnear > tfar) return false;
	
	bool found = false;
	recKDTreeTraverse(ray,kdtree,tnear,tfar,found);

/*
	bool found = false;
	for (size_t i = 0; i < triangles.size(); i++)
		if (triangles[i].intersect(ray))
			found = true;*/
	return found;
}


void Mesh::divideNode(KDNode* node, BoundingBox& box, int depth)
{
	if (depth > 15 || node->tris.size() < 10)
	{   // We have a leaf node!
		node->left = NULL; node->right = NULL;
		return;
	}
	node->left = new KDNode();
	node->right = new KDNode();
	node->axis = box.dominantAxis();

	// TODO: Surface Area Heuristic here!
	node->splitPos = 0.5*(box.min.cell[node->axis] + box.max.cell[node->axis]);
	BoundingBox boxLeft, boxRight;
	box.split(node->splitPos,node->axis,boxLeft,boxRight);
	for (size_t i = 0; i < node->tris.size(); i++) 	
	{
		Triangle* tri = node->tris.at(i);
		int result = tri->splitPlaneIntersect(node->splitPos,node->axis);
		if (result & 1) node->left->tris.push_back(tri);
		if (result & 2) node->right->tris.push_back(tri);
	}
	node->tris.clear();
	divideNode(node->left,boxLeft,depth+1);
	divideNode(node->right,boxRight,depth+1);
}

void Mesh::buildKDTree()
{
	cout << "Building KD Tree..." << endl;
	kdtree = new KDNode();
	kdtree->tris.resize(triangles.size());
	for (size_t i = 0; i < triangles.size(); i++)
		kdtree->tris[i] = &triangles[i];
	divideNode(kdtree,box,0);
}

float Mesh::recKDTreeTraverse(Ray& ray, KDNode* node, float tnear, float tfar, bool& found)
{
	if (node->isLeaf())
	{
		for (size_t i = 0; i < node->tris.size(); i++)
			if (node->tris.at(i)->intersect(ray))
				found = true;
		return ray.tmax;
	}

	int k = node->axis;
	float d = (node->splitPos - ray.org.cell[k]) / ray.dir.cell[k];


	KDNode* front = node->left;
	KDNode* back  = node->right;
	if (ray.dir.cell[k] < 0) swap(front,back); 

	if (d <= tnear)
	{
		recKDTreeTraverse(ray,back,tnear,tfar,found);
	} else
	if (d >= tfar)
	{
		recKDTreeTraverse(ray,front,tnear,tfar,found);
	} else
	{
		float t_hit = recKDTreeTraverse(ray,front,tnear,d,found);
		if (t_hit <= d) return t_hit;
		return recKDTreeTraverse(ray,back,d,tfar,found);
	}
}






void Mesh::readOff(string filename)
{
	ifstream is(filename.c_str(), ios::in);
	cout << "Reading mesh from " << filename << " ... " << endl;

	enum { HEADER, SIZE, VERTICES, POLYGONS, END };
	int nRow = 0, mode = HEADER;

	vertices.clear();
	polygons.clear();
	int V,F,E;

	while (is.good() && mode != END)
	{
		char buf[65536];
		is.getline(buf,65536);
		string row(buf);	

		switch(mode)
		{		
			case HEADER:
				if (row=="OFF") { mode++; nRow++; }
				break;

			case SIZE: 
				cout << row << endl;
				V = getCompi(row,0); vertices.resize(V);
				F = getCompi(row,1); polygons.resize(F);
				E = getCompi(row,2);
				cout << "Found " << V << " vertices, " << F << " polygons and " << E << " edges." << endl;
				nRow++; mode++;
				break;
			case VERTICES: {
							   Vertex v; v.v.fromString(row);
							   vertices[nRow-2] = v;
							   nRow++;
							   if (nRow >= V+2) mode++;
							   break;
						   }
			case POLYGONS: {
							   int n = getCompi(row,0);
							   if (n > 0)
							   {
								   polygons[nRow-V-2].resize(n);
								   for (int i = 0; i < n; i++)
								   {
									   int v = getCompi(row,i+1);
									   polygons[nRow-V-2][i] = &vertices[v];
								   }
								   nRow++;
							   }
							   if (nRow >= V+F+2) mode++;
							   break;
						   }
			case END:
						   cout << "End of file reached." << endl;
						   break;
		}
	}
	calcBoundingBox();
	calcNormals();
	calcTriangles();
	buildKDTree();

	is.close();
}

void Mesh::calcBoundingBox()
{
	box.min.set(100000.0f,100000.0f,100000.0f);
	box.max.set(-100000.0f,-100000.0f,-100000.0f);

	for (size_t i = 0; i < vertices.size(); i++)
	{
		vec3f v = vertices[i].v;
		for (int j = 0; j < 3; j++)
		{
			if (v.cell[j] > box.max.cell[j]) box.max.cell[j] = v.cell[j];
			if (v.cell[j] < box.min.cell[j]) box.min.cell[j] = v.cell[j];
		}
	}

	float length = (box.max-box.min).length();
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].v -= box.min+0.5*(box.max-box.min);
		vertices[i].v *= (8.0f / length);
	}

	box.min.set(100000.0f,100000.0f,100000.0f);
	box.max.set(-100000.0f,-100000.0f,-100000.0f);

	for (size_t i = 0; i < vertices.size(); i++)
	{
		vec3f v = vertices[i].v;
		for (int j = 0; j < 3; j++)
		{
			if (v.cell[j] > box.max.cell[j]) box.max.cell[j] = v.cell[j];
			if (v.cell[j] < box.min.cell[j]) box.min.cell[j] = v.cell[j];
		}
	}


}

void Mesh::calcNormals()
{
	for (size_t j = 0; j < polygons.size(); j++)
	{
		Polygon* P = &polygons[j];
		vec3f n = P->getNormal();

		for (size_t k = 0; k < P->size(); k++) 
			P->at(k)->n += n;
	}
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vec3f n = vertices[i].n;
		n.normalize();
		vertices[i].n = n;
	}
}


void Mesh::draw()
{
	for (size_t i = 0; i < polygons.size(); i++)
	{
		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < polygons[i].size(); j++)
		{
			Vertex* V = polygons[i][j];
			glColor3f(0.0,0.0,0.0);

			glNormal3f(V->n.x,V->n.y,V->n.z);		
			glVertex3f(V->v.x,V->v.y,V->v.z);
		}
		glEnd();
	}
}

void Mesh::displayNormals()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex v = vertices[i];
		vec3f n = (box.max-box.min).length()*0.02f*v.n; 

		glPushMatrix();
		glTranslatef(v.v.x,v.v.y,v.v.z);
		glBegin(GL_LINES);
		glColor3f( 0.0, 1.0, 0.0);
		glVertex3f( 0.0 , 0.0 , 0.0 );
		glVertex3f(n.x,n.y,n.z);
		glColor3f( 0.0, 0.0, 0.0);
		glEnd();

		glPopMatrix();
	}
}


