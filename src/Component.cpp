#include "Component.hpp"
#include "Actor.hpp"
#include "InputSystem.hpp"

Component::Component(Actor* ownerP, int updateOrderP) :
	owner(*ownerP),
	updateOrder(updateOrderP)
{
	owner.addComponent(this);
}

Component::~Component()
{
	owner.removeComponent(this);
}

void Component::processInput(const InputState& inputState)
{
}

void Component::update(float dt)
{
}

std::string Component::getComponentName()
{
	return "Default";
}