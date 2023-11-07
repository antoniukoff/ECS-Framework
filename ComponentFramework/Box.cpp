#include "Box.h"
#include <QMath.h>
#include <array>
using namespace MATH;

void GEOMETRY::Box::generateVerticesAndNormals()
{
	const Vec3 topLeftFront = QMath::rotate((centre + Vec3(-halfExtents.x, +halfExtents.y, +halfExtents.z)), orientation);
	const Vec3 bottomLeftFront = QMath::rotate((centre + Vec3(-halfExtents.x, -halfExtents.y, +halfExtents.z)), orientation);
	const Vec3 topRightFront = QMath::rotate((centre + Vec3(+halfExtents.x, +halfExtents.y, +halfExtents.z)), orientation);
	const Vec3 bottomRightFront = QMath::rotate((centre + Vec3(+halfExtents.x, -halfExtents.y, +halfExtents.z)), orientation);
	const Vec3 topLeftBack = QMath::rotate((centre + Vec3(-halfExtents.x, +halfExtents.y, -halfExtents.z)), orientation);
	const Vec3 bottomLeftBack = QMath::rotate((centre + Vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z)), orientation);
	const Vec3 topRightBack = QMath::rotate((centre + Vec3(+halfExtents.x, +halfExtents.y, -halfExtents.z)), orientation);
	const Vec3 bottomRightBack = QMath::rotate((centre + Vec3(+halfExtents.x, -halfExtents.y, -halfExtents.z)), orientation);
	
	
	// Back Face TopLeftBack triangle
	vertices.push_back(topLeftBack);
	vertices.push_back(topRightBack);
	vertices.push_back(bottomLeftBack);
	normals.push_back(QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), orientation));

	// Back Face BottomRightBack triangle
	vertices.push_back(bottomRightBack);
	vertices.push_back(bottomLeftBack);
	vertices.push_back(topRightBack);
	normals.push_back(QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, 0.0f, -1.0f), orientation));

	// Top Face Top Face TopLeftFront triangle
	vertices.push_back(topLeftFront);
	vertices.push_back(topRightBack);
	vertices.push_back(topLeftBack);
	normals.push_back(QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), orientation));

	// Top Face Top Face TopRightFront triangle
	vertices.push_back(topLeftFront);
	vertices.push_back(topRightFront);
	vertices.push_back(topRightBack);
	normals.push_back(QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), orientation));

	// Bottom Face BottomLeftFront triangle
	vertices.push_back(bottomLeftFront);
	vertices.push_back(bottomLeftBack);
	vertices.push_back(bottomRightBack);
	normals.push_back(QMath::rotate(Vec3(0.0f, -1.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, -1.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, -1.0f, 0.0f), orientation));

	// Bottom  Face BottomRightFront triangle
	vertices.push_back(bottomLeftFront);
	vertices.push_back(bottomRightBack);
	vertices.push_back(bottomRightFront);
	normals.push_back(QMath::rotate(Vec3(0.0f, -1.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, -1.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(0.0f, -1.0f, 0.0f), orientation));

	// Left TopLeftFront triangle
	vertices.push_back(topLeftFront);
	vertices.push_back(bottomLeftBack);
	vertices.push_back(bottomLeftFront);
	normals.push_back(QMath::rotate(Vec3(-1.0f, 0.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(-1.0f, 0.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(-1.0f, 0.0f, 0.0f), orientation));

	// Left TopLeftBack triangle
	vertices.push_back(topLeftFront);
	vertices.push_back(topLeftBack);
	vertices.push_back(bottomLeftBack);
	normals.push_back(QMath::rotate(Vec3(-1.0f, 0.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(-1.0f, 0.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(-1.0f, 0.0f, 0.0f), orientation));


	// Right TopRightFront triangle
	vertices.push_back(topRightFront);
	vertices.push_back(bottomRightFront);
	vertices.push_back(bottomRightBack);
	normals.push_back(QMath::rotate(Vec3(1.0f, 0.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(1.0f, 0.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(1.0f, 0.0f, 0.0f), orientation));

	// Right TopRightBack triangle
	vertices.push_back(topRightFront);
	vertices.push_back(bottomRightBack);
	vertices.push_back(topRightBack);
	normals.push_back(QMath::rotate(Vec3(1.0f, 0.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(1.0f, 0.0f, 0.0f), orientation));
	normals.push_back(QMath::rotate(Vec3(1.0f, 0.0f, 0.0f), orientation));

	StoreMeshData(GL_TRIANGLES);
}

GEOMETRY::RayIntersectionInfo GEOMETRY::Box::rayIntersectionInfo(const Ray& ray) const
{

	std::array<Slab, 3> slabs = {
	Slab{Vec3(1.0f, 0.0f, 0.0f), -halfExtents.x, halfExtents.x},
	Slab{Vec3(0.0f, 1.0f, 0.0f), -halfExtents.y, halfExtents.y},
	Slab{Vec3(0.0f, 0.0f, 1.0f), -halfExtents.z, halfExtents.z} };
	// Start of with the smallest and biggest possible values for the ray’s t value
	float tmin = 0.0f;
	float tmax = FLT_MAX;
	// Loop over the 3 slabs of the box along the x, y, and z axes
	for (int i = 0; i < 3; i++) {
		// Check intersection with the near plane. Do a ray-plane check for this 
		//particular axis
			float t1 = (slabs[i].distNear - VMath::dot(slabs[i].normal, ray.start)) / VMath::dot(slabs[i].normal, ray.dir);
			// Check intersection with the far plane. Do another ray-plane check
			float t2 = (slabs[i].distFar - VMath::dot(slabs[i].normal, ray.start)) / VMath::dot(slabs[i].normal, ray.dir);
			// Set tNear to be the smaller of the two t values
			float tNear = std::min(t1, t2);
			
			// Set tFar to be the larger of the two t values
			float tFar = std::max(t1, t2);
			
			// Update tmin so that it is the largest tNear value found so far
			if(tmin < tNear)
				tmin = tNear;

			// Update tmax so that it is the smallest tFar value found so far
			if(tmax > tFar)
				tmax = tFar;

			if (tmin > tmax) {
				// If tmin is greater than tmax, then we didn't intersect the box, get outta 
				// here
					return RayIntersectionInfo();
			}
	}
	// If we made it this far, we have a valid intersection
	RayIntersectionInfo rayInfo;
	rayInfo.isIntersected = true;
	rayInfo.t = tmin;
	rayInfo.intersectoinPoint = ray.currentPosition(rayInfo.t);
	return rayInfo;

}
