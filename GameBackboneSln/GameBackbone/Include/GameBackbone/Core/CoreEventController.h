#pragma once

#include <GameBackbone/Core/GameRegion.h>

#include <string>
#include <iostream>

namespace GB {

	static const int DEFAULT_WINDOW_HEIGHT = 700;
	static const int DEFAULT_WINDOW_WIDTH = 700;
	static const std::string DEFAULT_WINDOW_NAME = "GameBackbone";

	/// <summary>
	/// Base class meant to be inherited. Allows standard operation order for main game loop for clients of GB.
	/// Also provides simple and concise way to run the main game loop.
	/// Clients should always override the empty handleCoreEvent.
	///
	/// The execution order of the helper functions is 1) preHandleEvent 2) handleGuiEvent 3) handleCoreEvent, 4) postHandleEvent,
	/// 5) preDraw, 6) coreDraw, 7) postDraw, 8) preUpdate, 9) coreUpdate, 10) postUpdate
	///
	/// </summary>
	template <class Child>
	class CoreEventController {
	public:
		//ctr / dtr

		/// <summary>
		/// Initializes a new instance of the <see cref="CoreEventController"/> class. Window width, height, and name are default.
		/// </summary>
		CoreEventController() : CoreEventController(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_NAME) {}

		/// <summary>
		/// Initializes a new instance of the <see cref="CoreEventController"/> class. Window width and height are default.
		/// </summary>
		/// <param name="windowName">Name of the window.</param>
		explicit CoreEventController(const std::string& windowName) : CoreEventController(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, windowName) {}

		/// <summary>
		/// Initializes a new instance of the <see cref="CoreEventController"/> class. Window name is default.
		/// </summary>
		/// <param name="windowWidth">Width of the window.</param>
		/// <param name="windowHeight">Height of the window.</param>
		CoreEventController(int windowWidth, int windowHeight) : CoreEventController(windowWidth, windowHeight, DEFAULT_WINDOW_NAME){}

		/// <summary>
		/// Initializes a new instance of the <see cref="CoreEventController"/> class.
		/// </summary>
		/// <param name="windowWidth">Width of the window.</param>
		/// <param name="windowHeight">Height of the window.</param>
		/// <param name="windowName">Name of the window.</param>
		CoreEventController(int windowWidth, int windowHeight, const std::string & windowName) {
			window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), windowName);
			activeRegion = nullptr;
		}

		/// <summary>
		/// Finalizes an instance of the <see cref="CoreEventController"/> class.
		/// </summary>
		~CoreEventController() {
			delete window;
		}

		CoreEventController(const CoreEventController& other) = delete;
		CoreEventController(CoreEventController&& other) = default;
		CoreEventController& operator=(const CoreEventController& other) = delete;
		CoreEventController& operator=(CoreEventController&& other) = default;

		//operations
		void runLoop() {
			sf::Event event;

			//Ensure window is fully opened before we do any work on it.
			while (window->isOpen() == false) {
				continue;
			}

			while (window->isOpen()) {

				while (window->pollEvent(event)) {
					childCastThis()->handleEvent(event);
				}

				childCastThis()->draw();

				childCastThis()->update();
			}
		}

		/// <summary>
		/// Changes the active region to the passed game region.
		/// </summary>
		/// <param name="newActiveRegion">The new active GameRegion.</param>
		void setActiveRegion(GameRegion* newActiveRegion) {
			activeRegion = newActiveRegion;
		}

protected:
		//events

		/// <summary>
		/// Handles all window and user input events.
		/// </summary>
		/// <param name="event">The event.</param>
		void handleEvent(sf::Event& event) {
			childCastThis()->preHandleEvent(event);
			if (!childCastThis()->handleGuiEvent(event)) {
				childCastThis()->handleCoreEvent(event);
			}
			childCastThis()->postHandleEvent(event);
		}

		/// <summary>
		/// Handles the GUI event.
		/// </summary>
		/// <param name="event">The event.</param>
		/// <returns>Returns true if the event was consumed by the GUI. Returns false otherwise.</returns>
		bool handleGuiEvent(sf::Event& event) {
			return activeRegion->getGUI().handleEvent(event);
		}

		/// <summary>
		/// Handles the non GUI event.
		/// </summary>
		/// <param name="event">The event.</param>
		/// <returns>Returns true if the event was consumed. Returns false otherwise.</returns>
		bool handleCoreEvent(sf::Event& event) {
			return false;
		}

		/// <summary>
		/// Called before handleGuiEvent and handleCoreEvent. Place logic to be executed before the main event logic here.
		/// </summary>
		/// <param name="event">The event.</param>
		void preHandleEvent(sf::Event& event) {}

		/// <summary>
		/// Called after handleGuiEvent and handleCoreEvent. Place logic to be executed after the main event logic here.
		/// </summary>
		/// <param name="event">The event.</param>
		void postHandleEvent(sf::Event& event) {}

		//draw

		/// <summary>
		/// Calls all draw helper functions then displays the window.
		/// </summary>
		void draw() { 	
			window->clear();
			childCastThis()->preDraw();
			childCastThis()->coreDraw();
			childCastThis()->postDraw();

			window->display();
		}

		/// <summary>
		/// Called before coreDraw. PLace logic to be executed before the main draw logic here.
		/// </summary>
		void preDraw() {}

		/// <summary>
		/// Primary drawing logic. Draws every drawable object in the game region and the active regions gui.
		/// </summary>
		void coreDraw() {
			// Draw the activeRegion so it can draw its drawables.
			window->draw(*activeRegion);

			activeRegion->getGUI().draw();
		}

		/// <summary>
		/// called after core draw. Place logic to be executed after the main draw logic here.
		/// </summary>
		void postDraw() {}

		//update

		/// <summary>
		/// Calls all update helper functions (preUpdate, coreUpdate, postUpdate).
		/// </summary>
		void update() {
			childCastThis()->preUpdate();
			childCastThis()->coreUpdate();
			childCastThis()->postUpdate();
		}

		/// <summary>
		/// called before coreUpdate. Place logic meant to execute before the main update logic here.
		/// </summary>
		void preUpdate() {}

		/// <summary>
		/// Primary update logic. Runs behavior logic for active GameRegion. Updates every Updatable object in the active GameRegion.
		/// </summary>
		void coreUpdate() {
			sf::Time elapsedTime = updateClock.restart();
			activeRegion->update(elapsedTime.asMicroseconds());
		}

		/// <summary>
		/// Executes after coreUpdate. Place logic meant to update after the main update logic here.
		/// </summary>
		void postUpdate() {}

		sf::Clock updateClock;
		sf::RenderWindow* window;
		GameRegion* activeRegion;

	private:
		Child* childCastThis() {
			return static_cast<Child*>(this);
		}

	};

}

