#include "Actor.hpp"
#include <algorithm>
#include "Game.hpp"
#include "Component.hpp"
#include "Maths.hpp"

Actor::Actor() :
	state(Actor::ActorState::Active),
	position(Vector3::zero),
	scale(1.0f),
	rotation(Vector3( 1.0f, 0.0f, 0.0f )),
	mustRecomputeWorldTransform(true),
	game(Game::instance())
{
	game.addActor(this);
}

Actor::~Actor()
{
	game.removeActor(this);
	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	while (!components.empty())
	{
		delete components.back();
	}
}

void Actor::setPosition(Vector3 positionP)
{
	position = positionP;
	mustRecomputeWorldTransform = true;
}

void Actor::setScale(float scaleP)
{
	scale = scaleP;
	mustRecomputeWorldTransform = true;
}

void Actor::setRotation(Vector3 rotationP)
{
	rotation += rotationP;
	mustRecomputeWorldTransform = true;
}

void Actor::rotate(const Vector3& axis, float angle)
{
	float _x = cos(angle) - axis.x;
	float _y = sin(angle) - axis.y;  //A voir s'il ne faut pas -sin(y)
	setRotation(Vector3( _x,_y, 0.0f));
}

void Actor::computeWorldTransform()
{
	if (mustRecomputeWorldTransform)
	{
		mustRecomputeWorldTransform = false;
		worldTransform = Matrix4::createScale(scale);
		float theta = Maths::acos(rotation.x);
		worldTransform *= Matrix4::createRotationX(theta);
		worldTransform *= Matrix4::createTranslation(position);

		for (auto component : components)
		{
			component->onUpdateWorldTransform();
		}
	}
}

void Actor::setState(ActorState stateP)
{
	state = stateP;
}



void Actor::processInput(const InputState& inputState)
{
	if (state == Actor::ActorState::Active)
	{
		for (auto component : components)
		{
			component->processInput(inputState);
		}
		actorInput(inputState);
	}
}

void Actor::actorInput(const InputState& inputState)
{
}

void Actor::update(float dt)
{
	if (state == Actor::ActorState::Active)
	{
		computeWorldTransform();
		updateComponents(dt);
	}
}

template<typename T>
T* Actor::getComponent()
{
	for (Component* component : components) {
		if (component->getComponentName() == T->getComponentName()) return component;
	}

	return nullptr;
}

void Actor::updateComponents(float dt)
{
	for (auto component : components)
	{
		component->update(dt);
	}
}


void Actor::addComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->getUpdateOrder();
	auto iter = begin(components);
	for (; iter != end(components); ++iter)
	{
		if (myOrder < (*iter)->getUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	components.insert(iter, component);
}

void Actor::removeComponent(Component* component)
{
	auto iter = std::find(begin(components), end(components), component);
	if (iter != end(components))
	{
		components.erase(iter);
	}
}