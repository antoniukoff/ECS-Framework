#include "glew.h"
#include "LightActor.h"
#include "UBO_Padding.h"

LightActor::LightActor(Component* parent_, LightStyle lightStyle_, Vec3 position_, Vec4 colour_, float intensity_, Vec3 fallOff_) 
	: Actor(parent_), lightStyle(lightStyle_), position(position_), colour(colour_), intensity(intensity_), fallOff(fallOff_)
{
}

LightActor::LightActor(Component* parent_):Actor(parent_)
{
	position.set(0.0f, 0.0f, 0.0f);
	colour.set(1.0f, 1.0f, 1.0f, 0.0f);
	intensity = 1.0f;
	fallOff.set(0.0f, 0.0f, 0.0f);
	lightStyle = LightStyle::DirectionLight;
}

LightActor::~LightActor()
{
	OnDestroy();
}

bool LightActor::OnCreate()
{
	if (isCreated) return isCreated;
	// lets do the buffer for the light. Just like how we did for the camera
	glGenBuffers(1, &uboLightDataID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightDataID);

	// Allocate the memory for the buffer (just the position and colour)
	// We'll use that UBO-padding header file to generate right size
	// Need a Vec3 for position and a Vec4 for colour
	size_t buffer_size = UBO_PADDING::VEC3 + UBO_PADDING::VEC4; // same as 2 Vec4's due to padding, but best to be explicit
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboLightDataID);
	// lets not unbind yet. Load data first. Its a global light so won't change
	size_t offset = 0;
	// just write into some of the buffer
	// just a global light, so just one static position
	// we'll have local lighting attached to mario's hand eventually
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec3), position);
	// Now we need to know where to load in the data keeping in mind the padding
	// the next opening cell in the VRAM
	offset = UBO_PADDING::VEC3; //  in the SubData calls we had to pass in literally the size of the position vector
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4), colour);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboLightDataID);
	
	isCreated = true;
	return isCreated;
}

void LightActor::OnDestroy(){
	glDeleteBuffers(1, &uboLightDataID);
	isCreated = false;
}
