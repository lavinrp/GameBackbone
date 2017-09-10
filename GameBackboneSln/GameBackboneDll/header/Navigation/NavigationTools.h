#pragma once

#include <Backbone/CompoundSprite.h>
#include <Navigation/NavigationGridData.h>
#include <Util/Array2D.h>
#include <Util/DllUtil.h>
#include <Util/Point.h>
#include <Util/UtilMath.h>

#include <SFML/Graphics.hpp>

#include <list>
#include <memory>
#include <math.h>
#include <set>
#include <vector>


namespace GB {
	libGameBackbone typedef Array2D<NavigationGridData*> NavigationGrid;
	libGameBackbone typedef std::list<sf::Vector2f> WindowCoordinatePath;
	libGameBackbone typedef std::shared_ptr<WindowCoordinatePath> WindowCoordinatePathPtr;
	libGameBackbone typedef std::list<Point2D<int>> NavGridCoordinatePath;

	//---------------------------------------------------------------------------------------------------------------------
	// NavigationGrid memory


	/// <summary>
	/// Initializes all navigation grid values to the passed value.
	/// </summary>
	/// <param name="navGrid">The navigation grid to fill with values</param>
	/// <param name="gridData">The NavigationGridData to copy into each index of the Navigation Grid.</param>
	template <class NavigationDataType>
	extern void initAllNavigationGridValues(NavigationGrid& navGrid, NavigationDataType& gridData) {
		for (unsigned int ii = 0; ii < navGrid.getArraySizeX(); ++ii) {
			for (unsigned int jj = 0; jj < navGrid.getArraySizeY(); ++jj)
			{
				NavigationGridData* navData = static_cast<NavigationGridData*>(new NavigationDataType(gridData));
				navGrid[ii][jj] = navData;
			}
		}
	}

	libGameBackbone extern void freeAllNavigationGridData(NavigationGrid& navGrid);

	//---------------------------------------------------------------------------------------------------------------------
	// sprite movement to point



	/// <summary>
	/// Moves any movable object a step towards a destination.
	/// </summary>
	/// <param name="movableObject">The movable object.</param>
	/// <param name="destination">The destination.</param>
	/// <param name="maxStepLength">Maximum length of the step.</param>
	/// <param name="distanceToDestination">Out Value: The distance to destination.</param>
	/// <param name="angleToDest">Out Value: The angle to the destination.</param>
	template <class T>
	static inline void moveMovableStepTowardsPoint(T& movableObject,
												   const sf::Vector2f& destination,
												   const float maxStepLength,
												   float& distanceToDestination,
												   float& angleToDest) {

		// calculate the angle to the destination
		const sf::Vector2f currentPosition = movableObject.getPosition();
		angleToDest = atan2f(destination.y - currentPosition.y, destination.x - currentPosition.x);

		// Calculate the distance to the destination
		distanceToDestination = calcDistance2D<float, sf::Vector2f>(currentPosition, destination);

		// Move directly to the destination if it's within reach
		if (distanceToDestination <= maxStepLength) {
			movableObject.setPosition(destination);
		}
		else { // Move the sprite as close as possible to the destination
			float xProgress = cosf(angleToDest) * maxStepLength;
			float yProgress = sinf(angleToDest) * maxStepLength;

			movableObject.move(xProgress, yProgress);
		}
	}

	/// <summary>
	/// Moves the sprite in the direction of the destination.
	/// The sprite will never overshoot the destination.
	/// </summary>
	/// <param name="sprite">The sprite to move.</param>
	/// <param name="destination">The destination.</param>
	/// <param name="maxStepLength">Maximum length that the sprite can move.</param>
	/// <param name="orientSpriteToDestination">Orients sprites towards their destination if true. Does not orient sprites otherwise.</param>
	inline void moveSpriteStepTowardsPoint(sf::Sprite& sprite,
										   const sf::Vector2f& destination,
										   const float maxStepLength, 
										   const bool orientSpriteToDestination = true) {

		// move the sprite to
		float angleToDest;
		float distanceToDestination;

		// Move the sprite. Find its distance to destination and its angle to the destination.
		moveMovableStepTowardsPoint(sprite, destination, maxStepLength, distanceToDestination, angleToDest);

		// rotate the sprite if rotation is on
		if (orientSpriteToDestination && distanceToDestination != 0) {
			sprite.setRotation(angleToDest * 180.0f / (float)M_PI);
		}

	}


