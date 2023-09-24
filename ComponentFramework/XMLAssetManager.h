// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder in Game Engine 4 class. Lucky for me I was in that class
// Remember Scott's final assignment was reading in assets & Actors from XML file? Here was my attempt
#ifndef XMLASSETMANAGER_H
#define XMLASSETMANAGER_H

#include <unordered_map>
#include "Component.h"
#include <iostream>
#include "tinyxml2.h"
#include "Actor.h"
struct XMLAssetManager {
	// assets can contain meshes, materials, shaders, or actors
	std::unordered_map< std::string, Ref<Component> > xmlAssets;

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name, Args&& ... args_) {
		// Make the component shared pointer based on the arguments
		Ref<ComponentTemplate> component = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		// Add to the unordered map
		xmlAssets[name] = component;
	}

	// Have an overloaded AddComponent that takes in a fully built actor
	template<typename ComponentTemplate>
	void AddComponent(const char* name, Ref<ComponentTemplate> component) {
		xmlAssets[name] = component;
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent(const char* name) const {
		Ref<ComponentTemplate> assetWeNeed;
		for (auto const& pair : xmlAssets) {
			if (std::string(pair.first) == name) {
				assetWeNeed = std::dynamic_pointer_cast<ComponentTemplate>(pair.second);
			}
		}
		return assetWeNeed;
	}

	void AddSphereShape(const tinyxml2::XMLElement* child);
	// TODO Assignment 1: Code up these methods for Cylinder, Capsule and Box in XMLAssetManager.cpp
	void AddCylinderShape(const tinyxml2::XMLElement* child); 
	void AddCapsuleShape(const tinyxml2::XMLElement* child); 
	void AddBoxShape(const tinyxml2::XMLElement* child); 
	
	void AddMaterial(const tinyxml2::XMLElement* child);
	void AddShader(const tinyxml2::XMLElement* child);
	void AddCamera(const tinyxml2::XMLElement* child);
	void AddLight(const tinyxml2::XMLElement* child);
	void AddMeshToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor);
	void AddShaderToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor);
	void AddMaterialToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor);
	void AddShapeToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor);
	void AddTransformToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor, Component* parent);
	void AddPhysicsToActor  (const tinyxml2::XMLElement* child, Ref<Actor> actor, Component* parent);

	XMLAssetManager();
	~XMLAssetManager();
};
#endif