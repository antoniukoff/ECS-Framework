#include "MaterialComponent.h"
#include <SDL_image.h>
MaterialComponent::MaterialComponent(Component* parent_, const char* filename_):Component(parent_) {
	textureID = 0;
	filename = filename_;
}

MaterialComponent::~MaterialComponent() {
	OnDestroy();
}

bool MaterialComponent::OnCreate() {
	if (isCreated) return true;
		// generate names
		// just an array of one element
		glGenTextures(1, &textureID);
		// Tells the GPU that the textureID is a GL_TEXTURE_2D
		glBindTexture(GL_TEXTURE_2D, textureID);
		// Scott likes SDL as it has an image loader (unlike GLFW)
		SDL_Surface* textureSurface = IMG_Load(filename);
		if (textureSurface == nullptr) {
			return false;
		}
		// If the BytesPerPixel is 4, use the red/green/blue/alpha mode
		// else just red/green/blue mode. Eg jpegs don't always support alpha
		int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		// grid of data, mode, width, height. Scott doesn't know wht you need to send in the mode twice
		// blessed be SDL, the array size will be w * h * pixels per byte
		glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	
		SDL_FreeSurface(textureSurface);
		/// Wrapping and filtering options
		// you can find them here: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
		// UV coordinates go from 0-1, so the GPU normalizes the width and height (
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// This means, I'm done talking to you
		// Make sure you unbind
		glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
		isCreated = true;
		return true;
}

void MaterialComponent::OnDestroy() {	
	// Free up the memory on the GPU
	glDeleteTextures(1, &textureID);
	isCreated = false;
}
void MaterialComponent::Update(const float deltaTime) {}
void MaterialComponent::Render()const {}