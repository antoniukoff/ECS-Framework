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

void Physics::UpdateTransform(Ref<Actor> actor)
{
	Ref<TransformComponent> transform = actor->GetComponent<TransformComponent>();
	Ref<PhysicsComponent> body = actor->GetComponent<PhysicsComponent>();
	transform->pos = body->pos;
	transform->orientation = body->orientation;
}
	