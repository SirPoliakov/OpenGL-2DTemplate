#include "Actor.hpp"
#include <algorithm>
#include "Game.hpp"
#include "Component.hpp"
#include "Maths.hpp"

Actor::Actor() :
	state(Actor::ActorState::Active),
	position(Vector3::zero),
	scale(Vector2::zero),
	rotation(0.0f),
	mustRecomputeWorldTransform(true),
	game(Game::instance())
{
	game.addActor(this);
}
Actor::Actor(Vector2 _Scale, Vector3 _Pos ) :
	state(Actor::ActorState::Active),
	position(_Pos),
	scale(_Scale),
	rotation(0.0f),
	mustRecomputeWorldTransform(true),
	game(Game::instance())
{
	game.addActor(this);
}


Actor::~Actor()
{
	game.removeActor(this);
}

void Actor::setPosition(Vector3 positionP)
{
	position = positionP;
	mustRecomputeWorldTransform = true;
}

void Actor::setScale(Vector2 scaleP)
{
	scale = scaleP;
	mustRecomputeWorldTransform = true;
}

void Actor::setRotation(float rotationP)
{
	rotation += rotationP;
	mustRecomputeWorldTransform = true;
}

void Actor::rotate(const Vector3& axis, float angle)
{
	rotation += angle;
	mustRecomputeWorldTransform = true;
}

void Actor::computeWorldTransform()
{
	if (mustRecomputeWorldTransform)
	{
		mustRecomputeWorldTransform = false;

		Vector3 vec3 = Vector3::zero; vec3.x = scale.x; vec3.y = scale.y;

		worldTransform = Matrix4::createScale(vec3);
		worldTransform *= Matrix4::createRotationZ(rotation);
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