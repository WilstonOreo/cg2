

struct Slice : 
{
	KDTree kdtree
};

struct SlicableMesh : public Mesh 
{
	Slices buildSlices();
};
