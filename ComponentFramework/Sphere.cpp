#include "Sphere.h"
#include <MMath.h>

using namespace MATH;
using namespace GEOMETRY;

void Sphere::generateVerticesAndNormals()
{
	// We need to fill the vertices and normals arrays with the correct data for a sphere
	// deltaTheta governs how close each ring will be around our sphere. Try messing with it
	const float deltaTheta = 5.0f;
	// deltaPhi governs how close each point will be per ring. Try messing with it
	const float deltaPhi = 4.0f;
	for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
	{
		// Build a ring
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);
		for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
			// Rotate a point in the ring around the y-axis to build a sphere!
			Matrix3 rotationMatrix = MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			circle = rotationMatrix * circle;
			// Push the circle point to our vertices array
			vertices.push_back(circle);
			// The normal of a sphere points outwards from the center position Vec3(x, y, z)
			normals.push_back(circle - Vec3(x, y, z));
		}
	}
	// Once we are all done filling the vertices and normals, use the base class method to store the data in the GPU
	StoreMeshData(GL_POINTS);
}