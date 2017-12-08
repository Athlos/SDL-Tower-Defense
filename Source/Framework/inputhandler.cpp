// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "interfacemanager.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
	: m_pGameController(0)
	, m_mouseDown(false)
{
}

InputHandler::~InputHandler()
{
}

bool InputHandler::Initialise()
{
	return (true);
}

void InputHandler::ProcessInput(Game& game, InterfaceManager& ui)
{
	// Receive Input Events below...
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			game.Quit();
		}
		
		if (e.type == SDL_MOUSEBUTTONDOWN) // Mouse pressed
		{
			m_mouseDown = true;

			if (e.button.button == SDL_BUTTON_LEFT)
			{
				game.OnLeftMouseClick(e.button.x, e.button.y);

				ui.OnMouseClick(e.button.x, e.button.y);
			}
			else
			{
				game.OnRightMouseClick(e.button.x, e.button.y);
			}
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			m_mouseDown = false;

			ui.OnMouseRelease(e.button.x, e.button.y);
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
					game.UpdateGameState(PAUSED);
				}
				break;
				case SDLK_SPACE:
				{
					game.StartWave();
				}
				break;
			default:
				break;
			}
		}
	}
}
