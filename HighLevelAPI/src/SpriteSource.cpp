//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.cpp
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
#include "SpriteSource.h"

// Resources
#include <Texture.h>

// Systems
#include <EngineCore.h>	// GetFilePath
#include <FileStream.h>

// Math
#include <Vector2D.h> // SetX and SetY

//------------------------------------------------------------------------------

namespace Beta
{
	SpriteSourceManager SpriteSource::spriteSourceManager;
	TextureManager SpriteSource::textureManager;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	SpriteSourceManager::SpriteSourceManager()
		: ResourceManager("SpriteSources/")
	{
	}

	SpriteSourcePtr SpriteSourceManager::Create(const std::string& name)
	{
		// Try to load from file
		SpriteSourcePtr spriteSource = std::make_shared<SpriteSource>();

		std::string filename = GetFilePath() + name + GetFileExtension();
		FileStream stream(filename, OM_Read);
		try
		{
			// Attempt to load sprite source
			spriteSource->Deserialize(stream);

			// Set the sprite source's texture
			// Get texture if it exists, otherwise create it
			TexturePtr texture = SpriteSource::textureManager.GetResource(spriteSource->textureName);
			spriteSource->texture = texture;
		}
		catch (const FileStreamException & e)
		{
			std::cout << "ERROR in SpriteSourceManager: " << e.what() << std::endl;
			// Destroy the object
			spriteSource.reset();
		}

		return spriteSource;
	}

	TextureManager::TextureManager()
		: ResourceManager("Textures/", ".png")
	{
	}

	TexturePtr TextureManager::Create(const std::string& name)
	{
		return TexturePtr(Texture::CreateTextureFromFile(name));
	}

	// Constructor for SpriteSource
	// Params:
	//   texture = The texture that contains the frames that this sprite source will use.
	//   name = The name to use for this sprite source.
	//	 numCols = The number of total columns in the sprite sheet texture.
	//	 numRows = The number of total rows in the sprite sheet texture.
	SpriteSource::SpriteSource(ConstTexturePtr texture, std::string _name,
		unsigned numCols, unsigned numRows)
		: numCols(numCols), numRows(numRows), texture(texture), name(_name)
	{
		// If name is empty, use name of texture without file extension
		if (name == "" && texture != nullptr)
		{
			// Get texture name without path
			std::string filename = texture->GetName();
			size_t lastSlash = filename.find_last_of('/');
			if (lastSlash != std::string::npos)
				filename = filename.substr(lastSlash + 1);

			// Remove file extension (.png)
			name = filename.substr(0, filename.length() - 4);
		}
	}

	void SpriteSource::UseTexture(unsigned frameIndex, bool flipX, bool flipY) const
	{
		Vector2D uvOffset = GetUV(frameIndex);
		texture->Use(numCols, numRows, flipX, flipY, uvOffset);
	}

	// Returns the maximum number of possible frames in the sprite source's texture (rows * cols).
	unsigned SpriteSource::GetFrameCount() const
	{
		return numCols * numRows;
	}

	// Returns the UV coordinates of the specified frame in a sprite sheet.
	// Params:
	//	 frameIndex = A frame index within a sprite sheet.
	// Returns:
	//   A vector containing the UV/texture coordinates.
	const Vector2D SpriteSource::GetUV(unsigned int frameIndex) const
	{
		Vector2D textureCoords;

		float uSize = 1.0f / numCols;
		float vSize = 1.0f / numRows;

		textureCoords.x = uSize * (frameIndex % numCols);
		textureCoords.y = vSize * (frameIndex / numCols);

		return textureCoords;
	}

	// Gets the name of the sprite source.
	const std::string & SpriteSource::GetName() const
	{
		return name;
	}

	// Gets the dimensions of the texture in rows/cols
	const Vector2D SpriteSource::GetTextureDimensions() const
	{
		return Vector2D(static_cast<float>(numCols), static_cast<float>(numRows));
	}

	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void SpriteSource::Serialize(FileStream & stream) const
	{
		stream.WriteValue(name);
		stream.BeginScope();

		stream.WriteVariable("numRows", numRows);
		stream.WriteVariable("numCols", numCols);

		// Extract the filename from the relative path
		std::string filename = texture->GetName();
		size_t lastSlash = filename.find_last_of('/');
		if (lastSlash != std::string::npos)
			filename = filename.substr(lastSlash + 1);

		// Write texture filename to file
		stream.WriteVariable("texture", filename);

		stream.EndScope();
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void SpriteSource::Deserialize(FileStream & stream)
	{
		stream.ReadValue(name);
		stream.BeginScope();

		stream.ReadVariable("numRows", numRows);
		stream.ReadVariable("numCols", numCols);

		// Read the texture filename
		stream.ReadVariable("texture", textureName);

		stream.EndScope();
	}

	// Saves a sprite source to a file so it can be loaded later.
	void SpriteSource::SaveToFile() const
	{
		// Create filename
		std::string filename = SpriteSource::spriteSourceManager.GetFilePath() + GetName()
			+ SpriteSource::spriteSourceManager.GetFileExtension();
		FileStream stream(filename, OM_Write);

		// Attempt to save object
		Serialize(stream);
		std::cout << "Wrote sprite source " << GetName() << " to file " << filename << std::endl;
	}

	SpriteSourceManager& SpriteSource::GetSpriteSourceManager()
	{
		return spriteSourceManager;
	}

	TextureManager& SpriteSource::GetTextureManager()
	{
		return textureManager;
	}
}
