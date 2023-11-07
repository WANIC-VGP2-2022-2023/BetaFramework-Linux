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

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EditorLevel.h"

// Windows
#include <commdlg.h>
#include <cderr.h>

// Resources
#include <Tilemap.h>
#include <SpriteSource.h>

// Systems
#include <Space.h>
#include <Input.h>
#include <FileStream.h>
#include <EngineCore.h>		// EngineGetModule
#include <SpaceManager.h>
#include <GraphicsEngine.h>
#include <GameObjectFactory.h>
#include <WindowSystem.h>	// SetWindowTitle

// Components
#include <Area.h>
#include <SpriteTilemap.h>
#include <Reactive.h>
#include "CameraController.h"
#include "TilePalette.h"
#include "TilePlacer.h"
#include "ObjectPlacer.h"
#include "GridDraw.h"

//------------------------------------------------------------------------------

namespace Beta
{
	const std::string BE_EDITOR_VERSION = "0.7.2";

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	MapLoad::MapLoad(TilemapPtr const map)
		: Event("MapLoad"), map(map) {}

	PaletteChange::PaletteChange(SpriteSourcePtr source)
		: Event("PaletteChange"), source(source) {}

	ArchetypeChange::ArchetypeChange(Archetype archetype)
		: Event("ArchetypeChange"), archetype(archetype) {}

	namespace Editor
	{
		const float EditorLevel::tileSize = 1.0f;

		namespace { const size_t initialMapSize = 5; }

		// Creates an instance of the Map Editor.
		EditorLevel::EditorLevel()
			: Level("EditorLevel"), input(*EngineGetModule(Input)), mapResource(nullptr),
			archetype(nullptr), mode(EM_Tiles)
		{
		}

		// Loads resources associated with the Map Editor.
		void EditorLevel::Load()
		{
			// Enable perspective
			Camera& defaultCam = GetSpace()->GetCamera();
			defaultCam.SetProjectionMode(PM_Perspective);
			defaultCam.SetFOV(120.0f);

			// Create GUI space
			EngineCore& engine = EngineCore::GetInstance();
			guiSpace = EngineGetModule(SpaceManager)->AddSpace("GuiSpace", true);

			// Register for events
			events = engine.GetModule<EventManager>();

			// Disable quadtree
			GetSpace()->GetObjectManager().SetQuadtreeEnabled(false);

			// Set window title
			EngineGetModule(WindowSystem)->SetWindowTitle("BetaEngine Level Editor v" + BE_EDITOR_VERSION);
		}

		// Initialize the memory associated with the Map Editor.
		void EditorLevel::Initialize()
		{
			// Create a temporary map
			mapResource = TilemapPtr(new Tilemap(initialMapSize, initialMapSize, "temp"));

			// Create tile palette
			GameObject* paletteObject = new GameObject("TilePalette");
			paletteObject->AddComponent(new Transform());
			paletteObject->AddComponent(new Sprite());
			paletteObject->AddComponent(new TilePalette());
			paletteObject->AddComponent(new Reactive());
			guiSpace->GetObjectManager().AddObject(*paletteObject);

			// Create map object
			GameObject* mapObject = new GameObject("Map");
			mapObject->AddComponent(new Area(Vector2D(), Vector2D(tileSize, tileSize)));
			mapObject->AddComponent(new SpriteTilemap());
			mapObject->AddComponent(new TilePlacer());
			mapObject->AddComponent(new ObjectPlacer());
			mapObject->AddComponent(new GridDraw());
			mapObject->AddComponent(new Reactive());
			GetSpace()->GetObjectManager().AddObject(*mapObject);

			// Create camera object
			GameObject* editorCamera = new GameObject("EditorCamera");
			editorCamera->AddComponent(new Transform());
			editorCamera->AddComponent(new CameraController());
			GetSpace()->GetObjectManager().AddObject(*editorCamera);

			// Set the palette
			SetPalette("TilemapPalette");

			// Load map event
			events->SendEvent(new MapLoad(mapResource), GetSpace());

			// Center the camera on the map
			events->SendEvent(new Event("CameraReset"), GetSpace());
		}

