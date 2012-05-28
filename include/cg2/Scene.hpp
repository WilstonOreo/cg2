#pragma once

#include "cg2/SceneObject.hpp"
#include "cg2/Light.hpp"
#include "cg2/Camera.hpp"
#include "cg2/FrameBuffer.hpp"

#include <vector>

namespace cg2 {
	using std::vector;

	struct Scene {
		Scene() {
			maxBounce = 15;
		}

		void addObject(SceneObject * obj) {
			objects.push_back(obj);
		}

		void castRays(Rays & rays, Image * outImage = NULL);

		Vec3f traceRay(Ray & ray, SceneObject * curObj = NULL);
		bool traceShadowRay(Ray & shadowRay, SceneObject * curObj = NULL);

		unsigned maxBounce;

		SceneObjectList objects;
		LightList  lights;
		Camera * camera;
		FrameBuffer frameBuffer;
	};
}

