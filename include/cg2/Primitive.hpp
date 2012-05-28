#pragma once

#include "cg2/Object.hpp"
#include "cg2/Shader.hpp"
#include "cg2/Ray.hpp"

namespace cg2 {
	struct Primitive: public Object {
		virtual Vec3f normal(const Ray & ray) const = 0;
		virtual TexCoords texCoords(const Ray & ray) const = 0;

		virtual bool intersect(Ray & ray) const = 0;

		TBD_DECLARE_PROPERTY(Shader *, shader);
	};

	DEFINE_CONTAINERS(Primitive)
}
