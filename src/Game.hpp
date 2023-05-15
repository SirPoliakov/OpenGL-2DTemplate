#pragma once
#include <vector>
#include "Actor.hpp"
#include "SpriteComponent.hpp"
#include "Window.hpp"
#include "Vector2.hpp"
#include "RendererOGL.hpp"
#include "InputSystem.hpp"



using std::vector;

class Game
{
public:
	static Game& instance()
	{
		static Game inst;
		return inst;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

private:
	Game() : isRunning(true), isUpdatingActors(false) {}

public:
	bool initialize();
	void load();
	void loop();
	void unload();
	void close();

	void addActor(Actor* actor);
	void removeActor(Actor* actor);
	RendererOGL& getRenderer() { return renderer; }


	//v Game specifics ===============================================

	//^ Game specifics ===============================================

private:
	void processInput();
	void update(float dt);
	void render();

	bool isRunning;
	
	Window window;
	RendererOGL renderer;
	InputSystem inputSystem;

	bool isUpdatingActors;
	vector<Actor*> actors;
	vector<Actor*> pendingActors;



};
