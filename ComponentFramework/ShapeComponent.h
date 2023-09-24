// Author: Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder in Game Engine 4 class. Lucky for me I was in that class
// Remember how every actor is built from components?

#ifndef SHAPECOMPONENT_H
#define SHAPECOMPONENT_H

#include "Component.h"
#include "Shape.h"
#include "Sphere.h"
// TODO for Assignment 1
#include "Cylinder.h"
#include "Capsule.h"
//#include "Box.h"

// It's handy to keep track of what type of shape we have in this component
enum class ShapeType {
	sphere,
	cylinder,
	capsule,
	box
};

class ShapeComponent: public Component{
	// Scott hates these implicit copy/moves. Best to delete them
	ShapeComponent(const ShapeComponent&) = delete;
	ShapeComponent(ShapeComponent&&) = delete;
	ShapeComponent& operator = (const ShapeComponent&) = delete;
	ShapeComponent& operator = (ShapeComponent&&) = delete;

public:
	ShapeComponent(Component*, GEOMETRY::Sphere);
	// TODO for Assignment 1:
	ShapeComponent(Component*, GEOMETRY::Cylinder); 
	ShapeComponent(Component*, GEOMETRY::Capsule);  
	//ShapeComponent(Component*, GEOMETRY::Box); 

	// What shape do we have? Well it will be either a sphere, cylinder, capsule, or box
	// Use the power of polymorphism to figure it out
	Ref<GEOMETRY::Shape> shape;
	ShapeType shapeType;
	virtual ~ShapeComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
};

#endif