		// Update the Map Editor.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void EditorLevel::Update(float dt)
		{
			UNREFERENCED_PARAMETER(dt);

			// Check for inputs, respond accordingly
			// Ordered based on danger level, least to greatest
			UpdateNavigation();
			UpdateEdits();
			UpdateFiles();
		}

		void EditorLevel::Shutdown()
		{
			// If this map is temporary (i.e., not previously loaded from a file)
			if (Tilemap::GetTilemapManager().GetResource(mapResource->GetName(), false) == nullptr)
			{
				// Ask the user if they want to save
				SaveMap();
			}
		}

		// Unloads resources associated with the map Editor.
		void EditorLevel::Unload()
		{
			// Delete space
			guiSpace->Destroy();
		}

		//------------------------------------------------------------------------------
		// Map Editing:
		//------------------------------------------------------------------------------

		void EditorLevel::UpdateNavigation() const
		{
			if (input.CheckTriggered('R'))
				events->SendEvent(new Event("CameraReset"), GetSpace());
		}

		void EditorLevel::UpdateEdits()
		{
			// Ignore input if CTRL is held
			if (input.CheckHeld(VK_CONTROL))
				return;

			// Determine mode
			if (input.CheckTriggered('1'))
			{
				mode = EM_Tiles;
			}
			else if (input.CheckTriggered('2'))
			{
				mode = EM_Objects;
				if (archetype == nullptr)
					OpenArchetype();
			}
			else
			{
				// Place object
				std::string eventName = "";
				if (input.CheckHeld(VK_LBUTTON))
				{
					if (mode == EM_Tiles) eventName = "TileSet";
					else eventName = "ObjectCreate";
				}
				// Remove object
				else if (input.CheckHeld(VK_RBUTTON))
				{
					if (mode == EM_Tiles) eventName = "TileErase";
					else eventName = "ObjectDestroy";
				}

				// No action necessary
				if (eventName == "")
					return;

				// Send the event
				events->SendEvent(new Event(eventName), GetSpace(), 0.1f);
			}
		}

		void EditorLevel::UpdateFiles()
		{
			if (input.CheckHeld(VK_CONTROL))
			{
				if (input.CheckTriggered('S'))
					SaveMap();
				else if (input.CheckTriggered('O'))
					OpenMap();
				else if (input.CheckTriggered('P'))
					OpenPalette();
				else if (input.CheckTriggered('N'))
					GetSpace()->RestartLevel();
			}
			else
			{
				if (input.CheckTriggered('J'))
					OpenArchetype();
			}
		}

		//------------------------------------------------------------------------------
		// Tiles:
		//------------------------------------------------------------------------------

		void EditorLevel::OpenPalette()
		{
			std::string paletteName = OpenFileDialog("Select SpriteSource");
			if (paletteName == "")
				return;

			SetPalette(paletteName);
		}

		void EditorLevel::SetPalette(const std::string& paletteSourceName)
		{
			// Set the sprite source on the map and palette objects
			events->SendEvent(new PaletteChange(ResourceGetSpriteSource(paletteSourceName)), GetSpace());
		}

		//------------------------------------------------------------------------------
		// Objects:
		//------------------------------------------------------------------------------

		void EditorLevel::OpenArchetype()
		{
			std::string archetypeName = OpenFileDialog("Select Object");
			if (archetypeName == "")
				return;

			SetArchetype(archetypeName);
			mode = EM_Objects;
		}

		void EditorLevel::SetArchetype(const std::string& archetypeName)
		{
			// Get the archetype, create it if not found
			archetype = ResourceGetArchetype(archetypeName);

			// Send event
			events->SendEvent(new ArchetypeChange(archetype), GetSpace());
		}

		//------------------------------------------------------------------------------
		// Map:
		//------------------------------------------------------------------------------

