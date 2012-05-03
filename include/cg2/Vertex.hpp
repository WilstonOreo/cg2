
namespace cg2 
{
  struct Vertex
  {
    Vertex(point3f _v = point3f(), vec3f _n = vec3f()): v(_v), n(_n) {}
    void set(point3f _v, vec3f _n) { v = _v; n = _n; }

    point3f v;
    vec3f n;
    bool hasNormal() { return (n.x != 0.0f) || (n.y != 0.0f) || (n.z != 0.0f); }
  };

  typedef vector<Vertex*> Vertices;
  typedef vector<Vertex> VertexList;

}
