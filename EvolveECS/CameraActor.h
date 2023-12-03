// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder in Game Engine 4 class. Lucky for me I was in that class.

#pragma once
#include "Actor.h"
#include "TransformComponent.h"

class CameraActor:public Actor {
private:
	// The camera is nothing but a projection and a view
	// These are not pointers, so never delete them! They are created on the stack
	// The automatic constructor will be the identity
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	GLuint uboMatricesID; // can be an unsigned int instead of a GLuint
	const GLuint bindingPoint = 0; // This must be unique to the camera = 0. And never change it! This will be where the VRAM needs to know where this is (and the shader too). btw lights will be = 1
public:
	CameraActor(Component* parent_);
	virtual ~CameraActor();

	// we will override the base class one (new in C++ 11)
	// put override so that if you spell the method wrong it give a error
	bool OnCreate() override;
	void OnDestroy() override;

	Matrix4 GetProjectionMatrix() { return projectionMatrix; }
	Matrix4 GetViewMatrix() { return viewMatrix; }

	// needs to happen anytime we change 
	// field of view along y is roughly 40 degrees
	// need aspectRatio of window, near and far clipping planes
	// these are the standard arguments for generating the perspective matrix in OpenGL
	void UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far);
	void UpdateViewMatrix();
	GLuint GetMatricesID() const { return uboMatricesID; }
};

