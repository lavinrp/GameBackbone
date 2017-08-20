#pragma once

#include <Backbone\AnimatedSprite.h>
#include <Backbone\Updatable.h>
#include <Util\DllUtil.h>
#include <Util\Point.h>

#include <SFML\Graphics\Sprite.hpp>

#include <set>
#include <vector>

namespace GB {

	/// <summary> Controls several sprites and animated sprites as one logical unit. </summary>
	class libGameBackbone CompoundSprite : public virtual Updatable {
	public:

		//ctr / dtr
		//shallow copy and move are fine for this class
		CompoundSprite();
		explicit CompoundSprite(const std::vector<sf::Sprite*>& components);
		CompoundSprite(const std::vector<sf::Sprite*>& components, const sf::Vector2f& position);
		CompoundSprite(const std::vector<sf::Sprite*>& sprites, const std::vector<AnimatedSprite*>& animatedSprites);
		CompoundSprite(const std::vector<sf::Sprite*>& sprites, const std::vector<AnimatedSprite*>& animatedSprites, const sf::Vector2f& position);
		explicit CompoundSprite(const sf::Vector2f initialPosition);
		CompoundSprite(const CompoundSprite& other) = default;
		CompoundSprite(CompoundSprite&& other) = default;
		CompoundSprite& operator= (const CompoundSprite& other) = default;
		CompoundSprite& operator= (CompoundSprite&& other) = default;
		virtual ~CompoundSprite();

		//getters
		std::vector<sf::Sprite*>* getComponents();
		std::vector<AnimatedSprite*>* getAnimatedSprites();
		sf::Vector2f getPosition() const;
		
		//setters
		virtual void setPosition(sf::Vector2f val);
		virtual void setPosition(float x, float y);

		//add / remove
		virtual void addComponent(sf::Sprite* component);
		virtual void addComponent(AnimatedSprite* component);
		virtual void removeComponent(sf::Sprite* component);
		virtual void clearComponents();

		//operations
		virtual void scale(float factorX, float factorY);
		virtual void scale(sf::Vector2f newScale);
		virtual void setScale(float factorX, float factorY);
		virtual void setScale(sf::Vector2f newScale);

		virtual void rotate(float degreeOffset);
		virtual void setRotation(float newRotation);

		virtual void rotateComponents(std::set<size_t> indiciesToRotate, float degreeOffset);
		virtual void setRotationOfComponents(std::set<size_t> indicesToRotate, float newRotation);

		virtual void move(float offsetX, float offsetY);
		virtual void move(sf::Vector2f offset);

		virtual void update(sf::Time currentTime);

	protected:
		void removeAnimatedSprite(AnimatedSprite* component);
		std::vector<sf::Sprite*> components;
		std::vector<AnimatedSprite*> animatedSprites;
		sf::Vector2f position;

	};

}
