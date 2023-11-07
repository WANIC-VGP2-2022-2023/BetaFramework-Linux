//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 1
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "GameStateTable.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

typedef struct BetaObject BetaObject;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game state manager.
void GameStateManagerInit(BetaObject*);

// Update the game state manager.
void GameStateManagerUpdate(BetaObject*, float dt);

// Shutdown the game state manager.
void GameStateManagerShutdown(BetaObject*);

// Determine if the current game state is being restarted.
bool GameStateManagerIsRestarting(void);

// Determine if the game is still running.
bool GameStateManagerIsRunning(void);

// Set the next game state to run.
void GameStateManagerSetNextState(GameStates nextState);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

