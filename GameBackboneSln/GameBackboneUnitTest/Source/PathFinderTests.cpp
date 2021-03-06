#include "stdafx.h"

#include <GameBackbone/Navigation/NavigationTools.h>
#include <GameBackbone/Navigation/PathFinder.h>

#include <SFML/System/Vector2.hpp>

#include <chrono>
#include <sstream>

using namespace GB;

BOOST_AUTO_TEST_SUITE(Pathfinder_Tests)

BOOST_AUTO_TEST_SUITE(Pathfinder_CTRs)

BOOST_AUTO_TEST_CASE(Pathfinder_default_CTR_test) {

	Pathfinder* pathfinder = new Pathfinder();

	NavigationGrid* navGrid = pathfinder->getNavigationGrid();
	BOOST_CHECK(nullptr == navGrid);

	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_navigationGrid_CTR_test) {
	NavigationGrid navGrid(10);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure the grid is not copied;
	BOOST_CHECK_EQUAL(&navGrid, pathfinder->getNavigationGrid());

	delete pathfinder;
}

BOOST_AUTO_TEST_SUITE_END() // end Pathfinder_CTRs

BOOST_AUTO_TEST_CASE(Pathfinder_setNavigationGrid) {
	NavigationGrid navGrid(10);
	Pathfinder* pathfinder = new Pathfinder();

	//ensure the grid is not copied;
	pathfinder->setNavigationGrid(&navGrid);
	BOOST_CHECK_EQUAL(&navGrid, pathfinder->getNavigationGrid());

	delete pathfinder;
}

BOOST_AUTO_TEST_SUITE(Pathfinder_pathFind_Tests)

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_one_simple_path_no_sol) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//block all squares
	initAllNavigationGridValues(navGrid, NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 });

	//Set value at the midPoint
	sf::Vector2i startPoint{ 1, 1 };
	navGrid.at(startPoint.x, startPoint.y)->weight = 0;

	//create request
	PathRequest pathRequest{ startPoint, sf::Vector2i{2,2}};
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);


	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());
	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned path is empty
	BOOST_CHECK(pathsReturn[0].size() == 0);

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_one_simple_path_no_blocker) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all grid squares are clear
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	//create request
	sf::Vector2i startPoint{ 1, 1 };
	PathRequest pathRequest{ startPoint, sf::Vector2i{2,2}};
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);


	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());
	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned path is not empty
	BOOST_CHECK(pathsReturn[0].size() > 0);

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_one_path_single_blocker) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all grid squares are clear
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	//create request
	sf::Vector2i startPoint{ 0, 0 };
	PathRequest pathRequest{ startPoint, sf::Vector2i{2,0}};
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);


	//block grid square between start and end point
	sf::Vector2i blockedSquareCoord{ 1, 0 };
	NavigationGridData blockedSquareData;
	blockedSquareData.weight = BLOCKED_GRID_WEIGHT;
	blockedSquareData.blockerDist = 0;
	*navGrid.at(blockedSquareCoord.x, blockedSquareCoord.y) = blockedSquareData;


	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());

	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned path is not empty
	BOOST_CHECK(pathsReturn[0].size() > 0);

	//ensure the blocked grid square is not in the path

	for (sf::Vector2i gridSquare : pathsReturn[0]) {
		BOOST_CHECK(gridSquare != blockedSquareCoord);
	}

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_to_start) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all squares are clear
	//initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	//create request
	sf::Vector2i startPoint{ 0, 0 };
	PathRequest pathRequest{ startPoint, startPoint};
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);

	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());

	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned path is empty
	BOOST_CHECK(pathsReturn[0].size() == 0);

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_one_path_end_blocked) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all squares are blocked
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	//block end point
	sf::Vector2i endPoint{ 1, 1 };
	navGrid[endPoint.x][endPoint.y]->weight = BLOCKED_GRID_WEIGHT;

	//create request
	sf::Vector2i startPoint{ 0, 0 };
	PathRequest pathRequest{ startPoint, endPoint };
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);

	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());

	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned path is empty
	BOOST_CHECK(pathsReturn[0].size() == 0);

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_single_request_simple_maze) {
	const int X_DIM = 7;
	const int Y_DIM = 6;

	NavigationGrid navGrid(X_DIM, Y_DIM);
	//ensure all squares are clear;
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	/*

	representation of the maze
	Key:
	0: clear
	1: blocked
	S: start
	F: finish

	001F00
	001000
	011000
	0S1110
	001000
	001000
	000000


	*/



	//block grids for maze
	*navGrid[2][0] = NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 };
	*navGrid[2][1] = NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 };
	*navGrid[2][2] = NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 };
	*navGrid[2][3] = NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 };
	*navGrid[2][4] = NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 };
	*navGrid[1][2] = NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 };
	*navGrid[3][3] = NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 };
	*navGrid[4][3] = NavigationGridData{ BLOCKED_GRID_WEIGHT, 0 };


	//create pathfinder
	Pathfinder pathfinder(&navGrid);

	//create path request
	sf::Vector2i startPoint{ 1, 3 };
	sf::Vector2i goalPoint{ 3, 0 };
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(PathRequest{ startPoint, goalPoint });

	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());

	//find the path
	pathfinder.pathFind(pathRequests, &pathsReturn);

	//ensure the path is not empty
	BOOST_CHECK(pathsReturn[0].size() > 0);

	freeAllNavigationGridData(navGrid);
}

