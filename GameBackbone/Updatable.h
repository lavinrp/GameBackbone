#pragma once
#include "DllUtil.h"
#include<SFML\System\Clock.hpp>

/// <summary> Abstract class meant to be inherited. Class that is capable of being updated. </summary>
libGameBackbone class Updatable {
public:

	Updatable() {
	}

	virtual ~Updatable() {
	}

	virtual void update(sf::Time currentTime) = 0;

protected:
	sf::Time lastUpdate;
};