	/// <summary>
	/// Moves the CompoundSprite in the direction of the destination.
	/// The sprite will never overshoot the destination.
	/// </summary>
	/// <param name="sprite">The CompoundSprite to move.</param>
	/// <param name="destination">The destination.</param>
	/// <param name="maxStepLength">Maximum length that the sprite can move.</param>
	/// <param name="orientSpriteToDestination">Orients sprites towards their destination if true. Does not orient sprites otherwise.</param>
	inline void moveCompoundSpriteStepTowardsPoint(CompoundSprite& sprite,
												   const sf::Vector2f& destination,
												   const float maxStepLength,
												   const std::set<size_t>& spritesToRotate) {

		// move the sprite. Calculate its distance and angle to its destination.
		float angleToDestination;
		float distanceToDestination;
		moveMovableStepTowardsPoint(sprite, destination, maxStepLength, distanceToDestination, angleToDestination);

		// orient components towards destination
		if (distanceToDestination != 0) {
			sprite.setRotationOfComponents(spritesToRotate, angleToDestination * 180.0f / (float)M_PI);
		}

	}

	//---------------------------------------------------------------------------------------------------------------------
	// sprite movement along path

	/// <summary>
	/// Moves a sprite one step forward along path.
	/// The sprite will not necessarily reach the end of the path after one call to this function.
	/// The sprite will stop after reaching each point in the path even if it is capable of moving farther.
	/// </summary>
	/// <param name="sprite">The sprite.</param>
	/// <param name="path">The path.</param>
	/// <param name="msPassed">Time passed in ms.</param>
	/// <param name="distPerMs">The maximum distance that the sprite can move per ms.</param>
	/// <param name="orientSpriteToDestination">Whether or not the sprites should be oriented to face their destinations.</param>
	inline void moveSpriteAlongPath(sf::Sprite& sprite, 
									WindowCoordinatePathPtr path,
									sf::Uint64 msPassed,
									float distPerMs,
									const bool orientSpriteToDestination = true) {

		if (path->empty()) {
			return;
		}

		const sf::Vector2f destination = path->front();
		const float maxStepLength = msPassed*distPerMs;

		moveSpriteStepTowardsPoint(sprite, destination, maxStepLength, orientSpriteToDestination);

		// if the sprite has reached destination, move on to next point in path
		if (sprite.getPosition() == destination) {
			path->pop_front();
		}
	}


	/// <summary>
	/// Moves a CompoundSprite one step forward along path.
	/// The sprite will not necessarily reach the end of the path after one call to this function.
	/// The sprite will stop after reaching each point in the path even if it is capable of moving farther.
	/// </summary>
	/// <param name="sprite">The sprite.</param>
	/// <param name="path">The path.</param>
	/// <param name="msPassed">Time passed in ms.</param>
	/// <param name="distPerMs">The maximum distance that the sprite can move per ms.</param>
	/// <param name="spritesToRotate">The indices of the sprites that should be rotated to face their destinations.</param>
	inline void moveCompoundSpriteAlongPath(CompoundSprite& sprite,
											WindowCoordinatePathPtr path,
											sf::Uint64 msPassed,
											float distPerMs,
											const std::set<size_t>& spritesToRotate) {

		// do nothing if there is no path to follow
		if (path->empty()) {
			return;
		}

		// discover and move to the next destination
		const sf::Vector2f destination = path->front();
		const float maxStepLength = msPassed*distPerMs;
		moveCompoundSpriteStepTowardsPoint(sprite, destination, maxStepLength, spritesToRotate);

		// if the sprite has reached destination, move on to next point in path
		if (sprite.getPosition() == destination) {
			path->pop_front();
		}

	}

	//---------------------------------------------------------------------------------------------------------------------
}