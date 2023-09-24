// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder. Lucky for me I was in that class (Game Engine 4)

#pragma once
#include "Component.h"
#include <Matrix.h>
#include <Quaternion.h>
#include <Euler.h>
#include "Actor.h"
using namespace MATH;
struct TransformComponent : public Component {
	Vec3 pos;
	Quaternion orientation;
	Vec3 scale;
	TransformComponent(Component* parent_);
	TransformComponent(Component* parent_, Vec3 pos_, Quaternion orientation_, Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f)); // that last argument (has to be the last set) allows to initialize without a 4th argument
	~TransformComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;
	Matrix4 GetTransformMatrix() const;
	Quaternion GetOrientation() { return orientation; }
	void SetTransform(Vec3 pos_, Quaternion orientation_, Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f)) {
		pos = pos_;
		orientation = orientation_;
		scale = scale_;
	}
};