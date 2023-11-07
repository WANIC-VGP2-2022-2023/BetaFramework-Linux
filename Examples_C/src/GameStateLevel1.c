//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

// Low Level
#include <BetaLow.h>

// High Level
#include "Animation.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameStateManager.h"
#include "GameStateLevel1.h"
#include "Mesh.h"
#include "Physics.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

typedef enum MonkeyStates
{
	MonkeyInvalid = -1,
	MonkeyIdle,
	MonkeyWalk,
	MonkeyJump,

} MonkeyStates;

static const float groundHeight = -1.50f;
static const float wallDistance = 3.5f;

static const float moveVelocity = 5.0f;
static const float jumpVelocity = 10.0f;

static const Vector2D gravityNormal = { 0.0f, -15.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

static const float CheckSquareDistance = (0.75f * 0.75f);

// SpriteText Demo Code:
//#define SPRITETEXT

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static int numLives1 = 0;

// Pointers to memory allocated during Load.
static Mesh	*pMesh1x1;
static Mesh	*pMesh3x3;
static Texture		*pTexturePlanet;
static Texture		*pTextureMonkeyIdle;
static Texture		*pTextureMonkeyWalk;
static Texture		*pTextureMonkeyJump;
static SpriteSourcePtr	spriteSourcePlanet = NULL;
static SpriteSourcePtr	spriteSourceMonkeyIdle = NULL;
static SpriteSourcePtr	spriteSourceMonkeyWalk = NULL;
static SpriteSourcePtr	spriteSourceMonkeyJump = NULL;

// Pointers to memory allocated during Init.
static GameObjectPtr gameObjectPlanet = NULL;
static GameObjectPtr gameObjectMonkey = NULL;
static GameObjectPtr gameObjectClone = NULL;	// @@@DAS - Demo Code
static SpriteSourcePtr spriteSource = NULL;

static enum MonkeyStates monkeyState;

// Used to temporary store camera position
static float camX, camY;

#ifdef SPRITETEXT
// SpriteText Demo Code:
static Mesh*	pMesh16x6;
static Texture *pTextureFontSheet;
static SpriteSourcePtr spriteSourceFont;
static GameObjectPtr gameObjectText = NULL;
static char livesBuffer[16] = "";
#endif

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void GameStateLevel1BounceController(GameObjectPtr gameObject);
static void GameStateLevel1MovementController(GameObjectPtr gameObject);

static void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, MonkeyStates newState);

static bool GameStateLevel1IsColliding(GameObjectPtr object1, GameObjectPtr object2);

static void GameStateLevel1MoveCamera(void);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Level 1 game state.
void GameStateLevel1Load()
{
	// Read the number of lives from a file.
	Stream stream = StreamOpen("Data/Level1_Lives.txt");
	if (stream)
	{
		numLives1 = StreamReadInt(stream);
		StreamClose(&stream);
	}

	// Create a mesh for displaying the planet texture.
	Vector2D textureSize = { 1.0f, 1.0f };
	Vector2D extents = { 0.5f, 0.5f };
	pMesh1x1 = MeshFactoryCreateQuadTextured(&textureSize, &extents);

	// Create a mesh for displaying the monkey texture.
	textureSize.x = 1.0f / 3; textureSize.y = 1.0f / 3;
	pMesh3x3 = MeshFactoryCreateQuadTextured(&textureSize, &extents);

	// Load the planet texture.
	pTexturePlanet = TextureCreateFromFile("PlanetTexture.png", TM_Nearest);
	if(pTexturePlanet == NULL) 
		printf("Failed to create Planet texture!\n");

	// Create a sprite source for the planet texture.
	spriteSourcePlanet = SpriteSourceCreate(1, 1, pTexturePlanet);

	// Load the monkey textures.
	pTextureMonkeyIdle = TextureCreateFromFile("MonkeyIdle.png", TM_Nearest);
	if (pTexturePlanet == NULL)
		printf("Failed to create MonkeyIdle texture!");

	pTextureMonkeyWalk = TextureCreateFromFile("MonkeyWalk.png", TM_Nearest);
	if (pTexturePlanet == NULL)
		printf("Failed to create MonkeyWalk texture!");

	pTextureMonkeyJump = TextureCreateFromFile("MonkeyJump.png", TM_Nearest);
	if (pTexturePlanet == NULL)
		printf("Failed to create MonkeyJump texture!");

	// Create sprite sources for the monkey textures.
	spriteSourceMonkeyIdle = SpriteSourceCreate(1, 1, pTextureMonkeyIdle);
	spriteSourceMonkeyWalk = SpriteSourceCreate(3, 3, pTextureMonkeyWalk);
	spriteSourceMonkeyJump = SpriteSourceCreate(1, 1, pTextureMonkeyJump);

#ifdef SPRITETEXT
	// SpriteText Demo Code:
	pMesh16x6 = MeshCreateQuad(0.5, 0.5, 1.0f / 16, 1.0f / 6, "Mesh16x6");
	pTextureFontSheet = TextureLoad("Assets/FontSheet.png");
	spriteSourceFont = SpriteSourceCreate(16, 6, pTextureFontSheet);
	if(spriteSourceFont == NULL)
		printf("Failed to create Hexidecimal texture!");
#endif
}

