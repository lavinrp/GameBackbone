#include <Navigation\CoordinateConverter.h>
#include <Util\Point.h>

#include <SFML\Graphics.hpp>

using namespace GB;

/// <summary>
/// Initializes a new instance of the <see cref="CoordinateConverter"/> class.
/// Default values of 50 and (0, 0) for gridSquareWidth and originOffset respectively.
/// </summary>
CoordinateConverter::CoordinateConverter() {
	gridSquareWidth = 50.f;
	originOffset.x = originOffset.y = 0;
}

/// <summary>
/// Initializes a new instance of the <see cref="CoordinateConverter"/> class.
/// </summary>
/// <param name="WidthOfGridSquares">The width of grid squares.</param>
/// <param name="OffsetOfOrigin">The offset of origin.</param>
CoordinateConverter::CoordinateConverter(float widthOfGridSquares, Point2D<float> offsetOfOrigin) : originOffset(offsetOfOrigin) {
	gridSquareWidth = widthOfGridSquares;
}

/// <summary>
/// Converts the coordinate to an sf window coordinate.
/// This function returns the center of the sf Grid Square
/// </summary>
/// <param name="NavGridCoord">The navigation grid coordinate.</param>
/// <returns></returns>
sf::Vector2<float> CoordinateConverter::ConvertCoordToWindow(const Point2D<int>& navGridCoord) {
	sf::Vector2<float> windowCoordTopLeft = { navGridCoord.x*gridSquareWidth, navGridCoord.y*gridSquareWidth };

	sf::Vector2<float> windowCoordCenter = { windowCoordTopLeft.x + (gridSquareWidth /2), windowCoordTopLeft.y + (gridSquareWidth / 2)};

	sf::Vector2<float> offsetWindowCoord = { windowCoordCenter.x + originOffset.x, windowCoordCenter.y + originOffset.y};
	return offsetWindowCoord;
}

/// <summary>
/// Converts the coordinate to a navigation grid coordinate.
/// The value of right/bottom edge is not contained by the grid square square 
/// ei: [0, gridSquareWidth)
/// ei: gridSquareWidth = 50; WindowCoord(49.999, 50) -> NavGridCoord(0, 1)
/// </summary>
/// <param name="WindowCoord">The sf window coordinate.</param>
/// <returns></returns>
Point2D<int> CoordinateConverter::ConvertCoordToNavGrid(const sf::Vector2<float>& windowCoord) {
	sf::Vector2<float> windowCoordCenter = { windowCoord.x - originOffset.x, windowCoord.y - originOffset.y};

	Point2D<int> navGridCoord = {(int)(windowCoordCenter.x/ gridSquareWidth), (int)(windowCoordCenter.y/ gridSquareWidth)};
	return navGridCoord;
}

/// <summary>
/// Sets the width of the grid square.
/// </summary>
/// <param name="newWidth">The new width.</param>
void CoordinateConverter::setGridSquareWidth(float newWidth) {
	gridSquareWidth = newWidth;
}

/// <summary>
/// Sets the origin offset.
/// </summary>
/// <param name="newOffset">The new offset.</param>
void CoordinateConverter::setOriginOffset(const Point2D<float>& newOffset) {
	originOffset = newOffset;
}