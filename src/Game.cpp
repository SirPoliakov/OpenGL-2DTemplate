#include "Game.hpp"
#include "Actor.hpp"
#include "Assets.hpp"
#include "Timer.hpp"




bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = myRenderer.initialize(window);
	bool isInputInit = inputSystem.initialize();

	return isWindowInit && isRendererInit && isInputInit; // Return bool && bool && bool ...to detect error
}

void Game::load()
{
	inputSystem.setMouseRelativeMode(true);

	// SHADERS ==============================
	Assets::loadShader("C:\\Users\\Ben\\Documents\\Prog\\GIT\\C++\\OpenGL-2DTemplate\\src\\Rsc\\Shaders\\Sprite.vert", "C:\\Users\\Ben\\Documents\\Prog\\GIT\\C++\\OpenGL-2DTemplate\\src\\Rsc\\Shaders\\Sprite.frag", "", "", "", "Sprite");

	// TEXTURES =============================
	Assets::loadTexture(myRenderer, "C:\\Users\\Ben\\Documents\\Prog\\GIT\\C++\\OpenGL-2DTemplate\\src\\Rsc\\Textures\\Tree.bmp", "Tree");

	myActor = new Actor(Vector2(200.0f, 200.0f), Vector3::zero);
	tree = new SpriteComponent(myActor, Assets::getTexture("Tree"), 1);
	myActor->addComponent(tree);

}

void Game::processInput()
{
	inputSystem.preUpdate();

	// SDL Event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		isRunning = inputSystem.processEvent(event);
	}

	inputSystem.update();
	const InputState& input = inputSystem.getInputState();

	// Escape: quit game
	if (input.keyboard.getKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Released)
	{
		isRunning = false;
	}

	// Actor input
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->processInput(input);
	}
	isUpdatingActors = false;
}

void Game::update(float dt)
{
	//v =============================================================╗
	//v Common actor management                                      ║

	// Update actors 
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->update(dt);
	}
	isUpdatingActors = false;

	// Move pending actors to actors
	for (auto pendingActor : pendingActors)
	{
		pendingActor->computeWorldTransform();
		actors.emplace_back(pendingActor);
	}
	pendingActors.clear();

	// Delete dead actors
	std::vector<Actor*> deadActors;
	for (auto actor : actors)
	{
		if (actor->getState() == Actor::ActorState::Dead)
		{
			deadActors.emplace_back(actor);
		}
	}
	for (auto deadActor : deadActors)
	{
		delete deadActor;
	}

	//^ Common actor management                                      ║
	//^ =============================================================╝

}

void Game::render()
{
	myRenderer.beginDraw();
	myRenderer.draw();
	myRenderer.endDraw();
}


void Game::loop()
{
	Timer timer;
	float dt = 0;
	while (isRunning)
	{
		float dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		timer.delayTime();
	}
}

void Game::unload()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!actors.empty())
	{
		delete actors.back();
	}

	// Resources
	Assets::clear();
}

void Game::close()
{
	inputSystem.close();
	myRenderer.close();
	window.close();
	SDL_Quit();
}

void Game::addActor(Actor* actor)
{
	if (isUpdatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::removeActor(Actor* actor)
{
	// Erase actor from the two vectors
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors))
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}
	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors))
	{
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}