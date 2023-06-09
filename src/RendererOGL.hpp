#pragma once
#include "IRenderer.hpp"
#include "VertexArray.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"
#include "Shader.hpp"
#include <vector>


class RendererOGL : public IRenderer
{
public:
	RendererOGL();
	virtual ~RendererOGL();
	RendererOGL(const RendererOGL&) = delete;
	RendererOGL& operator=(const RendererOGL&) = delete;

	bool initialize(Window& window);
	void beginDraw();
	void draw();
	void endDraw();
	void close();
	IRenderer::Type type() { return Type::OGL; }

	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);
	void drawSprite(class Actor& actor, const class Texture& tex, Vector2 origin, Flip flip) const;

private:
	
	void drawSprites();

	Window* window;
	SDL_GLContext context;
	VertexArray* spriteVertexArray;
	Matrix4 viewProj;

	std::vector<class SpriteComponent*> sprites;

};
