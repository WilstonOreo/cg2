#pragma once

#include "cg2/Ray.hpp"
#include "cg2/Image.hpp"

typedef enum { SM_STANDARD, SM_PHONG_VERTEX, SM_PHONG_FRAGMENT, SM_ } ShadeMode;
typedef enum { TF_NONE, TF_BILINEAR, TF_TRILINEAR, TF_ } TextureFilter;

namespace cg2 {
	class Texture {
		public:
			Texture() {};
			Texture(string filename) {
				init(filename);
			}

			void init(string filename);

			Vec3f operator()(float x, float y, float mipMapLevel);

			TextureFilter texFilter;
			bool mipMapping;

		private:
			vector<Image> mipMaps;
	};

	class Scene;

	struct Shader {
		Shader(Scene * _scene): scene(_scene) {}
		virtual Vec3f shade(Ray & ray) = 0;
		Scene * scene;
	};

	struct PhongShader : Shader {
		PhongShader(Scene * _scene,
					Vec3f _ambient = Vec3f(),
					Vec3f _diffuse = Vec3f(1.0,1.0,1.0),
					Vec3f _specular= Vec3f(0.2,0.2,0.2),
					float _shininess = 10.0,
					float _reflect   = 0.0,
					float _refract   = 0.0,
					float _IOR       = 1.5):

			Shader(_scene),
			ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess),
			reflect(_reflect), refract(_refract), IOR(_IOR) {}

		Vec3f shade(Ray & ray);

		Vec3f ambient, diffuse, specular;
		float shininess;
		float reflect, refract, IOR;
	};

	struct TextureShader: Shader {
		TextureShader(Scene * _scene): Shader(_scene) {
		}

		TextureShader(Scene * _scene, string filename): Shader(_scene) {
			init(filename,_scene);
		}

		void init(string filename, Scene * _scene) {
			tex.init(filename);
		}

		Vec3f shade(Ray & ray) {
			return tex(ray.u,ray.v,0.0);
		};
		Texture tex;
	};
}