BOOST_AUTO_TEST_CASE(Pathfinder_two_path_both_clear) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all grid squares are clear
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	//create request
	sf::Vector2i startPoint{ 1, 1 };
	PathRequest pathRequest{ startPoint, sf::Vector2i{2,2} };
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);

	//second request
	sf::Vector2i startPoint2{ 1, 1 };
	PathRequest pathRequest2{ startPoint2, sf::Vector2i{0,0} };
	pathRequests.push_back(pathRequest2);


	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());
	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned paths are not empty
	BOOST_CHECK(pathsReturn[0].size() > 0);
	BOOST_CHECK(pathsReturn[1].size() > 0);

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_two_path_both_blocked) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all grid squares are not clear
	initAllNavigationGridValues(navGrid, NavigationGridData{ BLOCKED_GRID_WEIGHT,0 });

	//create request
	sf::Vector2i startPoint{ 1, 1 };
	PathRequest pathRequest{ startPoint, sf::Vector2i{2,2} };
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);

	//second request
	sf::Vector2i startPoint2{ 2, 2 };
	PathRequest pathRequest2{ startPoint2, sf::Vector2i{0,0} };
	pathRequests.push_back(pathRequest2);

	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());
	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned paths are empty
	BOOST_CHECK(pathsReturn[0].size() == 0);
	BOOST_CHECK(pathsReturn[1].size() == 0);

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_two_path_one_blocked) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all grid squares are not clear
	initAllNavigationGridValues(navGrid, NavigationGridData{ BLOCKED_GRID_WEIGHT,0 });


	//clean path for first request

	/*

	representation of the maze
	Key:
	0: clear
	1: blocked
	S: start
	F: finish

	S1  1   S2
	0   1   1
	F1  1   F2


	*/

	navGrid[0][0]->weight = 0;
	navGrid[0][1]->weight = 0;
	navGrid[0][2]->weight = 0;

	//create request
	sf::Vector2i startPoint{ 0, 0 };
	PathRequest pathRequest{ startPoint, sf::Vector2i{0,2} };
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);

	//second request
	sf::Vector2i startPoint2{ 2, 0 };
	PathRequest pathRequest2{ startPoint2, sf::Vector2i{2,2} };
	pathRequests.push_back(pathRequest2);


	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());
	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned paths are correct
	BOOST_CHECK(pathsReturn[0].size() > 0);
	BOOST_CHECK(pathsReturn[1].size() == 0);

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_one_path_single_left_blocker) {
	const int SQUARE_DIM = 3;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all grid squares are clear
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	//create request
	sf::Vector2i startPoint{ 2, 0 };
	PathRequest pathRequest{ startPoint, sf::Vector2i{0,0} };
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);


	//block grid square between start and end point
	sf::Vector2i blockedSquareCoord{ 1, 0 };
	NavigationGridData blockedSquareData;
	blockedSquareData.weight = BLOCKED_GRID_WEIGHT;
	blockedSquareData.blockerDist = 0;
	*navGrid.at(blockedSquareCoord.x, blockedSquareCoord.y) = blockedSquareData;


	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());

	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned path is not empty
	BOOST_CHECK(pathsReturn[0].size() > 0);

	//ensure the blocked grid square is not in the path
	for (sf::Vector2i gridSquare : pathsReturn[0]) {
		BOOST_CHECK(gridSquare != blockedSquareCoord);
	}

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_two_path_different_weight_paths) {
	const int SQUARE_DIM = 2;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all grid squares are clear
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	//create request
	sf::Vector2i startPoint{ 1, 1 };
	PathRequest pathRequest{ startPoint, sf::Vector2i{0,0} };
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);


	//Make square that is left have weight 1
	sf::Vector2i leftSquareCoord{ 0, 1 };
	NavigationGridData leftSquareData;
	leftSquareData.weight = 1;
	leftSquareData.blockerDist = 0;
	*navGrid.at(leftSquareCoord.x, leftSquareCoord.y) = leftSquareData;

	//Make square that is up have weight 2
	sf::Vector2i upSquareCoord{ 1, 0 };
	NavigationGridData upSquareData;
	upSquareData.weight = 500;
	upSquareData.blockerDist = 0;
	*navGrid.at(upSquareCoord.x, upSquareCoord.y) = upSquareData;


	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());

	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned path is not empty
	BOOST_CHECK(pathsReturn[0].size() > 0);

	//ensure the blocked grid square is not in the path
	for (sf::Vector2i gridSquare : pathsReturn[0]) {
		BOOST_CHECK(gridSquare != upSquareCoord);
	}

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_two_path_different_weight_paths_flipped_weights) {
	const int SQUARE_DIM = 2;
	NavigationGrid navGrid(SQUARE_DIM);
	Pathfinder* pathfinder = new Pathfinder(&navGrid);

	//ensure all grid squares are clear
	initAllNavigationGridValues(navGrid, NavigationGridData{ 0,0 });

	//create request
	sf::Vector2i startPoint{ 1, 1 };
	PathRequest pathRequest{ startPoint, sf::Vector2i{0,0} };
	std::vector<PathRequest> pathRequests;
	pathRequests.push_back(pathRequest);


	//Make square that is left have weight 2
	sf::Vector2i leftSquareCoord{ 0, 1 };
	NavigationGridData leftSquareData;
	leftSquareData.weight = 500;
	leftSquareData.blockerDist = 0;
	*navGrid.at(leftSquareCoord.x, leftSquareCoord.y) = leftSquareData;

	//Make square that is up have weight 1
	sf::Vector2i upSquareCoord{ 1, 0 };
	NavigationGridData upSquareData;
	upSquareData.weight = 1;
	upSquareData.blockerDist = 0;
	*navGrid.at(upSquareCoord.x, upSquareCoord.y) = upSquareData;


	//create return value
	std::vector<std::deque<sf::Vector2i>> pathsReturn;
	pathsReturn.resize(pathRequests.size());

	//find the path
	pathfinder->pathFind(pathRequests, &pathsReturn);

	//ensure the returned path is not empty
	BOOST_CHECK(pathsReturn[0].size() > 0);

	//ensure the blocked grid square is not in the path
	for (sf::Vector2i gridSquare : pathsReturn[0]) {
		BOOST_CHECK(gridSquare != leftSquareCoord);
	}

	freeAllNavigationGridData(navGrid);
	delete pathfinder;
}

