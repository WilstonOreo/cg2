class Polygon : public vector<Vertex*> 
{
public:
	vec3f getNormal();
};


struct Mesh : public Object
{

public:
	Mesh() {};

	void readOff(string filename);
	void draw();

	void displayNormals();

private:
	void calcNormals();
	void calcBoundingBox();

	vector<Vertex> vertices;
	vector<Polygon> polygons;

	// Bounding box points
	vec3f min, max;
};


