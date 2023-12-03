#pragma once
#include <Vector.h>
#include "PhysicsComponent.h"
#include "Actor.h"

namespace Physics {
	// All the physics equations you need under one namespace
// I got this idea from Beautiful C++: 30 Core Guidelines for Writing Clean, 
// Safe, and Fast Code by Davidson & Gregory
// Updates acceleration using a = force/mass
	void ApplyForce(Ref<PhysicsComponent> body, const MATH::Vec3& force);
	// Update position using pos += v * deltaTime. 
	// We don’t need the 0.5at^2 term for the constraint physics
	void UpdatePos(Ref<PhysicsComponent> body, float deltaTime);
	// Updates velocity using v += a * deltaTime
	void UpdateVel(Ref<PhysicsComponent> body, float deltaTime);
	// Updates the orientation quaternion using q = rotation * q
	// Find the rotation quaternion using the axis of the angular velocity
	// and the angle based on the magnitude of the angular velocity * deltaTime
	// Then use:
	// Quaternion rotation = QMath::angleAxisRotation(angleDegrees, axis);
	// Finally, multiply by the original orientation to find the new one
	// body->orientation = rotation * body->orientation;
	void UpdateOrientation(Ref<PhysicsComponent> body, float deltaTime);
	void StraightLineConstraint(Ref<PhysicsComponent> body, float deltaTime, float slope, float yIntercept);
	void PlaneConstraint(Ref<PhysicsComponent> body, float deltaTime, const Vec3& normal, float distance);
	// Ensure the actor’s transform component has the same position and orientation 
	// as the physics component. Otherwise graphics won’t match physics
	void UpdateTransform(Ref<Actor> actor);

}