		void EditorLevel::SaveMap()
		{
			std::string filename = mapResource->GetName();

			// File does not yet exist
			if (mapResource->GetName() == "temp")
			{
				// Get filename from dialog box
				filename = SaveFileDialog();

				// If user cancelled, return.
				if (filename == "")
					return;

				mapResource->SetName(filename);
			}

			// Prevent edits for 1 second, shrink/relocate map object
			events->SendEvent(new Event("MapSave"), GetSpace());

			// Serialize
			try
			{
				FileStream p(Tilemap::GetTilemapManager().GetFilePath()
					+ filename + Tilemap::GetTilemapManager().GetFileExtension(), OM_Write);
				mapResource->Serialize(p);
				std::cout << "Map saved to " << filename << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}

		void EditorLevel::OpenMap()
		{
			std::string mapName = OpenFileDialog("Open Map");
			if (mapName == "")
				return;

			SetMap(mapName);
		}

		// Sets the map to a new map
		void EditorLevel::SetMap(const std::string& mapName)
		{
			TilemapPtr oldMap = mapResource;

			// Retrieve the map
			mapResource = ResourceGetTilemap(mapName);

			// Don't do anything else if file fails to load
			if (mapResource == nullptr)
				return;

			// Prevent edits for 2 seconds
			events->SendEvent(new MapLoad(mapResource), GetSpace());

			// Center the camera on the map
			events->SendEvent(new Event("CameraReset"), GetSpace());

			// If the old map was temporary (i.e., not previously loaded from a file), get rid of it
			if (Tilemap::GetTilemapManager().GetResource(oldMap->GetName(), false) == nullptr)
				oldMap.reset();
		}

		//------------------------------------------------------------------------------
		// File Operations:
		//------------------------------------------------------------------------------

		std::string EditorLevel::SaveFileDialog() const
		{
			char filename[MAX_PATH];

			OPENFILENAMEA ofn;
			memset(&filename, 0, sizeof(filename));
			memset(&ofn, 0, sizeof(ofn));

			// Open file settings
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = nullptr;
			ofn.lpstrFilter = "Text Files\0*.txt\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Save Map";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

			if (GetSaveFileNameA(&ofn))
			{
				std::string shortPath(filename);
				// Remove full path
				shortPath = shortPath.substr(shortPath.find_last_of('\\') + 1);
				// Remove extension
				shortPath = shortPath.substr(0, shortPath.find_first_of('.'));
				std::cout << "Creating file " << shortPath << ".txt" << std::endl;
				return shortPath;
			}
			else
			{
				HandleFileError(CommDlgExtendedError());
				return std::string();
			}
		}

		// Opens a tilemap file using the Windows open file dialog
		// Params:
		//   instructions = The string to use for the title of the dialog box.
		std::string EditorLevel::OpenFileDialog(const std::string& instructions) const
		{
			char filename[MAX_PATH];

			OPENFILENAMEA ofn;
			memset(&filename, 0, sizeof(filename));
			memset(&ofn, 0, sizeof(ofn));

			// Open file settings
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = nullptr;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "Text Files\0*.txt\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = instructions.c_str();
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileNameA(&ofn))
			{
				std::string shortPath(filename);
				// Remove full path
				shortPath = shortPath.substr(shortPath.find_last_of('\\') + 1);
				// Remove extension
				shortPath = shortPath.substr(0, shortPath.find_first_of('.'));
				return shortPath;
			}
			else
			{
				HandleFileError(CommDlgExtendedError());
				return std::string();
			}
		}

		void EditorLevel::HandleFileError(DWORD errorCode) const
		{
			std::cout << "Failed to open selected file: ";
			switch (errorCode)
			{
			case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
			case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
			case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
			case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
			case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
			case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
			case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
			case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
			case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
			case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
			case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
			case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
			case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
			case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
			case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
			default: std::cout << "User cancelled.\n";
			}
		}
	}
}

//----------------------------------------------------------------------------
