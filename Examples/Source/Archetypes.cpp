//------------------------------------------------------------------------------
//
// File Name:	Archetypes.cpp
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
#include "Archetypes.h"

// Systems
#include <EngineCore.h>			// EngineGetModule
#include <Space.h>				// GetObjectManager
#include <GameObjectManager.h>	// AddArchetype
#include <GameObjectFactory.h>	// SaveObjectToFile
#include <GameObject.h>

// Resources
#include <SpriteSource.h>
#include <Animation.h>

// Standard components
#include <Transform.h>
#include <Sprite.h>
#include <RigidBody.h>
#include <ColliderCircle.h>
#include <ColliderRectangle.h>
#include <ColliderTilemap.h>
#include <SpriteTilemap.h>
#include <Animator.h>
#include <ColliderTilemap.h>

// Custom components
#include "PlayerShip.h"
#include "TimedDeath.h"
#include "Asteroid.h"
#include "ScreenWrap.h"
#include "PlayerProjectile.h"
#include "RaycastTest.h"
#include "MonkeyMovement.h"
#include "MonkeyAnimation.h"
#include "Enemy.h"
#include "CameraFollow.h"

//------------------------------------------------------------------------------

namespace Beta
{
	namespace Archetypes
	{
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Create a game object that uses the Ship sprite source.
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateShip(void)
		{
			GameObject* ship = new GameObject("PlayerShip");

			// Add components: Transform, Sprite, RigidBody, PlayerShip
			Transform* transform = new Transform(0.0f, 0.0f);
			transform->SetScale(Vector2D(1.0f, 1.0f));
			ship->AddComponent(transform);

			Sprite* sprite = new Sprite();
			SpriteSourcePtr spriteSource = ResourceGetSpriteSource("Ship");
			sprite->SetSpriteSource(spriteSource);
			sprite->SetColor(Colors::Blue);
			ship->AddComponent(sprite);

			ship->AddComponent(new RigidBody());
			ship->AddComponent(new PlayerShip());
			ship->AddComponent(new ScreenWrap());
			ship->AddComponent(new ColliderCircle(transform->GetScale().x / 2.0f));
			ship->AddComponent(new RaycastTest(2.0f));

			//EngineGetModule(GameObjectFactory)->SaveObjectToFile(ship);
			//space->GetResourceManager().AddSpriteSource(spriteSource);
			//space->GetResourceManager().SaveSpriteSourceToFile(spriteSource);

			return ship;
		}

		// Create a game object archetype that uses the Bullet sprite source.
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateBulletArchetype(void)
		{
			GameObject* bullet = new GameObject("Bullet");

			Transform* transform = new Transform(0.0f, 0.0f);
			transform->SetScale(Vector2D(0.14f, 0.14f));
			bullet->AddComponent(transform);

			Sprite* sprite = new Sprite();
			SpriteSourcePtr spriteSource = ResourceGetSpriteSource("Bullet");
			sprite->SetSpriteSource(spriteSource);
			sprite->SetColor(Colors::Yellow);
			bullet->AddComponent(sprite);

			bullet->AddComponent(new RigidBody());
			bullet->AddComponent(new TimedDeath());
			bullet->AddComponent(new ColliderCircle(transform->GetScale().x / 2.0f));
			bullet->AddComponent(new PlayerProjectile());

			//EngineGetModule(GameObjectFactory)->SaveObjectToFile(bullet);
			//space->GetResourceManager().AddSpriteSource(spriteSource);
			//space->GetResourceManager().SaveSpriteSourceToFile(spriteSource);

			return bullet;
		}

		// Create a game object archetype that uses the Asteroid texture.
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateAsteroidArchetype(void)
		{
			GameObject* asteroid = new GameObject("Asteroid");

			Transform* transform = new Transform(0.0f, 0.0f);
			transform->SetScale(Vector2D(1.0f, 1.0f));
			asteroid->AddComponent(transform);

			Sprite* sprite = new Sprite();
			SpriteSourcePtr spriteSource = ResourceGetSpriteSource("Asteroid");
			sprite->SetSpriteSource(spriteSource);
			asteroid->AddComponent(sprite);

			asteroid->AddComponent(new RigidBody());
			asteroid->AddComponent(new Asteroid());
			asteroid->AddComponent(new ScreenWrap());
			asteroid->AddComponent(new ColliderCircle(transform->GetScale().x / 2.0f));

			//EngineGetModule(GameObjectFactory)->SaveObjectToFile(asteroid);
			//space->GetResourceManager().AddSpriteSource(spriteSource);
			//space->GetResourceManager().SaveSpriteSourceToFile(spriteSource);

			return asteroid;
		}