// Initialize the memory associated with the Level 1 game state.
void GameStateLevel1Init()
{
	// Create a "Planet" game object.
	gameObjectPlanet = GameObjectFactoryBuild("PlanetBounce");
	if (gameObjectPlanet)
	{
		SpritePtr sprite = GameObjectGetSprite(gameObjectPlanet);
		SpriteSetSpriteSource(sprite, spriteSourcePlanet);
		SpriteSetMesh(sprite, pMesh1x1);
	}

	// Create a Monkey game object.
	gameObjectMonkey = GameObjectFactoryBuild("Monkey");
	if (gameObjectMonkey)
	{
		monkeyState = MonkeyInvalid;
		GameStateLevel1SetMonkeyState(gameObjectMonkey, MonkeyIdle);
	}

#ifdef SPRITETEXT
	// Create a Lives Text game object.
	gameObjectText = GameObjectFactoryBuild("MonkeyLivesText");
	if (gameObjectText)
	{
		SpritePtr sprite = GameObjectGetSprite(gameObjectText);
		SpriteSetSpriteSource(sprite, spriteSourceFont);
		SpriteSetMesh(sprite, pMesh16x6);

		sprintf_s(livesBuffer, _countof(livesBuffer), "Lives: %d", numLives1);
		SpriteSetText(sprite, livesBuffer);
	}
#endif

	Color bgColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	GraphicsSetBackgroundColor(&bgColor);
}

// Update the Level 1 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{
	// Update the object's movement based upon user input.
	GameStateLevel1MovementController(gameObjectMonkey);

	// Update the movement of the bouncing planet.
	GameStateLevel1BounceController(gameObjectPlanet);

#ifdef DEMO_MODE
	// Move the camera
	GameStateLevel1MoveCamera();
