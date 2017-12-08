#pragma once

// Library includes:
#include <SDL.h>

// Forward Declarations:
class Game;
class InterfaceManager;

class InputHandler
{
	// Member Methods:
public:
	InputHandler();
	~InputHandler();

	bool Initialise(); // Set up handler
	void ProcessInput(Game& game, InterfaceManager& ui); // Process all inputs

private:
	InputHandler(const InputHandler& inputHandler);
	InputHandler& operator=(const InputHandler& inputHandler);

	// Member Data:
protected:
	SDL_Joystick* m_pGameController;
	bool m_mouseDown;
};