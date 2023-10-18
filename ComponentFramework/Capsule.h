#pragma once
#include "Shape.h"

namespace GEOMETRY {

	struct Capsule : public Shape {
		float r;
		MATH::Vec3 topPoint;
		MATH::Vec3 bottomPoint;

		// The constructors sets the radius and centre positions of the caps, generates 
		// the vertices and normal for the shape, and stores the mesh data in order to
		// render
		Capsule();
		Capsule(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_);
		void set(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_);
		// Fill the vertices and normals list with Vec3's to represent a capsule
		// Try spheres for the top and bottom, with a cylinder for the main body
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;
	};
}