BOOST_AUTO_TEST_SUITE_END() // end Pathfinder_pathFind_Tests


BOOST_AUTO_TEST_SUITE(Pathfinder_perf_Tests)

// BOOST_AUTO_TEST_CASE(Pathfinder_pathFind_many_long_paths) {
// 	const int SQUARE_DIM = 1000;
// 	const int NUM_REQUESTS = 1000;
// 
// 	NavigationGrid navGrid(SQUARE_DIM);
// 	Pathfinder* pathfinder = new Pathfinder(&navGrid);
// 
// 	//ensure all grid squares are not clear
// 	initAllNavigationGridValues(navGrid, NavigationGridData{ BLOCKED_GRID_WEIGHT,0 });
// 
// 	//clean perimeter
// 	for (int i = 0; i < SQUARE_DIM; ++i) {
// 		// clear top row
// 		navGrid.at(0, i)->weight = 0;
// 		// clear left col
// 		navGrid.at(i, 0)->weight = 0;
// 		// clear right col
// 		navGrid.at(i, SQUARE_DIM - 1)->weight = 0;
// 		// clear bottom row
// 		navGrid.at(SQUARE_DIM - 1, i)->weight = 0;
// 	}
// 	// reblock short path
// 	navGrid.at(0, 1)->weight = BLOCKED_GRID_WEIGHT;
// 
// 	/*
// 	representation of the maze
// 	Key:
// 	0: clear
// 	1: blocked
// 	S: start
// 	F: finish
// 
// 	S   1  F    0  0
// 	0   1  ...  1  0
// 	0   1  ...  1  0
//     ... 1  ...  1  ...
//     0  ... 0 ...   0
// 	*/
// 
// 	//create requests
// 	sf::Vector2i startPoint{ 0, 0 };
// 	sf::Vector2i endPoint{ 0, 2 };
// 
// 	PathRequest pathRequest{ startPoint, endPoint };
// 	std::vector<PathRequest> pathRequests;
// 	
// 	for (int i = 0; i < NUM_REQUESTS; ++i) {
// 		pathRequests.push_back(pathRequest);
// 	}
// 
// 	//create return value
// 	std::vector<std::deque<sf::Vector2i>> pathsReturn;
// 	pathsReturn.resize(pathRequests.size());
// 	//find the path
// 	auto startTime = std::chrono::high_resolution_clock::now();
// 	pathfinder->pathFind(pathRequests, &pathsReturn);
// 	auto endTime = std::chrono::high_resolution_clock::now();
// 
// 	BOOST_CHECK(pathsReturn[0].size() > 0);
// 
// 	std::chrono::duration<double> elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
// 	std::stringstream strStream;
// 	strStream << "Pathfinding time of: " << elapsedTime.count() << " seconds for pathfinding perf test";
// 	BOOST_TEST_MESSAGE(strStream.str());
// 
// 	freeAllNavigationGridData(navGrid);
// 	delete pathfinder;
// }

BOOST_AUTO_TEST_SUITE_END() // end Pathfinder_perf_Tests


BOOST_AUTO_TEST_SUITE_END() // end Pathfinder_tests