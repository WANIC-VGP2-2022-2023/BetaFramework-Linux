//------------------------------------------------------------------------------
//
// File Name:	Test.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project7
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Level.h"

#include "SoundManager.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

namespace Beta
{
	class Mesh; class SpriteSource; class Texture; 
	class PostEffect; class Font;
	class GameObject;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	class Test : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Create an instance of the test level state.
		Test();

		// Load the resources associated with the MainMenu game state.
		void Load();

		// Initialize the memory associated with the MainMenu game state.
		void Initialize();

		// Update the test level game state.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt);

		// Shutdown any memory associated with the MainMenu game state.
		void Shutdown();

		// Unload the resources associated with the MainMenu game state.
		void Unload();

	private:
		//------------------------------------------------------------------------------
		// Private Function Declarations:
		//------------------------------------------------------------------------------

		GameObject* CreatePlayer(void);
		GameObject* CreateButton(void);
		GameObject* CreateBox(void);
		void MoveCamera(float dt);
		void MovePlayer(float dt);
		GameObject* CreateSpriteText();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		GameObject* player;
		SoundEventPtr bgmInstance;

		// HUD Space
		Space* hudSpace;

		// Fonts
		GameObject* spriteTextObject;
		TexturePtr textureFont;
		SpriteSourcePtr spriteSourceFont;
		FontPtr font;

		// Effects
		PostEffect* colorInverter;
		PostEffect* fadeToBlack;

		// Systems
		SoundManager* sound;
	};
}

//------------------------------------------------------------------------------
