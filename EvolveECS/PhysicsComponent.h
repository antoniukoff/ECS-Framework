#pragma once
#include "Component.h"
#include <Quaternion.h>
struct PhysicsComponent : public Component {
	float mass = 1.0f;
	MATH::Matrix3 rotationalInertia;
	MATH::Vec3 pos;
	MATH::Vec3 vel;
	MATH::Vec3 accel;
	MATH::Quaternion orientation;
	MATH::Vec3 angularVel;
	MATH::Vec3 angularAcc;
	MATH::Matrix3 inverseRotationMatrix;

	PhysicsComponent(Component* parent_) : Component(parent_) {};
	~PhysicsComponent() {};
	bool OnCreate() { return true; };
	void OnDestroy() {};
	void Update(const float deltaTime_) {};
	void Render() const {};

};