#endif

	// Update and draw all game objects.
	GameObjectUpdate(gameObjectPlanet, dt);
	GameObjectUpdate(gameObjectMonkey, dt);
	//GameObjectUpdate(gameObjectText, dt);
	GameObjectDraw(gameObjectPlanet);
	GameObjectDraw(gameObjectMonkey);
	//GameObjectDraw(gameObjectText);
	GameObjectUpdate(gameObjectClone, dt);		// @@@DAS - Demo Code

	// Check for "collision" between the Planet and Monkey game objects.
	if (GameStateLevel1IsColliding(gameObjectPlanet, gameObjectMonkey))
	{
		//if (--numLives1 <= 0)
		//{
			//GameStateManagerSetNextState(GsLevel2);
		//}
		//else
		//{
			GameStateManagerSetNextState(GsRestart);
		//}
	}

	// Check for game state change requests.
	if (InputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsRestart);
	}
	/*else if (AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}
	else if (AEInputCheckTriggered('3'))
	{
		GameStateManagerSetNextState(GsAsteroids);
	}
	else if (AEInputCheckTriggered('4'))
	{
		GameStateManagerSetNextState(GsOmega);
	}*/
	else if (InputCheckTriggered(' '))	// @@@DAS - Demo Code
	{
		GameObjectFree(&gameObjectClone);
		gameObjectClone = GameObjectClone(gameObjectMonkey);
		int randNum = rand();
		float randPosX = (((float)randNum) / (RAND_MAX + 1)) * 800 - 400;
		TransformPtr transform = GameObjectGetTransform(gameObjectClone);
		Vector2D translation = *TransformGetTranslation(transform);
		translation.x = randPosX;
		TransformSetTranslation(transform, &translation);
	}

	GameObjectDraw(gameObjectMonkey);
	GameObjectDraw(gameObjectClone);		// @@@DAS - Demo Code
}

// Shutdown any memory associated with the Level 1 game state.
void GameStateLevel1Shutdown()
{
	// Free all objects and components used for graphics.
	GameObjectFree(&gameObjectPlanet);
	GameObjectFree(&gameObjectMonkey);
	//GameObjectFree(&gameObjectText);

	// @@@DAS - Demo Code
	GameObjectFree(&gameObjectClone);
}

// Unload the resources associated with the Level 1 game state.
void GameStateLevel1Unload()
{
	// Free all sprite sources.
	SpriteSourceFree(&spriteSourcePlanet);
	SpriteSourceFree(&spriteSourceMonkeyIdle);
	SpriteSourceFree(&spriteSourceMonkeyWalk);
	SpriteSourceFree(&spriteSourceMonkeyJump);

	// Freeing the meshes and textures
	MeshDestroy(&pMesh1x1);
	MeshDestroy(&pMesh3x3);
	//TextureUnload(pTexturePlanet);
	//TextureUnload(pTextureMonkeyIdle);
	//TextureUnload(pTextureMonkeyWalk);
	//TextureUnload(pTextureMonkeyJump);

#ifdef SPRITETEXT
	// SpriteText Demo Code:
	SpriteSourceFree(&spriteSourceFont);
	AEGfxMeshFree(pMesh16x6);
	//TextureUnload(pTextureFontSheet);
#endif
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void GameStateLevel1BounceController(GameObjectPtr gameObject)
{
	PhysicsPtr physics = GameObjectGetPhysics(gameObject);
	TransformPtr transform = GameObjectGetTransform(gameObject);
	if (physics && transform)
	{
		// Get the Object's current position.
		Vector2D position = *TransformGetTranslation(transform);
		// Get the Object's current velocity.
		Vector2D velocity = *PhysicsGetVelocity(physics);

		if (position.x <= -wallDistance)
		{
			position.x = -wallDistance;
			velocity.x = -velocity.x;
		}
		else if (position.x >= wallDistance)
		{
			position.x = wallDistance;
			velocity.x = -velocity.x;
		}

		if ((position.y <= groundHeight))
		{
			position.y = groundHeight + (groundHeight - position.y);
			velocity.y = -velocity.y;
		}

		// Set the object's new position and velocity.
		TransformSetTranslation(transform, &position);
		PhysicsSetVelocity(physics, &velocity);
	}
}

static void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, MonkeyStates newState)
{
	if (monkeyState != newState)
	{
		monkeyState = newState;
		SpritePtr sprite = GameObjectGetSprite(gameObject);
		AnimationPtr animation = GameObjectGetAnimation(gameObject);
		switch (newState)
		{
		case MonkeyIdle:
			SpriteSetMesh(sprite, pMesh1x1);
			SpriteSetSpriteSource(sprite, spriteSourceMonkeyIdle);
			AnimationPlay(animation, 1, 0.0f, false);
			break;

		case MonkeyWalk:
			SpriteSetMesh(sprite, pMesh3x3);
			SpriteSetSpriteSource(sprite, spriteSourceMonkeyWalk);
			AnimationPlay(animation, 8, 0.1f, true);
			break;

		case MonkeyJump:
			SpriteSetMesh(sprite, pMesh1x1);
			SpriteSetSpriteSource(sprite, spriteSourceMonkeyJump);
			AnimationPlay(animation, 1, 0.0f, false);
			break;

		default:
			break;
		}
	}
}

