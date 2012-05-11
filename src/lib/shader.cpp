#include "cg2/Shader.hpp"

#include <stdlib.h>
#include <iostream>
#include "cg2/Scene.hpp"

using namespace std;

namespace cg2
{
  void Texture::init(string filename)
  {
    Image img(filename);
    mipMaps.clear();
    mipMaps.push_back(img);

    // Generate MipMaps
    int level = 0;

    while ((img.width() > 2) && (img.height() > 2))
    {
      cout << level << endl;
      Image dsImg(img.width()/2,img.height()/2); // Downsized image (by factor of 2)

      for (unsigned y = 0; y < dsImg.height(); y++)
        for (unsigned x = 0; x < dsImg.width(); x++)
        {
          Color color = 0.25 * ( img(x*2,y*2)  + img(x*2+1,y*2) +
              img(x*2,y*2+1)+ img(x*2+1,y*2+1) );
          dsImg.set(x,y,color); 
        }
      img = dsImg;
      mipMaps.push_back(img);

      level++;
    }
  }

  Vec3f Texture::operator() (float x, float y, float mipMapLevel)
  {
    /*	switch (texFilter)
        {
        case TF_NONE:
    // No filtering
    break;

    case TF_BILINEAR:
    // Bilinear filtering

    break;

    case TF_TRILINEAR:
    */		// Trilinear filtering

    if (mipMapLevel < 0) mipMapLevel = 0;
    if (mipMapLevel >= mipMaps.size()-1) 
      mipMapLevel  = mipMaps.size()-2;

    Image* img = &mipMaps[(int)mipMapLevel];
    int w = img->width(), h = img->height(); 
    float px = x*w, py = y*h; Vec3f color[2];
    float idx = 0; float fracL = modf( mipMapLevel, &idx);

    int i = 0;
    //for (int i = 0; i < 2; i++)
    {
      int mIdx = int(idx) + i;
      float fracX = modf( px, &px ), fracY = modf(py, &py);
      u32 X = u32(px), Y = u32(py);

      color[i] = ((1.0-fracY)*((1.0-fracX)*img->get(X % w,Y % h) + 
            (    fracX)*img->get((X+1) % w,Y % h)) +
          (    fracY)*((1.0-fracX)*img->get(X % w,(Y+1) % h) + 
            (    fracX)*img->get((X+1) % w,(Y+1) % h)));
      px /= 2; py /= 2;
    }
    // Blend colors

    return color[0];
    //	return fracL*color[0] + (1.0-fracL)*color[1];
    /*	}

        return Vec3f(0.0f,0.0f,0.0f); */
  }


  Vec3f PhongShader::shade(Ray& ray)
  {
    if (!scene) return Vec3f();

    vector<Light*>::iterator lightIt;
    Vec3f color;
    Point3f iPoint = ray.getIntersectionPoint();
    Vec3f N = ray.normal.normalized();

    for (lightIt = scene->lights.begin(); lightIt != scene->lights.end(); ++lightIt)
    {
      Light* light = *lightIt;
      if (!light) { color += Vec3f(0.5,0.5,0.5); continue; }

      float invlightDist = 1.0f / (light->pos() - iPoint).length();
      invlightDist *= invlightDist * light->intensity();

      Vec3f L = ( light->pos()- iPoint).normalized();
      color += ambient % light->ambient() * invlightDist;

      float angle = N * L;

      if (angle > 0.0f)
      {	
        Vec3f diff = light->diffuse() % diffuse * angle * invlightDist;
        Vec3f E = ray.org.vec3f().normalized();
        L = -L;
        float dotLN = N * L;
        Vec3f R = L - (2.0 * dotLN * N);

        float spec = pow(max( R * E , 0.0f ), shininess );
        diff += light->specular() % specular * spec * invlightDist;

        if (light->shadows() > 0.0f)
        {
          Point3f rndPos(light->pos().x-(RND-0.5)*light->radius(),
              light->pos().y-(RND-0.5)*light->radius(),
              light->pos().z-(RND-0.5)*light->radius());

          Ray shadowRay(rndPos,iPoint-rndPos);
          shadowRay.tmin = 0.01;
          shadowRay.tmax = 0.99;

          if (scene->traceShadowRay(shadowRay,(SceneObject*)ray.obj))
            diff *=  (1.0 - light->shadows()*(1.0-refract)*(1.0-reflect));
        }	

        color += diff;
      }
    }

    ray.color = color;

    if (ray.bounce < scene->maxBounce)
    {
      if (reflect > 0.0f)
      {
        Ray rayRefl = ray.reflect();
        color += color*(1.0f - reflect) + scene->traceRay(rayRefl,(SceneObject*)ray.obj) * reflect;
      }
      if (refract > 0.0f)
      {
        Ray rayRefr = ray.refract(IOR);
        color += color*(1.0f - refract) + scene->traceRay(rayRefr,(SceneObject*)ray.obj) * refract;
      }
    }

    return color;	
  }
}
