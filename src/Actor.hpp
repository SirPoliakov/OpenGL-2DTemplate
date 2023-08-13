#pragma once
#include <vector>
#include "Vector2.hpp"
#include "Vector3.hpp"
#include <SDL_stdinc.h>
#include "Matrix4.hpp"
using std::vector;

class Game;
class Component;

class Actor
{
public:

	enum class ActorState
	{
		Active,
		Paused,
		Accomplished,
		Out,
		Dead
	};

	Actor();
	Actor(Vector2 _Scale, Vector3 _Pos = Vector3::zero);
	virtual ~Actor();
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;

	Game& getGame() const { return game; }
	const ActorState getState() const { return state; }
	const Vector3 getPosition() const { return position; }
	const Vector2 getScale() const { return scale; }
	const Matrix4& getWorldTransform() const { return worldTransform; }

	void setPosition(Vector3 positionP);
	void setScale(Vector2 scaleP);
	void setRotation(float rotationP);
	void setState(ActorState stateP);

	//void setAngle(const Vector3& axis, float angle);
	void rotate(const Vector3& axis, float angle);
	void computeWorldTransform();

	void processInput(const struct InputState& inputState);
	virtual void actorInput(const struct InputState& inputState);
	void update(float dt);

	inline vector<Component*> getComponents() { return components; }

	template <typename T>
	T* getComponent();

	void updateComponents(float dt);
	void addComponent(Component* component);
	void removeComponent(Component* component);

protected:
	Game& game;
	ActorState state;
	Vector3 position;
	float rotation;
	Vector2 scale;
	Matrix4 worldTransform;
	bool mustRecomputeWorldTransform;

	vector<Component*> components;
};