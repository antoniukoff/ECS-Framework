// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder. Lucky for me I was in that class (Game Engine 4)

#pragma once
#include "Component.h"
#include <glew.h>
#include <vector>
#include <Vector.h>
using namespace MATH;

class MeshComponent : public Component {
	// delete all the implicit stuff. Scott doesn't trust them
	MeshComponent(const MeshComponent&) = delete;
	MeshComponent(MeshComponent&&) = delete;
	MeshComponent& operator = (const MeshComponent&) = delete;
	MeshComponent& operator = (MeshComponent&&) = delete;

private:
	const char* filename;
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvCoords;
	size_t dataLength;
	GLenum drawmode;

	/// Private helper methods
	void LoadModel(const char* filename);
	void StoreMeshData(GLenum drawmode_);
	GLuint vao, vbo;
public:

	MeshComponent(Component* parent_, const char* filename_);
	~MeshComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	void Render(GLenum drawmode) const;
};
