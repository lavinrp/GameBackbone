#pragma once

#include <Backbone/AnimationSet.h>
#include <Backbone/BackboneBaseExceptions.h>
#include <Backbone/CompoundSprite.h>
#include <Backbone/Updatable.h>
#include <Util/DllUtil.h>

#include <TGUI/TGUI.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <functional>
#include <list>
#include <vector>

namespace GB {

	/// <summary> Base class meant to be inherited. Controls game logic and actors for a specific time or space in game. </summary>
	class libGameBackbone GameRegion {
	public:
		//ctr / dtr
		GameRegion();
		explicit GameRegion(sf::RenderWindow & window);

		virtual ~GameRegion();

		//getters / setters
		//setters
		void setUpdatable(bool status, Updatable* object);
		void setDrawable(bool status, sf::Sprite* object);
		void setDrawable(bool status, CompoundSprite* object);
		void setDrawAndUpdateable(bool status, AnimatedSprite* object);
		void setDrawAndUpdateable(bool status, CompoundSprite* object);
		void setParentRegion(GameRegion* newParent);

		//getters
		std::list<Updatable*>const * const getUpdatables();
		std::list<sf::Sprite*>const * const getDrawables();
		std::vector<GameRegion*>* getNeighborRegions();
		std::vector<GameRegion*>* getChildRegions();
		GameRegion* getParentRegion();
		tgui::Gui* getGUI();


		//internal behavior alteration

		/// <summary>
		/// Resets this instance.
		/// </summary>
		virtual void reset() {}

		/// <summary>
		/// Runs the game behaviors and logic for this instance.
		/// </summary>
		virtual void behave(sf::Time currentTime) {}

		virtual void registerSetActiveRegionCB(std::function<void(GameRegion*)> newSetActiveRegionCB);

		//general operations

		//additions
		void addChildRegion(GameRegion* childToAdd);
		void addNeighborRegion(GameRegion* neighborToAdd);
		//removals
		void removeNeighborRegion(GameRegion* neighborToRemove);
		void removeChildRegion(GameRegion* childToRemove);
		//clears
		void clearUpdatable();
		void clearChildren();
		void clearDrawable();
		void clearNeighborRegions();

	protected:

		//operations
		void clearAssociations(std::function<void(GameRegion*)> memberFunctionPointer, std::vector<GameRegion*>* container);

		//internal logic members
		std::list<sf::Sprite*>* drawables;
		std::list<Updatable*>* updatables;

		//associated regions
		GameRegion* parentRegion;
		std::vector<GameRegion*> childRegions;
		std::vector<GameRegion*> neighborRegions;
		std::function<void(GameRegion*)> setActiveRegionCB;

		//GUI
		tgui::Gui* regionGUI;

	private:
		//deleted copy and assignment ctr
		GameRegion(const GameRegion&) = delete;
		GameRegion& operator=(const GameRegion&) = delete;
		GameRegion(GameRegion&&) = delete;
		GameRegion& operator=(GameRegion&&) = delete;

	};

}
