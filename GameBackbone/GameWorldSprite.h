#pragma once
#include "DllUtil.h"
#include "GameWorldObject.h"
#include <SFML\Graphics\Sprite.hpp>

/// <summary> A sf::Sprite that exists withing a game world and tracks its position withing the game world. </summary>
libGameBackbone class GameWorldSprite :
	public GameWorldObject, public virtual sf::Sprite {
public:
	GameWorldSprite();
	explicit GameWorldSprite(const sf::Texture &texture);
	GameWorldSprite(const sf::Texture &texture, const sf::IntRect &rectangle);
	virtual ~GameWorldSprite();

	//getters / setters
	virtual void gMove(double x, double y);
	virtual void setActive(bool active);

	//operations
	virtual void update(sf::Time currentTime);
};

