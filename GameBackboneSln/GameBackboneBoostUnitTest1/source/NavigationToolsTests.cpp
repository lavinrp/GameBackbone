#include "stdafx.h"

#include <Navigation\NavigationTools.h>

#include <SFML\Graphics.hpp>

#include <vector>

using namespace GB;

// Contains all of the tests for Navigation Tools

BOOST_AUTO_TEST_SUITE(NavigationToolsTests)

/// <summary>
/// Struct to store objects that can be reused for all or nearly all 
/// of the unit tests in this file. This struct is meant to be used with fixtures
/// at the unit test level.
/// </summary>
struct ReusableObjects
{
	ReusableObjects() {
		// add sprites
		for (unsigned int i = 0; i < NUM_SPRITES; i++) {
			sprites.push_back(new sf::Sprite());
			destinations.push_back(sf::Vector2f(sinf(i), sinf(i+1))); // should cover all quadrants
			//TODO (Ryan Lavin): initialize paths 
		}

	}

	~ReusableObjects() {
		for (sf::Sprite* sprite : sprites) {
			delete sprite;
		}
	}

	const unsigned int NUM_SPRITES = 100;
	std::vector<sf::Sprite*> sprites;
	std::vector<sf::Vector2f> destinations;
	std::vector<std::list<Point2D<int>>*> paths;
};

// Test moving a large number of sprites to their destinations by taking small steps
BOOST_FIXTURE_TEST_CASE(bulkMoveSpriteStepTowardsPoint_Large_Batch_Small_Step, ReusableObjects) {
	
	//create the vector of movement lengths
	std::vector<float> maxMovementDistances;
	const float MAX_MOVEMENT_DISTANCE = 0.1f;
	for (unsigned int  i = 0; i < NUM_SPRITES; i++) {
		maxMovementDistances.push_back(MAX_MOVEMENT_DISTANCE);
	}
	
	//move the sprites
	const int NUM_MOVES = 1000;
	for (unsigned int i = 0; i < NUM_MOVES; i++) {
		bulkMoveSpriteStepTowardsPoint(sprites, destinations, maxMovementDistances);
	}

	//ensure that all sprites reached their destinations
	bool missedTarget = false;
	for (unsigned int i = 0; i < NUM_SPRITES; i++) {
		if (sprites[i]->getPosition() != destinations[i]) {
			missedTarget = true;
		}
	}
	BOOST_CHECK(!missedTarget);
}

// Test moving a large number of sprites to their destinations by taking medium steps
BOOST_FIXTURE_TEST_CASE(bulkMoveSpriteStepTowardsPoint_Large_Batch_Medium_Step, ReusableObjects) {

	//create the vector of movement lengths
	std::vector<float> maxMovementDistances;
	
	//maximum possible distance of destination from sprite is 1.0
	//it should take 2 moves at this speed to reach the destination.
	const float MAX_MOVEMENT_DISTANCE = 0.5f;
	for (unsigned int i = 0; i < NUM_SPRITES; i++) {
		maxMovementDistances.push_back(MAX_MOVEMENT_DISTANCE);
	}

	//move the sprites
	//extra move is required to compensate for floating point error
	const int NUM_MOVES = 3; 
	for (unsigned int i = 0; i < NUM_MOVES; i++) {
		bulkMoveSpriteStepTowardsPoint(sprites, destinations, maxMovementDistances);
	}

	//ensure that all sprites reached their destinations
	bool missedTarget = false;
	for (unsigned int i = 0; i < NUM_SPRITES; i++) {
		if (sprites[i]->getPosition() != destinations[i]) {
			missedTarget = true;
		}
	}
	BOOST_CHECK(!missedTarget);
	
}

// Test moving a large number of sprites to their destinations by taking large steps
BOOST_FIXTURE_TEST_CASE(bulkMoveSpriteStepTowardsPoint_Large_Batch_large_Step, ReusableObjects) {

	//create the vector of movement lengths
	std::vector<float> maxMovementDistances;
	const float MAX_MOVEMENT_DISTANCE = 1000.0f;
	for (unsigned int i = 0; i < NUM_SPRITES; i++) {
		maxMovementDistances.push_back(MAX_MOVEMENT_DISTANCE);
	}

	//move the sprites
	const int NUM_MOVES = 1;
	for (unsigned int i = 0; i < NUM_MOVES; i++) {
		bulkMoveSpriteStepTowardsPoint(sprites, destinations, maxMovementDistances);
	}

	//ensure that all sprites reached their destinations
	bool missedTarget = false;
	for (unsigned int i = 0; i < NUM_SPRITES; i++) {
		if (sprites[i]->getPosition() != destinations[i]) {
			auto testSpritePos = sprites[i]->getPosition();
			auto testDest = destinations[i];
			missedTarget = true;
		}
	}
	BOOST_CHECK(!missedTarget);
}

// keep at end of file
BOOST_AUTO_TEST_SUITE_END() // end NavigationToolsTests