#pragma once
#include "Shape.h"
#include <Quaternion.h>


namespace GEOMETRY {
	struct Box : public Shape
	{
		// Represent an Orientated Bounding Box by centre position, half extents, and orientation.
		// REFERENCE: Real Time Collision Detection by Ericson
		// Ericson recommends keeping the orientation as a matrix for fast collision
		// detection, but I love quaternions too much 
		MATH::Vec3 centre;
		MATH::Vec3 halfExtents; 
		MATH::Quaternion orientation;

		// The constructors set the centre position, the half extents along x, y, and z,and the orientation quaterion.
		// Then it generates the vertices and normal for the shape and stores the mesh data in order to render
		Box() {

		}
		Box(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_) {
			set(centre_, halfExtents_, orientation_);
		}
		void set(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_) {
			centre = centre_; 
			halfExtents = halfExtents_;
			orientation = orientation_;
			generateVerticesAndNormals();
		}
		// Fill the vertices and normals list with Vec3's to represent a box
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;
	};
}
