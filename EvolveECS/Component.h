// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder. Lucky for me I was in that class (Game Engine 4)

#pragma once
#include <memory>

// Whenever we say reference, we mean a shared pointer
template<typename T>
using Ref = std::shared_ptr<T>;

class Component {
	public:
		Component(Component* parent_):parent(parent_), isCreated(false) {}
		// using = default; is the same as {};
		virtual ~Component() = default;
		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void Update(const float deltaTime_) = 0;
		virtual void Render()const = 0;
	protected:
		Component* parent;
		// the isCreated variable will be brought into anything inherited from Component
		// will be handy to stop us doing OnCreate twice!
		bool isCreated;
};

