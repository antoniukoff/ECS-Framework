#include <glew.h> // you might get burned if this isn't the first line
#include "CameraActor.h"
#include <MMath.h>
#include <QMath.h>
#include "Debug.h"
#include "UBO_Padding.h"
using namespace MATH;
CameraActor::CameraActor(Component* parent_):Actor(parent_) {
// lets set up the projection and view
	projectionMatrix = Matrix4();
	viewMatrix = Matrix4();

	// One of the hazards of putting the OnCreate here is that we want to catch any dangling pointers (return false)
}

CameraActor::~CameraActor() {
	// Let's start calling OnDestroy in the destructor
	// Handy to keep OnDestroy around as we could keep the memory around, not necessarily delete and re-allocate
	// Might be a purpose to release all assets in OnDestroy but not delete the memory. ie do OnDestroy out of this 
	// destructor and do OnCreate again
	OnDestroy();
}

bool CameraActor::OnCreate()
{
	// This'll stop us from creating something twice and generating memory leaks
	// This will save us sweating when the code gets even more complicated
	if (isCreated) return isCreated; // or return true

	// Let's layout the UBO
	// Generate a GlUint. Get the name of a brand new buffer
	glGenBuffers(1, &uboMatricesID);

	// Yell at the buffer using it's name. We're talking to you
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);

	// Allocate the memory for the buffer (projection and view matrices)
	// We'll use that UBO-padding header file to generate right size
	// Need two 4x4 matrices
	size_t buffer_size = 2* UBO_PADDING::MAT4;
	// static ram that won't fluctauate during the run. It optimizes the data transfer
	// allocating memory here, but not putting in the data yet. btw we'll do projection first then view
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	// done talking to the buffer
	glBindBuffer(GL_UNIFORM_BUFFER,0);
	// hook to it using the bindingPoint for the camera. So that the shaders can find it later on
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboMatricesID);

	// can't have near at zero, will crash
	UpdateProjectionMatrix(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	UpdateViewMatrix();

	isCreated = true;
	return isCreated;
}


void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far)
{
	projectionMatrix = MMath::perspective(fovy, aspectRatio, near, far);
	// if we need to update the projectionMatrix, lets bind to the buffer and use that
	// trying to make the camera live in the VRAM inside the UBO
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
	size_t offset = 0; // start with projection (then we'll do view)
	// Asking for size of data we are transferring (not padding)
	// Scott has this amazing piece of code in his Matrix class that defines the name of the matrix as a float *. OMG (line 152 in Matrix.h)
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Matrix4), projectionMatrix);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CameraActor::UpdateViewMatrix()
{
	// let's hope we have transform component!
	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();
	if (transformComponent == nullptr) {
		// we have no transform component! use default view. takes 3 vectors
		// this sets up where is the camera, what's it looking at, and what's up
		// we are moving the universe, not a camera
		// the eye moves the camera back towards us (positive z)
		// but what really happens is we push the whole universe negative 5 along z
		viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		
	}
	else {
		Quaternion orientation = transformComponent->GetOrientation();
		Vec3 position = transformComponent->pos;
		// This is viewpoint of the model (right to left)
		// but from the point of view of the camera read from left to right. It's weird
		// I'm used to rotating on right side for models. But from the camera's vantage point we read the other way round
		viewMatrix = MMath::toMatrix4(orientation) * MMath::translate(position);
		// forget using the scale matrix above. What does that even mean for a camera?
	}
	size_t offset = sizeof(Matrix4); // start where the projection matrix ended
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Matrix4), viewMatrix);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CameraActor::OnDestroy()
{
	// protect the memory
	glDeleteBuffers(1, &uboMatricesID);
	// We've released all the assets, so we could do OnCreate again
	isCreated = false;
}