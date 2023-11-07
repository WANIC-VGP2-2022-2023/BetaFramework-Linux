//------------------------------------------------------------------------------
//
// File Name:	Sprite.cpp
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
#include "Sprite.h"

// Resources
#include "SpriteSource.h"
#include "Camera.h"			// GetViewMatrix
#include "Mesh.h"			// Draw
#include <ShaderProgram.h>	// Use

// Components
#include "GameObject.h" // GetComponent
#include "Transform.h"	// GetMatrix, GetTranslation

// Math
#include "Matrix2D.h"

// Systems
#include <EngineCore.h>			// GetModule
#include "ResourceManager.h"	// GetSpriteSource
#include <GraphicsEngine.h>		// SetTexture
#include <MeshFactory.h>		// CreateQuadMesh
#include "FileStream.h"
#include "Space.h"

//------------------------------------------------------------------------------

namespace Beta
{


	MeshManager Sprite::meshManager;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	MeshManager::MeshManager()
		: ResourceManager("Meshes/")
	{
	}

	MeshPtr MeshManager::Create(const std::string& name)
	{
		MeshFactory* factory = EngineGetModule(MeshFactory);
		MeshPtr sharedMesh;

		if (name == "DefaultSpriteMesh")
		{
			sharedMesh = MeshPtr(factory->CreateQuadMesh());
		}
		else
		{
			// Try to load from file
			sharedMesh = MeshPtr(factory->CreateMeshFromFile(name));

			// Did we find a mesh?
			if (sharedMesh == nullptr)
			{
				// No luck, use default mesh
				return ResourceGetMesh("DefaultSpriteMesh");
			}
		}

		sharedMesh->SetName(name);

		return sharedMesh;
	}

	// Create a new sprite object.
	Sprite::Sprite()
		: Component("Sprite"), transform(nullptr), frameIndex(0), spriteSource(nullptr), 
		mesh(nullptr), zDepth(0.0f), flipX(false), flipY(false)
	{
	}

	void Sprite::Initialize()
	{
		// Get the transform component
		transform = GetOwner()->GetComponent<Transform>();

		// Is there a mesh?
		if (mesh == nullptr)
		{
			// So just find/create the mesh.
			mesh = ResourceGetMesh(GetOwner()->GetName());
		}
	}

	// Draw a sprite (Sprite can be textured or untextured).
	void Sprite::Draw()
	{
		// Call other draw function
		Draw(Vector2D(0.0f, 0.0f));
	}

	void Sprite::Draw(const Vector2D & offset)
	{
		// No mesh - get out!
		if (mesh == nullptr) return;
		
		// No transform - retreat!
		if (transform == nullptr) return;

		GraphicsEngine & graphics = *EngineGetModule(GraphicsEngine);
		graphics.GetSpriteShader().Use();

		if (spriteSource)
		{
			spriteSource->UseTexture(frameIndex, flipX, flipY);
		}
		else
		{
			graphics.GetDefaultTexture().Use();
		}

		// Set blend color
		graphics.SetSpriteBlendColor(color);

		// Set transform using given translation
		graphics.SetTransform(Matrix2D::TranslationMatrix(offset.x, offset.y) * transform->GetMatrix(), zDepth);

		// Draw!
		mesh->Draw();
	}

	// Set a sprite's transparency (between 0.0f and 1.0f).
	// Params:
	//   alpha = New value for the sprite's 'alpha' value.
	void Sprite::SetAlpha(float alpha)
	{
		color.a = std::max(alpha, 0.0f);
		color.a = std::min(alpha, 1.0f);
	}

	// Get the current value for a sprite's transparency.
	float Sprite::GetAlpha() const
	{
		return color.a;
	}

	// Set the sprite's current frame.
	// (NOTE: You must validate the frame index against the texture's frame count.)
	// Params:
	//   frameIndex = New frame index for the sprite (0 .. frame count).
	void Sprite::SetFrame(unsigned int frameIndex_)
	{
		if (frameIndex_ < spriteSource->GetFrameCount())
		{
			frameIndex = frameIndex_;
		}
	}

	// Set the sprite's mesh.
	// (NOTE: This mesh may be textured or untextured.)
	// (NOTE: This mesh may contain any number of triangles.)
	// Params:
	//   mesh = Pointer to a mesh created using the Beta Framework.
	void Sprite::SetMesh(MeshPtr mesh_)
	{
		mesh = mesh_;
	}

	// Set a new SpriteSource for the specified sprite.
	// Params:
	//	 spriteSource = A new sprite source for the sprite.
	void Sprite::SetSpriteSource(ConstSpriteSourcePtr spriteSource_)
	{
		spriteSource = spriteSource_;
	}

	// Returns the current sprite source being used by the sprite.
	ConstSpriteSourcePtr Sprite::GetSpriteSource()
	{
		return spriteSource;
	}

	// Set the tint color for the specified sprite.
	// Params:
	//	 color = A new color for the sprite.
	void Sprite::SetColor(Color color_)
	{
		color = color_;
	}

	// Retrieves the blend color for the sprite.
	const Color& Sprite::GetColor() const
	{
		return color;
	}

	float Sprite::GetZDepth() const
	{
		return zDepth;
	}

	void Sprite::SetZDepth(float depth_)
	{
		zDepth = depth_;
	}

	void Sprite::SetFlipX(bool flipX_)
	{
		flipX = flipX_;
	}

	void Sprite::SetFlipY(bool flipY_)
	{
		flipY = flipY_;
	}


	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void Sprite::Serialize(FileStream & stream) const
	{
		if (spriteSource == nullptr)
		{
			stream.WriteVariable("spriteSource", "null");
		}
		else
		{
			stream.WriteVariable("spriteSource", spriteSource->GetName());
		}

		stream.WriteVariable("color", color);
		stream.WriteVariable("zDepth", zDepth);
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void Sprite::Deserialize(FileStream & stream)
	{
		std::string spriteSourceText;
		stream.ReadVariable("spriteSource", spriteSourceText);

		// Is there a sprite source?
		if (spriteSourceText == "null")
		{
			spriteSource = nullptr;
		}
		else
		{
			// If so, retrieve using resource manager
			//spriteSource = GetOwner()->GetSpace()->GetResourceManager().GetSpriteSource(spriteSourceText);
			spriteSource = std::shared_ptr<SpriteSource>(
				ResourceGetSpriteSource(spriteSourceText));
		}

		stream.ReadVariable("color", color);
		stream.ReadVariable("zDepth", zDepth);
	}

	MeshManager& Sprite::GetMeshManager()
	{
		return meshManager;
	}

	// RTTI
	COMPONENT_SUBCLASS_DEFINITION(Sprite)
}