		// Create the monkey object
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//   A pointer to the newly constructed game object.
		GameObject* CreateMonkey(void)
		{
			GameObject* monkey = new GameObject("Monkey");

			Transform* transform = new Transform(0.0f, 0.0f);
			transform->SetScale(Vector2D(1.0f, 1.0f));
			monkey->AddComponent(transform);

			Sprite* sprite = new Sprite();
			sprite->SetSpriteSource(ResourceGetSpriteSource("Monkey"));
			monkey->AddComponent(sprite);

			Animator* animator = new Animator();
			animator->AddAnimation(ResourceGetAnimation("MonkeyIdle"));
			animator->AddAnimation(ResourceGetAnimation("MonkeyJump"));
			animator->AddAnimation(ResourceGetAnimation("MonkeyWalk"));
			monkey->AddComponent(animator);
			monkey->AddComponent(new RigidBody());
			monkey->AddComponent(new ColliderRectangle(Vector2D(transform->GetScale().x * .35f,
				transform->GetScale().y * 0.5f)));
			monkey->AddComponent(new MonkeyMovement());
			monkey->AddComponent(new MonkeyAnimation());

			// Release handle on sprite source
			//EngineGetModule(ResourceManager)->ReleaseResource<SpriteSource>("MonkeyWalk");

			return monkey;
		}

		// Create the spikes game object.
		// Params:
		//   mesh  = The mesh to use for the object's sprite.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateSpikes(MeshPtr mesh)
		{
			GameObject* spikes = new GameObject("Spikes");

			Transform* transform = new Transform(0.0f, 0.0f);
			transform->SetScale(Vector2D(1.0f, 1.0f));
			transform->SetRotation(static_cast<float>(M_PI_2));
			spikes->AddComponent(transform);

			Sprite* sprite = new Sprite();
			sprite->SetMesh(mesh);
			spikes->AddComponent(sprite);

			spikes->AddComponent(new ColliderCircle(transform->GetScale().x / 2.0f));

			//EngineGetModule(GameObjectFactory)->SaveObjectToFile(spikes);

			return spikes;
		}

		// Create the coin game object.
		// Params:
		//   space = The space used to access the resource manager.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateCoin(void)
		{
			GameObject* coin = new GameObject("Coin");

			// Add components: Transform, Sprite, RigidBody, CollderCircle
			Transform* transform = new Transform(0.0f, 0.0f);
			transform->SetScale(Vector2D(0.6f, 0.7f));
			coin->AddComponent(transform);

			Sprite* sprite = new Sprite();
			sprite->SetSpriteSource(ResourceGetSpriteSource("Circle"));
			sprite->SetColor(Colors::Yellow);
			coin->AddComponent(sprite);

			coin->AddComponent(new ColliderCircle(transform->GetScale().x / 2.0f));

			//EngineGetModule(GameObjectFactory)->SaveObjectToFile(coin);
			//space->GetResourceManager().SaveSpriteSourceToFile(sprite->GetSpriteSource());

			return coin;
		}

		// Create the enemy game object.
		// Params:
		//   mesh = Custom mesh used for enemy object.
		// Returns:
		//	 A pointer to the newly constructed game object
		GameObject* CreateEnemy(MeshPtr mesh)
		{
			GameObject* enemy = new GameObject("Enemy");

			Transform* transform = new Transform(0.0f, 0.0f);
			transform->SetScale(Vector2D(1.0f, 1.0f));
			enemy->AddComponent(transform);

			Sprite* sprite = new Sprite();
			sprite->SetMesh(mesh);
			enemy->AddComponent(sprite);

			Enemy* ai = new Enemy();
			enemy->AddComponent(ai);

			enemy->AddComponent(new RigidBody());

			enemy->AddComponent(new ColliderCircle(transform->GetScale().x / 2.0f));

			//EngineGetModule(GameObjectFactory)->SaveObjectToFile(enemy);

			return enemy;
		}
	}
}

//------------------------------------------------------------------------------
