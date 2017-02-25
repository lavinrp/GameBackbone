#pragma once
#include "stdafx.h"
#include "DllUtil.h"
#include "Array3D.h"
#include "NavigationHexData.h"
#include "PathRequest.h"

#include<SFML/System/Vector3.hpp>

#include<vector>
#include<set>
#include<list>
#include<map>

typedef Array3D<NavigationHexData> NavigationGrid;

/// <summary> used to calculate groups of paths in one navigation grid. </summary>
class libGameBackbone Pathfinder {
public:
	//ctr / dtr
	Pathfinder();
	explicit Pathfinder(NavigationGrid* navigationGrid);
	
	~Pathfinder();

	//getters / setters
	
		//setters
	void setNavigationGrid(NavigationGrid* navigationGrid);

		//getters
	NavigationGrid* getNavigationGrid();

	//operations
	void pathFind(const std::vector<PathRequest>& pathRequests, std::vector<std::list<sf::Vector3i>> * const returnedPaths);

private:

	//helper functions
	sf::Vector3i chooseNextHex(const PathRequest& pathRequest, const std::set<sf::Vector3i> * const availableHexes);
	std::vector<sf::Vector3i> getNeighbors(const sf::Vector3i& hexCoordinate);
	std::list<sf::Vector3i> reconstructPath(const sf::Vector3i & endPoint, std::map<sf::Vector3i, sf::Vector3i> const * const cameFrom);

	//data
	NavigationGrid* navigationGrid;

	//deleted copy and assignment
	Pathfinder(const Pathfinder&) = delete;
	Pathfinder(Pathfinder&&) = delete;
	Pathfinder& operator=(const Pathfinder&) = delete;
	Pathfinder& operator=(Pathfinder&&) = delete;
};



//define required functions to use sf::Vector3i in sets
namespace sf {
	
	/// <summary>
	/// Strict weak ordering less than comparator for sf::Vector3i
	/// </summary>
	/// <param name="lhs">The left hand side sf::Vector3i.</param>
	/// <param name="rhs">The right hand side sf::Vector3i.</param>
	/// <returns>True if lhs is less than rhs, falsee otherwise</returns>
	bool operator<(const sf::Vector3i& lhs, const sf::Vector3i& rhs) {
		if (lhs.x < rhs.x) return true;
		if (lhs.x > rhs.x) return false;
		if (lhs.y < rhs.y) return true;
		if (lhs.y > rhs.y) return false;
		if (lhs.z < rhs.z) return true;
		if (lhs.z > rhs.z) return false;
		return false;
	}
	
	/// <summary>
	/// Strict weak ordering greater than comparator for sf::Vector3i
	/// </summary>
	/// <param name="lhs">The left hand side sf::Vector3i.</param>
	/// <param name="rhs">The right hand side sf::Vector3i.</param>
	/// <returns>True if lhs is greater than rhs, falsee otherwise</returns>
	bool operator>(const sf::Vector3i& lhs, const sf::Vector3i& rhs) {
		return !(lhs < rhs);
	}

}