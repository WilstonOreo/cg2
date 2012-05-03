#ifndef _SHADER_HPP
#define _SHADER_HPP

#include "raycast.hpp"
#include "image.hpp"

typedef enum { SM_STANDARD, SM_PHONG_VERTEX, SM_PHONG_FRAGMENT, SM_ } ShadeMode;
typedef enum { TF_NONE, TF_BILINEAR, TF_TRILINEAR, TF_ } TextureFilter;


class Texture
{
public:
	Texture() {};
	Texture(string filename)
	{
		init(filename);
	}

	void init(string filename)
	{
		Image img(filename);
		mipMaps.clear();
		mipMaps.push_back(img);

		// Generate MipMaps
		int level = 0;
		
		while ((img._w > 2) && (img._h > 2))
		{
			cout << level << endl;
			Image dsImg(img._w/2,img._h/2); // Downsized image (by factor of 2)

			for (int y = 0; y < dsImg._h; y++)
				for (int x = 0; x < dsImg._w; x++)
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

	vec3f operator() (float x, float y, float mipMapLevel);

	TextureFilter texFilter;
	bool mipMapping;

private:
	vector<Image> mipMaps;
};

class Scene;

struct Shader
{
	Shader(Scene* _scene): scene(_scene) {}
	virtual vec3f shade(Ray& ray) = 0;
	Scene* scene;
};

struct PhongShader : Shader
{
	PhongShader(Scene* _scene, 
				vec3f _ambient = vec3f(), 
				vec3f _diffuse = vec3f(1.0,1.0,1.0), 
				vec3f _specular= vec3f(0.2,0.2,0.2),
				float _shininess = 10.0,
				float _reflect   = 0.0,
				float _refract   = 0.0,
				float _IOR 		 = 1.5):
		ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess),
		reflect(_reflect), refract(_refract), IOR(_IOR), Shader(_scene) {}

	vec3f shade(Ray& ray);

	vec3f ambient, diffuse, specular;
	float shininess;
	float reflect, refract, IOR;
};

struct TextureShader: Shader
{
	TextureShader(Scene* _scene): Shader(_scene)
	{
	}

	TextureShader(Scene* _scene, string filename): Shader(_scene)
	{
		init(filename,_scene);
	}

	void init(string filename, Scene* _scene)
	{
		tex.init(filename);
	}

	vec3f shade(Ray& ray) 
	{ 
		return tex(ray.u,ray.v,0.0); 
	}; 
	Texture tex;
};



#endif /* _SHADER_HPP */ 
