#include "Physics.h"
#include "TransformComponent.h"
void Physics::ApplyForce(Ref<PhysicsComponent> body, const MATH::Vec3& force)
{
	body->accel = force / body->mass;


}

void Physics::UpdatePos(Ref<PhysicsComponent> body, float deltaTime)
{
	body->pos += body->vel * deltaTime;
}

void Physics::UpdateVel(Ref<PhysicsComponent> body, float deltaTime)
{
	body->vel += body->accel * deltaTime;
}

void Physics::UpdateOrientation(Ref<PhysicsComponent> body, float deltaTime)
{
}

void Physics::StraightLineConstraint(Ref<PhysicsComponent> body, float deltaTime, float slope, float yIntercept)
{
	float positionConstraint = body->pos.y - slope * body->pos.x - yIntercept;
	float boungardStabilization = 0.15f;
	if (deltaTime < VERY_SMALL) {
		deltaTime = 1.0f / 60.0f;
	}
	// the sign on the bias can be negative or positive
	// depending on the direction of the constraint
	float bias = boungardStabilization * positionConstraint / deltaTime;
	// derived JV, which is the velocity constraint on thr bosrd
	float JV = body->vel.y - slope * body->vel.x;
	// calculated J * inverse M * transpose J on the board
	float inverseMeffective = (slope * slope + 1.0f) / body->mass;
	float langrangian = (-JV - bias) / inverseMeffective;
	// JT is a jacobian transpose vector which is a column vector
	Vec3 JT = { -slope, 1.0f, 0.0f };
	Vec3 changeInVelocity = (1.0f / body->mass) * JT * langrangian;
	body->vel += changeInVelocity;
}
void Physics::UpdateTransform(Ref<Actor> actor)
{
	Ref<TransformComponent> transform = actor->GetComponent<TransformComponent>();
	Ref<PhysicsComponent> body = actor->GetComponent<PhysicsComponent>();
	transform->pos = body->pos;
	transform->orientation = body->orientation;
}
	