// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
: m_pGameController(0)
, counter(0)
{
}

InputHandler::~InputHandler()
{
}

bool InputHandler::Initialise()
{
	return (true);
}

void InputHandler::ProcessInput(Game& game)
{
	// Receive Input Events below...
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		
		if (e.type == SDL_MOUSEBUTTONDOWN) // Mouse pressed
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				game.OnLeftMouseClick(e.button.x, e.button.y);
			}
			else
			{
				game.OnRightMouseClick(e.button.x, e.button.y);
			}
		}

		int xPos;
		int yPos;

		SDL_GetMouseState(&xPos, &yPos);

		game.UpdateCursorPosition(xPos, yPos);
		
		switch (e.type)
		{
			/* Look for a keypress */
		case SDL_KEYDOWN:
			/* Check the SDLKey values*/
			switch (e.key.keysym.sym)
			{
				case SDLK_ESCAPE: // Escape pressed
				{
					game.Quit();
				}
				break;
				case SDLK_SPACE:
				{
					game.StartWave();
				}
				break;
				case SDLK_1:
				{
					int xPos;
					int yPos;

					SDL_GetMouseState(&xPos, &yPos);

					game.PlaceTower(xPos, yPos);
				}
				break;
			default:
				break;
			}
		}
	}
}
