//------------------------------------------------------------------------------
//
// File Name:	Test.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Test.h"

// Resources
#include "FadeToBlack.h"
#include "ColorInverter.h"

// Components
#include "Button.h"
#include "ColorChange.h"

// Levels
#include "ChangeLevel.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create an instance of the test level state.
	Test::Test()
		: Level("Test Level"), player(nullptr),
		bgmInstance(nullptr), hudSpace(nullptr)
	{
	}

	// Load the resources associated with the MainMenu game state.
	void Test::Load()
	{
		GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);

		// Set Beta Framework’s background color to black.
		graphics.SetBackgroundColor(Colors::Black);

		// Enable perspective for default camera
		Camera& gameCamera = GetSpace()->GetCamera();
		gameCamera.SetProjectionMode(PM_Perspective);
		gameCamera.SetFOV(120.0f);

		// Shaders
		fadeToBlack = nullptr;
		colorInverter = graphics.PushEffect<ColorInverter>();

		// Sounds
		EngineCore& engine = EngineCore::GetInstance();
		sound = engine.GetModule<SoundManager>();
		sound->AddEffect("teleport.wav");
		sound->AddBank("Master Bank.strings.bank");
		sound->AddBank("Master Bank.bank");

		// Textures
		textureFont = TexturePtr(Texture::CreateTextureFromFile("FontSheet.png"));
		spriteSourceFont = SpriteSourcePtr(new SpriteSource(textureFont, "FontSheet", 16, 6));

		// Create HUD Space
		hudSpace = EngineGetModule(SpaceManager)->AddSpace("HudSpace", true);
		Camera& hudCamera = hudSpace->GetCamera();
		hudCamera.SetProjectionMode(PM_Orthographic);
		hudCamera.SetVerticalSize(12);

		// Load a font
		font = ResourceGetFont("Arial.ttf 40");
	}

	// Initialize the memory associated with the MainMenu game state.
	void Test::Initialize()
	{
		// Background
		GetSpace()->GetObjectManager().AddObject(
			*EngineGetModule(GameObjectFactory)->CreateObject("Background", GetSpace()));

		// Make button and box
		hudSpace->GetObjectManager().AddObject(*CreateButton());

		// Make player and keep pointer
		player = CreatePlayer();

		// Start the event (plays sounds as appropriate given the default parameters settings)
		bgmInstance = sound->PlayEvent("Level Music");
		bgmInstance->Start();

		// Create sprite text
		spriteTextObject = CreateSpriteText();
	}

	// Update the MainMenu game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Test::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		Input& input = *EngineGetModule(Input);
		GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);

		// Move the camera based on user input
		MoveCamera(dt);
		// Move the player based on user input
		MovePlayer(dt);

		// If the user presses the 'F' key, fade out.
		if (input.CheckTriggered('F'))
		{
			if (!fadeToBlack->IsActive())
			{
				fadeToBlack = graphics.PushEffect<FadeToBlack>();
			}
			else
			{
				graphics.RemoveEffect(fadeToBlack);
				fadeToBlack = nullptr;
			}
		}

		// If the user presses the 'C' key, toggle color inversion.
		if (input.CheckTriggered('C'))
		{
			if (!colorInverter->IsActive())
			{
				colorInverter = graphics.PushEffect<ColorInverter>();
			}
			else
			{
				graphics.RemoveEffect(colorInverter);
				colorInverter = nullptr;
			}
		}

		// If the user presses plus and minus, increase and decrease volume
		if (input.CheckTriggered('+'))
			sound->SetEffectsVolume(sound->GetEffectsVolume() + 0.05f);
		else
			sound->SetEffectsVolume(sound->GetEffectsVolume() - 0.05f);

		// Is the fade effect active?
		if (fadeToBlack && fadeToBlack->IsActive())
		{
			fadeToBlack->Update(dt);
		}

		// Is the color effect active?
		if (colorInverter && colorInverter->IsActive())
		{
			colorInverter->Update(dt);
		}

		// Toggle between bitmap and font
		static bool fontMode = false;
		if (input.CheckTriggered('B'))
		{
			if (fontMode == true)
				spriteTextObject->GetComponent<SpriteText>()->SetFont(nullptr);
			else
				spriteTextObject->GetComponent<SpriteText>()->SetFont(font);

			fontMode = !fontMode;
		}

		// Check for level change keys
		ChangeLevel(this);
	}

	// Shutdown any memory associated with the MainMenu game state.
	void Test::Shutdown()
	{
		bgmInstance->Stop(true);
	}

	// Unload the resources associated with the MainMenu game state.
	void Test::Unload()
	{
		sound->Shutdown();
		hudSpace->Destroy();

		EngineGetModule(GraphicsEngine)->ClearEffects();
		delete colorInverter;
		delete fadeToBlack;

		// Set camera size to default
		Camera& gameCamera = GetSpace()->GetCamera();
		gameCamera.SetProjectionMode(PM_Orthographic);
		gameCamera.Reset();
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Move the camera based on user input
	void Test::MoveCamera(float dt)
	{
		float moveSpeed = 2.0f;
		float zoomSpeed = 10.0f;
		Camera& spaceCamera = GetSpace()->GetCamera();
		Vector2D cameraPos = spaceCamera.GetTranslation();
		float cameraZoom = 0.0f;
		ProjectionMode mode = spaceCamera.GetProjectionMode();
		if (mode == PM_Perspective)
			cameraZoom = spaceCamera.GetFOV();
		else
			cameraZoom = spaceCamera.GetVerticalSize();

		// Move camera
		Input & input = *EngineGetModule(Input);
		if (input.CheckHeld(VK_UP))
			cameraPos.y += moveSpeed * dt;

		if (input.CheckHeld(VK_DOWN))
			cameraPos.y -= moveSpeed * dt;

		if (input.CheckHeld(VK_RIGHT))
			cameraPos.x += moveSpeed * dt;

		if (input.CheckHeld(VK_LEFT))
			cameraPos.x -= moveSpeed * dt;

		// Zoom out
		if (input.CheckHeld('Q'))
			cameraZoom += zoomSpeed * dt;

		// Zoom in
		if (input.CheckHeld('E'))
			cameraZoom -= zoomSpeed * dt;

		spaceCamera.SetTranslation(cameraPos);

		if (mode == PM_Perspective)
			spaceCamera.SetFOV(cameraZoom);
		else
			spaceCamera.SetVerticalSize(cameraZoom);
	}

	// Move the camera based on user input
	void Test::MovePlayer(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (player == nullptr) return;

		Input & input = *EngineGetModule(Input);
		if (input.CheckTriggered('W') || input.CheckTriggered('S') || input.CheckTriggered('D') || input.CheckTriggered('A'))
		{
			float moveSpeed = 2.5f;
			Vector2D dir;

			// Move player
			if (input.CheckTriggered('W'))
				dir.y = 1.0f;

			if (input.CheckTriggered('S'))
				dir.y = -1.0f;

			if (input.CheckTriggered('D'))
				dir.x = 1.0f;

			if (input.CheckTriggered('A'))
				dir.x = -1.0f;

			RigidBody* physics = player->GetComponent<RigidBody>();
			physics->SetVelocity(dir.Normalized() * moveSpeed);
		}
	}

	GameObject* Test::CreateSpriteText()
	{
		// Create a game object with the following parameter.
		GameObject* textObject = new GameObject("Text");

		// Transform
		Transform* transform = new Area(0.0f, 0.0f);
		transform->SetScale(Vector2D(1.0f, 1.0f));
		textObject->AddComponent(transform);

		// Sprite
		SpriteText* sprite = new SpriteText("Testing testing testing", font, true, 6);
		sprite->SetColor(Colors::White);
		sprite->SetSpriteSource(spriteSourceFont);

		//sprite->SetZDepth(1.0f);
		textObject->AddComponent(sprite);

		// Add textObject to object manager
		GetSpace()->GetObjectManager().AddObject(*textObject);
		//GetSpace()->GetResourceManager().SaveSpriteSourceToFile(spriteSourceFont);

		return textObject;
	}

	// Create the player object.
	//	Returns:
	//		Pointer to the newly created player object.
	GameObject* Test::CreatePlayer(void)
	{
		// Create a game object with the following parameter :
		// "player"
		GameObject* playerObject = new GameObject("Player");

		// Transform
		Transform* transform = new Transform(0.0f, 0.0f);
		transform->SetScale(Vector2D(0.6f, 0.6f));
		playerObject->AddComponent(transform);

		// Sprite
		Sprite* sprite = new Sprite();
		sprite->SetColor({ 0.2f, 0.5f, 0.0f, 1.0f });
		sprite->SetSpriteSource(ResourceGetSpriteSource("Asteroid"));
		playerObject->AddComponent(sprite);

		// RigidBody
		RigidBody* physics = new RigidBody();
		playerObject->AddComponent(physics);

		// Collider
		Collider* collider = new ColliderCircle(transform->GetScale().x / 2.0f);
		playerObject->AddComponent(collider);

		// Add playerObject to object manager
		GetSpace()->GetObjectManager().AddObject(*playerObject);

		return playerObject;
	}

	// Create a button object.
	GameObject * Test::CreateButton(void)
	{
		// Create a game object with the following parameter :
		// "Button"
		GameObject* button = new GameObject("Button");
		// Create a transform object with the following parameters :
		// Translation : 0, 0
		Transform* transform = new Transform(-6.0f, 4.0f);
		// Scale : 100, 50
		transform->SetScale(Vector2D(2.0f, 1.0f));
		// Create a sprite object with the following parameter :
		// "button Sprite"
		Sprite* sprite = new Sprite();
		// Set sprite’s mesh.
		sprite->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
		sprite->SetZDepth(0.0f);

		// Set the game object’s transform, sprite, and reactive 
		// components using the created objects.
		button->AddComponent(transform);
		button->AddComponent(sprite);
		button->AddComponent(new Reactive());
		button->AddComponent(new Button());

		// Add button to object manager
		return button;
	}

	// Create a box object.
	GameObject* Test::CreateBox(void)
	{
		GameObject* box = new GameObject("Box");

		Vector2D position(2.0f, -2.0f);
		Vector2D scale(1.0f, 1.0f);

		Transform * transform = new Transform(position.x, position.y);
		transform->SetScale(scale);
		box->AddComponent(transform);

		Sprite * sprite = new Sprite();
		sprite->SetColor(Colors::White);
		box->AddComponent(sprite);

		Collider * collider = new ColliderRectangle(transform->GetScale() / 2.0f);
		box->AddComponent(collider);

		box->AddComponent(new ColorChange());

		return box;
	}
}
