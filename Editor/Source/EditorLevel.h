//------------------------------------------------------------------------------
//
// File Name:	EditorLevel.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework Map Editor
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Level.h"
#include <EventManager.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

namespace Beta
{
	// Levels
	class Space; class Tilemap; 
	typedef std::shared_ptr<Tilemap> TilemapPtr;

	// Graphics
	class Camera; 
	class SpriteSource; 
	typedef std::shared_ptr<SpriteSource> SpriteSourcePtr;
	
	// Windows
	class Input;

	// Core
	class GameObject;
	typedef std::shared_ptr<const GameObject> Archetype;
	namespace Editor { class CameraController; class TilePalette; }

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	struct MapLoad : public Event
	{
		MapLoad(TilemapPtr map);
		TilemapPtr const map;
	};

	struct PaletteChange : public Event
	{
		PaletteChange(SpriteSourcePtr source);
		SpriteSourcePtr source;
	};

	struct ArchetypeChange : public Event
	{
		ArchetypeChange(Archetype archetype);
		Archetype archetype;
	};

	namespace Editor
	{
		class EditorLevel : public Level
		{
		public:
			//------------------------------------------------------------------------------
			// Public Functions:
			//------------------------------------------------------------------------------

			// Creates an instance of the Map Editor.
			EditorLevel();

			// Loads resources associated with the Map Editor.
			void Load();

			// Initialize the memory associated with the Map Editor.
			void Initialize();

			// Update the Map Editor.
			// Params:
			//	 dt = Change in time (in seconds) since the last game loop.
			void Update(float dt);

			// Free memory associated with the Map Editor.
			void Shutdown();

			// Unloads resources associated with the map Editor.
			void Unload();

			// Global size scale for tiles
			static const float tileSize;

		private:
			//------------------------------------------------------------------------------
			// Private Structures:
			//------------------------------------------------------------------------------

			enum EditMode
			{
				EM_Tiles,
				EM_Objects
			};

			//------------------------------------------------------------------------------
			// Input:
			//------------------------------------------------------------------------------

			void UpdateNavigation() const;
			void UpdateEdits();
			void UpdateFiles();

			//------------------------------------------------------------------------------
			// Tiles:
			//------------------------------------------------------------------------------

			// Opens a sprite source to use as a palette
			void OpenPalette();
			void SetPalette(const std::string& paletteSourceName);

			//------------------------------------------------------------------------------
			// Objects:
			//------------------------------------------------------------------------------

			// Opens an object file to use for placement
			void OpenArchetype();
			void SetArchetype(const std::string& archetypeName);

			//------------------------------------------------------------------------------
			// Map:
			//------------------------------------------------------------------------------

			// Saves the currently loaded map
			void SaveMap();

			// Opens a map for editing
			void OpenMap();

			// Sets the map to a new map
			void SetMap(const std::string& mapName);

			//------------------------------------------------------------------------------
			// File Operations:
			//------------------------------------------------------------------------------

			// Saves a tilemap file using the Windows save file dialog
			std::string SaveFileDialog() const;

			// Opens a tilemap file using the Windows open file dialog
			// Params:
			//   instructions = The string to use for the title of the dialog box.
			std::string OpenFileDialog(const std::string& instructions) const;

			// Handles windows file errors
			void HandleFileError(DWORD errorCode) const;

			//------------------------------------------------------------------------------
			// Private Variables:
			//------------------------------------------------------------------------------

			// Tilemap, palette
			TilemapPtr mapResource;
			Archetype archetype;
			EditMode mode;

			// GUI
			Space* guiSpace;

			// Misc
			Input& input;
			EventManager* events;
		};
	}
}

//----------------------------------------------------------------------------
