#include "Box.h"
#include <QMath.h>
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
