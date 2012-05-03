
class PointCloud : public Mesh, public vector<point3f>
{
	void readOff(string filename);
	void buildKDTree();
	void draw(color3f Color = color3f());

	void divideNode(KDNode* node, BoundingBox& box, int depth);
	vector<Vertex> vertices;
	BoundingBox box;
};

