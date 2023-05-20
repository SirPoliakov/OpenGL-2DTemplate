#include "SpriteComponent.hpp"
#include "Actor.hpp"
#include "Game.hpp"
#include "Rectangle.hpp"

SpriteComponent::SpriteComponent(Actor* ownerP, Texture& textureP, int drawOrderP) :
	Component(ownerP),
	texture(textureP),
	drawOrder(drawOrderP),
	texWidth(textureP.getWidth()),
	texHeight(textureP.getHeight()),
	isVisible(true)
{
	owner.getGame().getRenderer().addSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	owner.getGame().getRenderer().removeSprite(this);
}

void SpriteComponent::setTexture(const Texture& textureP)
{
	texture = textureP;
	texture.updateInfo(texWidth, texHeight);
}

void SpriteComponent::draw(IRenderer& renderer)
{
	Vector2 origin{ texWidth / 2.f, texHeight / 2.f };
	renderer.drawSprite(owner, texture, origin, IRenderer::Flip::None);
}

void SpriteComponent::setVisible(bool isVisibleP)
{
	isVisible = isVisibleP;
}