static void GameStateLevel1MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr physics = GameObjectGetPhysics(gameObject);
	TransformPtr transform = GameObjectGetTransform(gameObject);
	if (physics && transform)
	{
		// Get the Object's current velocity.
		Vector2D velocity = *PhysicsGetVelocity(physics);

		// Check for lateral movement controls.
		if (InputCheckHeld(VK_LEFT))
		{
			velocity.x = -moveVelocity;
			if (monkeyState != MonkeyJump)
			{
				GameStateLevel1SetMonkeyState(gameObject, MonkeyWalk);
			}
#ifdef DEMO_MODE
			Vector2D scale = *TransformGetScale(transform);
			scale.x = -fabsf(scale.x);
			TransformSetScale(transform, &scale);
#endif
		}
		else if (InputCheckHeld(VK_RIGHT))
		{
			velocity.x = moveVelocity;
			if (monkeyState != MonkeyJump)
			{
				GameStateLevel1SetMonkeyState(gameObject, MonkeyWalk);
			}
#ifdef DEMO_MODE
			Vector2D scale = *TransformGetScale(transform);
			scale.x = fabsf(scale.x);
			TransformSetScale(transform, &scale);
#endif
		}
		else
		{
			velocity.x = 0;
			if (monkeyState != MonkeyJump)
			{
				GameStateLevel1SetMonkeyState(gameObject, MonkeyIdle);
			}
		}

		// Check for jump movement controls.
		if (InputCheckTriggered(VK_UP))
		{
			velocity.y = jumpVelocity;
			PhysicsSetAcceleration(physics, &gravityNormal);
			GameStateLevel1SetMonkeyState(gameObject, MonkeyJump);
		}

		// Check for landing.
		Vector2D position = *TransformGetTranslation(transform);
		if (position.y < groundHeight)
		{
			// Set the object's Y position equal to the ground height.
			position.y = groundHeight;
			TransformSetTranslation(transform, &position);

			// Set the object's Y velocity to zero.
			velocity.y = 0;
			PhysicsSetAcceleration(physics, &gravityNone);

			GameStateLevel1SetMonkeyState(gameObject, MonkeyIdle);
		}

		// Set the object's new velocity.
		PhysicsSetVelocity(physics, &velocity);
	}
}

static bool GameStateLevel1IsColliding(GameObjectPtr object1, GameObjectPtr object2)
{
	TransformPtr transform1 = GameObjectGetTransform(object1);
	TransformPtr transform2 = GameObjectGetTransform(object2);
	const Vector2D * translation1 = TransformGetTranslation(transform1);
	const Vector2D * translation2 = TransformGetTranslation(transform2);
	return (Vector2DDistanceSquared(translation1, translation2) < CheckSquareDistance);
}

#ifdef DEMO_MODE
void GameStateLevel1MoveCamera(void)
{
	// Move the camera
	AEGfxGetCamPosition(&camX, &camY);
	if (AEInputCheckCurr('W'))
		AEGfxSetCamPosition(camX, camY + 2);
	else if (AEInputCheckCurr('S'))
		AEGfxSetCamPosition(camX, camY - 2);

	AEGfxGetCamPosition(&camX, &camY);
	if (AEInputCheckCurr('A'))
		AEGfxSetCamPosition(camX - 2, camY);
	else if (AEInputCheckCurr('D'))
		AEGfxSetCamPosition(camX + 2, camY);
}
#endif
