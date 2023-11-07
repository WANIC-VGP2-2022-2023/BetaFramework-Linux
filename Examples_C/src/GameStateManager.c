//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 1
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"

#include "GameStateTable.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct
{
	GameStates	current;	/* Index of the current game state. */
	GameStates	previous;	/* Index of the previous game state. */
	GameStates	next;		/* Index of the next game state. */

} GameState;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static GameState gameState;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static bool	GameStateIsChanging();

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game state manager.
void GameStateManagerInit(BetaObject* obj)
{
	UNREFERENCED_PARAMETER(obj);

	printf("GSM: Init\n");

	// Set the previous and current game states to invalid.
	gameState.previous = GsInvalid;
	gameState.current = GsInvalid;

	// Set the initial game state to launch the game.
	gameState.next = GsInitial;
}

// Update the game state manager.
void GameStateManagerUpdate(BetaObject* obj, float dt)
{
	UNREFERENCED_PARAMETER(obj);

	//printf("GSM: Update\n");

	// Check for a game state change.
	if (GameStateIsChanging())
	{
		// Shutdown the current game state.
		GameStateExecuteShutdown(gameState.current);

		// TODO: Add
		if (!GameStateManagerIsRestarting())
		{
			// Unload the current game state.
			GameStateExecuteUnload(gameState.current);

			// Shutdown game object manager
			GameObjectManagerShutdown();

			// Update the recorded states.
			gameState.previous = gameState.current;
			gameState.current = gameState.next;
		}

		if (!GameStateManagerIsRestarting())
		{
			// Load the new game state.
			GameStateExecuteLoad(gameState.current);
		}
		else
		{
			// Force the old state to be restarted.
			gameState.next = gameState.current;
		}

		// Initialize the new game state.
		GameStateExecuteInit(gameState.current);
	}

	// Update the current game state.
	GameStateExecuteUpdate(gameState.current, dt);
}

// Shutdown the game state manager.
void GameStateManagerShutdown(BetaObject* obj)
{
	UNREFERENCED_PARAMETER(obj);

	printf("GSM: Shutdown\n");

	// Shutdown the current game state.
	GameStateExecuteShutdown(gameState.current);

	// Unload the current game state.
	GameStateExecuteUnload(gameState.current);

	// Shutdown game object manager
	GameObjectManagerShutdown();
}

// Determine if the current game state is being restarted.
bool GameStateManagerIsRestarting()
{
	return gameState.next == GsRestart;
}

// Determine if the game is still running.
bool GameStateManagerIsRunning()
{
	return gameState.current != GsQuit;
}

// Set the next game state to run.
void GameStateManagerSetNextState(GameStates nextState)
{
	if (GameStateIsValid(nextState) || GameStateIsSpecial(nextState))
	{
		gameState.next = nextState;
	}
	else
	{
#ifdef _DEBUG
		/* In debug mode, print a message that an error has occurred. */
		printf("\nERROR: Invalid game state: %d\n", nextState);
#endif
	}
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

// Determine if a game state change has been requested.
static bool	GameStateIsChanging()
{
	return (gameState.current != gameState.next);
}
