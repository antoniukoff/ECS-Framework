// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder. Lucky for me I was in that class (Game Engine 4)

#pragma once
#include <vector> 
#include <iostream>
#include "Component.h"
#include <Matrix.h>

using namespace MATH;
class Actor: public Component {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	Actor(const Actor&) = delete;
	Actor(Actor&&) = delete;
	Actor& operator=(const Actor&) = delete;
	Actor& operator=(Actor&&) = delete;

protected:
	// Make a vector list of shared_ptrs
	std::vector<Ref<Component>> components;
	Matrix4 modelMatrix;
public:
	Actor(Component* parent_);
	~Actor();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;
	
	/// Footnote to those who think you can't write code in the header file - this is true
	/// with a few exceptions. (1) You can't inline code (implicitly or not) unless it is in 
	/// the header file and (2) templates must be in the header file

	// Copying in Scott's new AddComponents that use smart pointers
	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(Args&& ... args_) {
		/// before you add the component, ask if you have the component in the list already,
		/// if so - don't add a second one. 
		if (GetComponent<ComponentTemplate>().get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		/// Using std::make_shared to do the work. This is the new idea in class!
		components.push_back(std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...));
	}

	// To take advantage of an asset manager, make another AddComponent that just takes a component
	template<typename ComponentTemplate>
	void AddComponent(Ref<ComponentTemplate> component_) {
		if (GetComponent<ComponentTemplate>().get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		components.push_back(component_);
	}

	// Need this to work nicely with shared pointers
	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent() const{
		for (auto component : components) {
			if (dynamic_cast<ComponentTemplate*>(component.get())) {
				// This is dynamic cast designed for shared_ptr's
				// https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
				return std::dynamic_pointer_cast<ComponentTemplate>(component);
			}
		}
		return Ref<ComponentTemplate>(nullptr);
	}

	// code needs to be in the header if it's a templated function
	template<typename ComponentTemplate>
	void RemoveComponent() {
		for (unsigned int i = 0; i < components.size(); i++) {
			if (dynamic_cast<ComponentTemplate*>(components[i].get()) != nullptr) {
				components.erase(components.begin() + i);
				break;
			}
		}
	}

	// Note that these are not templated, so will write this in cpp
	void RemoveAllComponents();
	void ListComponents() const;

	// Getting the parenting thing working
	Matrix4 GetModelMatrix();
};

