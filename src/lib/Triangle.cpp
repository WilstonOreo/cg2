#include "cg2/Triangle.hpp"

#include "tbd/log.h"

namespace cg2 
{
  using std::min;
  using std::max;

  bool Triangle::intersect(Ray &ray)
  {
    int k  = kuv & 3, ku = (kuv >> 2) & 3, kv = (kuv >> 4) & 3;

    float t = (n_d - ray.org[k] - n_u * ray.org[ku] - n_v * ray.org[kv]) /
      (ray.dir[k] + n_u * ray.dir[ku] + n_v * ray.dir[kv]);

//    LOG_MSG_(2) << fmt("% % %") % t % ray.tmin % ray.tmax; 

    if (t <= ray.tmin || t >= ray.tmax) return false;
    float hu = ray.org[ku] + t * ray.dir[ku] - au;
    float hv = ray.org[kv] + t * ray.dir[kv] - av;

    float u = hv * b_nu + hu * b_nv; 
    float v = hu * c_nu + hv * c_nv; 

    if (v < 0 || (u+v > 1.0)) return false; 
    if (u < 0) return false;

    ray.t(t);
    ray.texCoord(u,v);
    ray.u = u; ray.v = v;
    ray.normal = normal(ray);
    ray.obj = this;
    return true;
  }

  void Triangle::build(Vertex* _v0, Vertex* _v1, Vertex* _v2)
  {
    v0 = _v0; v1 = _v1; v2 = _v2;
    Vec3f c = v1->v - v0->v;
    Vec3f b = v2->v - v0->v;
    Vec3f n = c.cross(b).normalized();
    int k = 2;
    Vec3f nAbs(fabs(n.x),fabs(n.y),fabs(n.z));

    if (nAbs.x > nAbs.y)
    { 	if (nAbs.x > nAbs.z) k = 0;
    } else
      if (nAbs.y > nAbs.z) k = 1;

    int u = (k+1) % 3, v = (k+2) % 3;
    kuv = k | (u << 2) | (v << 4);

    au = v0->v[u]; av = v0->v[v];

    n_u = n[u] / n[k];
    n_v = n[v] / n[k];

    Vec3f v_v = v0->v.vec3f();
    n_d = (v_v * n) / n[k];
//    LOG_MSG_(2) << fmt("Built triangle n_duk % % %, k = %, nAbs (% % %)") % n_d % n_u % n_v % k % nAbs.x % nAbs.y % nAbs.z;

    float reci = b[u]*c[v] - b[v]*c[u];
    b_nu =  b[u] / reci;
    b_nv = -b[v] / reci;
    c_nu =  c[v] / reci;
    c_nv = -c[u] / reci;
  }

  int Triangle::splitPlaneIntersect(float splitPos, int axis)
  {
    float minPos = min(v0->v[axis],min(v1->v[axis],v2->v[axis]));
    float maxPos = max(v0->v[axis],max(v1->v[axis],v2->v[axis]));

    if (maxPos < splitPos) return 1;
    if (minPos > splitPos) return 2; 
    return 3;
  }

  Vec3f Triangle::normal(const Ray& ray)
  {
      Vec3f n = (1.0f - ray.u - ray.v)*v0->n + ray.u*v1->n + ray.v*v2->n;
      return n.normalized();
  }

}
