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
	void drawSprite(const class Actor& actor, const class Texture& tex, struct Rectangle srcRect, Vector2 origin, Flip flip) const;

	void addMesh(class MeshComponent* mesh);
	void removeMesh(class MeshComponent* mesh);

	void setViewMatrix(const Matrix4& viewP);

private:
	
	void drawSprites();

	Window* window;
	SDL_GLContext context;
	VertexArray* spriteVertexArray;
	Matrix4 spriteViewProj;
	Matrix4 view;
	Matrix4 projection;

	std::vector<class SpriteComponent*> sprites;

};
