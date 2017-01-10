#include "AnimatedSprite.h"


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>

//ctr and dtr

///<summary>Default Constructor. Values initialized to 0</summary>
AnimatedSprite::AnimatedSprite() {
	animations = nullptr;
	init();
}

///<summary>Constructor. Texture set to passed value. Position set to 0.</summary>
AnimatedSprite::AnimatedSprite(const sf::Texture & texture) : sf::Sprite(texture) {
	animations = nullptr;
	init();
}

/**
<summary>Constructor. Initializes texture to first frame of first animation.</summary>
<param name = "texture"> texture representing the animation sheet </param>
<param name = "rows"> the number of rows in the animated sprite animation sheet</param>
<param name = "cols"> the number of columns in the animated sprite animation sheet</param>
<param name = "totalFrames"> the total number of frames in the animated sprite animation sheet</param>*/
AnimatedSprite::AnimatedSprite(const sf::Texture & texture, AnimationSet * animations) : sf::Sprite(texture){
	setAnimations(animations);
	//initialize sprite to first frame of first animation
	setTextureRect(this->animations->at(0).at(0));
	init();
}


///<summary>Initializes properties to false, 0, or nullptr.</summary>
void AnimatedSprite::init() {

	animating = false;
	currentFrame = 0;
	currentAnimationId = 0;
	currentAnimation = nullptr;
	lastUpdate = lastUpdate.Zero;
	animationDelay = 0;
}

AnimatedSprite::~AnimatedSprite() {
}


//getters and setters

	//setters

///<summary> Whether or not the animated sprite is currently playing an animation</summary>
void AnimatedSprite::setAnimating(bool animating) {
	this->animating = animating;
}

///<summary>sets the current frame (within the current animation) of the animated sprite</summary>
void AnimatedSprite::setCurrentFrame(unsigned int frame) {
	currentFrame = frame;
}

///<summary>sets the animations of the sprite to the passed AnimationSet</summary>
void AnimatedSprite::setAnimations( AnimationSet * animationSet) {
	animations = animationSet->getAnimations();
}

///<summary>Sets the minimum time (in ms) between two animation frames.</summary>
void AnimatedSprite::setAnimationDelay(unsigned int delay) {
	animationDelay = delay;
}

	//getters

///<summary>returns the current frame of the current animation</summary>
unsigned int AnimatedSprite::getCurrentFrame() {
	return currentFrame;
}

///<summary>returns the ID of the current animation</summary>
unsigned int AnimatedSprite::getCurrentAnimationId() {
	return currentAnimationId;
}

///<summary>returns the minimum time (in ms) between two animation frames.</summary>
unsigned int AnimatedSprite::getAnimationDelay() {
	return animationDelay;
}

///<summary>Whether or not the animated sprite is currently playing an animation.</summary>
bool AnimatedSprite::isAnimating() {
	return animating;
}

//operations

/**
<summary>begins a new animation from the first frame</summary>
<param name = "animationId"> the index of the animation to begin </param>
*/
void AnimatedSprite::runAnimation(unsigned int animationId) {
	this->animating = true;
	this->currentAnimationId = animationId;
	this->currentAnimation = &animations->at(animationId);
	this->currentFrame = 0;
}

///<summary>Moves the next frame of the active animation if the sprite is animating</summary>
void AnimatedSprite::update(sf::Time currentTime) {
	
	if (animating && (currentTime.asMilliseconds() - lastUpdate.asMilliseconds() > animationDelay)) {
		setCurrentFrame((currentFrame + 1) % currentAnimation->size());
		setTextureRect(currentAnimation->at(currentFrame));
		lastUpdate = currentTime;
	}
}