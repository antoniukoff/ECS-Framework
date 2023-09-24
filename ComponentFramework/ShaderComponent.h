// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder. Lucky for me I was in that class (Game Engine 4)

#pragma once
#include "Component.h"
#include "glew.h"
#include <unordered_map>
#include <string>

class ShaderComponent: public Component {
	ShaderComponent(const ShaderComponent&) = delete;
	ShaderComponent(ShaderComponent&&) = delete;
	ShaderComponent& operator = (const ShaderComponent&) = delete;
	ShaderComponent& operator = (ShaderComponent&&) = delete;

private:
	const char* vsFilename;
	const char* fsFilename;
	unsigned int shaderID;
	unsigned int vertShaderID;
	unsigned int fragShaderID;
	// Make a map of strings (all the Uniforms like model matrix, view matrix) and get ID number of those things
	// For example the projectionMatrix has the ID 2
	// The unordered map will be address of projection matrix and get ID for example
	std::unordered_map<std::string, GLuint > uniformMap;

	/// Private helper methods
	char* ReadTextFile(const char *fileName);	
	bool CompileAttach();
	bool Link();
	void SetUniformLocations();
	

public:
	ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_);
	~ShaderComponent();
	inline GLuint GetProgram() const { return shaderID;}
	GLuint GetUniformID(std::string name);
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime ) override;
	void Render() const override;
};


