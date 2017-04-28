#pragma once
#include "stdafx.h"
#include "DllUtil.h"
#include "AnimatedSprite.h"
#include "Updatable.h"

#include<SFML/Graphics/Sprite.hpp>

#include<vector>

/// <summary> Controls several sprites and animated sprites as one logical unit. </summary>
class libGameBackbone CompoundSprite : public virtual Updatable {
public:

	//ctr / dtr
	//shallow copy and move are fine for this class
	CompoundSprite();
	CompoundSprite(const std::vector<sf::Sprite*>& sprites, const std::vector<AnimatedSprite*>& animatedSprites);
	virtual ~CompoundSprite();

	//getters
	std::vector<sf::Sprite*>* getSfSprites();
	std::vector<AnimatedSprite*>* getAnimatedSprites();

	//add / remove
	void addSprite(sf::Sprite* component);
	void addAnimatedSprite(AnimatedSprite* component);
	void removeSprite(sf::Sprite* component);
	void removeAnimatedSprite(AnimatedSprite* component);
	void clearComponents();

	//operations
	void scale(float factorX, float factorY);
	void move(float offsetX, float offsetY);
	virtual void update(sf::Time currentTime);

protected:
	std::vector<sf::Sprite*> sprites;
	std::vector<AnimatedSprite*> animatedSprites;
};

