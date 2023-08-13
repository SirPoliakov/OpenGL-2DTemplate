#include <GL/glew.h>
#include <SDL_image.h>

#include "RendererOGL.hpp"
#include "Rectangle.hpp"
#include "Assets.hpp"
#include "Actor.hpp"
#include "SpriteComponent.hpp"
#include "Log.hpp"


RendererOGL::RendererOGL() :
	window{ nullptr },
	context{ nullptr },
	spriteVertexArray{ nullptr },
	viewProj{ Matrix4::createSimpleViewProj(WINDOW_WIDTH, WINDOW_HEIGHT) }
{
}

RendererOGL::~RendererOGL()
{
}

bool RendererOGL::initialize(Window& windowP)
{
	window = &windowP;

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Depth buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// OpenGL Context
	context = SDL_GL_CreateContext(windowP.getSDLWindow());

	// GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Log::error(LogCategory::Video, "Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code, so clear it
	glGetError();

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		Log::error(LogCategory::Video, "Unable to initialize SDL_image");
		return false;
	}

	spriteVertexArray = new VertexArray(spriteVertices, 4, indices, 6);
	return true;
}

void RendererOGL::addSprite(SpriteComponent* sprite)
{
	// Insert the sprite at the right place in function of drawOrder
	int spriteDrawOrder = sprite->getDrawOrder();
	auto iter = begin(sprites);
	for (; iter != end(sprites); ++iter)
	{
		if (spriteDrawOrder < (*iter)->getDrawOrder()) break;
	}
	sprites.insert(iter, sprite);
}

void RendererOGL::removeSprite(SpriteComponent* sprite)
{
	auto iter = std::find(begin(sprites), end(sprites), sprite);
	sprites.erase(iter);
}

void RendererOGL::beginDraw()
{
	glClearColor(0.45f, 0.45f, 1.0f, 1.0f);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererOGL::draw()
{
	//Draw() into drawSprites() into drawSprites(): Pourra ulterieurement appeler d'autres assets a draw comme des meshes ici en plus des sprites
	drawSprites();
}

void RendererOGL::drawSprites()
{
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Active shader and vertex array
	Shader& spriteShader = Assets::getShader("Sprite");
	spriteShader.use();
	spriteShader.setMatrix4("uViewProj", viewProj);
	spriteVertexArray->setActive();

	for (auto sprite : sprites)
	{
		if (sprite->getVisible())
		{
			sprite->draw(*this);
		}
	}
}

void RendererOGL::drawSprite(const Actor& actor, const Texture& tex, Vector2 origin, Flip flip) const
{	
	tex.setActive();
	Assets::getShader("Sprite").setInteger("uTexture", 0);
	Matrix4 world = actor.getWorldTransform();
	Assets::getShader("Sprite").setMatrix4("uWorldTransform", world);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RendererOGL::endDraw()
{
	SDL_GL_SwapWindow(window->getSDLWindow());
}

void RendererOGL::close()
{
	delete spriteVertexArray;
	SDL_GL_DeleteContext(context);
}