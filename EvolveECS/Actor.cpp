#include "Actor.h"
#include "TransformComponent.h"
#include "Debug.h"
Actor::Actor(Component* parent_):Component(parent_) {}

// destructor usually deallocates memory
Actor::~Actor() {
	OnDestroy();
}

void Actor::OnDestroy() {
	RemoveAllComponents();
	Debug::Info("Deleting assets for Actor/Components: ", __FILE__, __LINE__);
	isCreated = false;
}

bool Actor::OnCreate() {
	if (isCreated) return isCreated;
	for (auto component : components) {
		if (component->OnCreate() == false) {
			Debug::Error("Loading assets for Actor/Components: ", __FILE__, __LINE__);
				isCreated = false;
				return isCreated;
		}
	}
	isCreated = true;
	return isCreated;
}

void Actor::Update(const float deltaTime) {
	std::cout << "Hello from Update\n";
}
void Actor::Render()const {}

void Actor::RemoveAllComponents() {
	// As we are using a vector list of shared ptrs, we won't have memory leaks!
	components.clear();
}

void Actor::ListComponents() const {
	// This is the RTTI stuff
	// will look up what these components point to. by dereferences we get the type of object
	// otherwise you get the base class. we want the polymorphic answer
	std::cout << typeid(*this).name() << " contains the following components:\n";
	for (auto component : components) {
		std::cout << typeid(*component.get()).name() << std::endl;
	}
	std::cout << '\n';
}

Matrix4 Actor::GetModelMatrix()
{
	Ref<TransformComponent> transform = GetComponent<TransformComponent>();
	if (transform) {
		modelMatrix = transform->GetTransformMatrix();// if it exists, lets make a model matrix
	}
	else {
		modelMatrix.loadIdentity();
	}
	
	if (parent) { // if there is a parent, we can dynamic cast the component to an actor
		modelMatrix = dynamic_cast<Actor*>(parent)->GetModelMatrix()* modelMatrix;
	}

	return modelMatrix;
}
