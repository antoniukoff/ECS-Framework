#pragma once
#include"Shape.h"


namespace GEOMETRY{
	struct Cylinder :public Shape
	{
		float radius;
		MATH::Vec3 capCentrePosA;
		MATH::Vec3 capCentrePosB;

		// Default constructor sets the radius and cap centre positions, and fires off 
		Cylinder() {
			set(1.0f, MATH::Vec3(0.0f, 2.0f, 0.0f), MATH::Vec3(0.0f, 0.0f, 0.0f));
		}
		// Don’t forget to execute generateVerticesAndNormals at the end of this three 
		Cylinder(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_) {
			set(r_, capCentrePosA_, capCentrePosB_);
		}
		// A set method is handy, just remember to fire off generateVerticesAndNormals
		
		void set(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_) {
			radius = r_;
			capCentrePosA = capCentrePosA_;
			capCentrePosB = capCentrePosB_;
			generateVerticesAndNormals();
		}
		// Fill the vertices and normals list with Vec3's to represent a cylinder
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;
	};
}