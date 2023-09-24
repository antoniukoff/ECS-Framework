/// Author: Umer Noor, 2022
/// Abstract class for geometric objects and bounding volume heirachies
/// Anything a ray can intersect with would be under this class
/// REFERENCE: Chapter 4 Ray Tracing, Fundementals of Computer Graphics, 4ed
/// They think Surface is a good name. I prefer Shape.
#ifndef SHAPE_H
#define SHAPE_H

#include <glew.h>
#include <vector>
#include <Vector.h>
namespace GEOMETRY {
	struct Shape {
		// TODO for Assignment 2:
 		// Anything that can be hit by a ray needs to be part of this class heirachy
		// virtual RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const = 0;

		// It's handy for any shape to draw itself for debugging purposes
		virtual void generateVerticesAndNormals() = 0;
		// Some concrete data and functions that every child will need
		void debugDraw();
		void StoreMeshData(GLenum drawmode_);
		std::vector<MATH::Vec3> vertices;
		std::vector<MATH::Vec3> normals;
		GLuint vao = 0;
		GLuint vbo = 0;
		size_t dataLength = 0;
		GLenum drawmode = GL_POINTS;
	};
}
#endif
