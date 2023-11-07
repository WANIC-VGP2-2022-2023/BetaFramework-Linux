//------------------------------------------------------------------------------
//
// File Name:	Texture.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Texture.h"

// OpenGL
#include <glad.h>
#include "../../glfw/src/glfw3.h"

// Texture loading
#pragma warning(push, 1)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)

// Systems
#include "EngineCore.h"		// GetModule, GetFilePath
#include "GraphicsEngine.h" // GetSpriteShader
#include "ShaderProgram.h"	// SetUniform

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	std::string Texture::texturePath = "Textures/";

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates a 1x1 white texture
	Texture::Texture()
		: bufferID(0), graphics(*EngineGetModule(GraphicsEngine))
	{
		Array<Color> white(1);
		white[0] = Colors::White;
		CreateTextureFromArray(white, 1, 1);
	}

	// Loads a texture from an array
	Texture::Texture(const Array<Color>& colors, size_t width, size_t height, const std::string& name)
		: bufferID(0), name(name), graphics(*EngineGetModule(GraphicsEngine))
	{
		CreateTextureFromArray(colors, width, height);
	}

	Texture::Texture(const unsigned char* buffer, size_t width, size_t height, const std::string& name)
		: bufferID(0), name(name), graphics(*EngineGetModule(GraphicsEngine))
	{
		CreateRedTextureFromArray(buffer, width, height);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &bufferID);
	}

	// Set this as the active texture
	// Params:
	//   uvOffset = The texture coordinate offset to use when rendering.
	void Texture::Use(const Vector2D& uvOffset, const ShaderProgram* shader) const
	{
		// Assume 1 column and row, no flipping
		Use(1, 1, false, false, uvOffset, shader);
	}

	void Texture::Use(unsigned spriteColumns, unsigned spriteRows, bool flipX, bool flipY, const Vector2D& uvOffset, const ShaderProgram* shader) const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bufferID);

		// Default to using sprite shader
		if (shader == nullptr)
			shader = &graphics.GetSpriteShader();

		shader->SetUniform("diffuse", 0);

		// Store UV
		Vector2D modifiedUV = uvOffset;
		shader->SetUniform("uvOffset", modifiedUV);
		shader->SetUniform("uvStride", Vector2D(1.0f / static_cast<float>(spriteColumns), 1.0f / static_cast<float>(spriteRows)));

		// Flip
		shader->SetUniform("flipX", flipX);
		shader->SetUniform("flipY", flipY);
	}

	const std::string& Texture::GetName() const
	{
		return name;
	}

	unsigned Texture::GetBufferID() const
	{
		return bufferID;
	}

	// Loads a texture from a file
	Texture* Texture::CreateTextureFromFile(const std::string & filename, TextureFilterMode mode)
	{
		// Create full filepath
		const std::string& enginePath = EngineCore::GetInstance().GetFilePath();
		std::string filePath = enginePath + texturePath + filename;

		// Attempt to open file
		int width, height, numChannels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);
		if (data == nullptr || width == 0 || height == 0)
		{
			std::cout << "Error loading texture from file " << filename << std::endl
				<< "Make sure the specified path is valid." << std::endl;
			return nullptr;
		}

		// Pick format based on channels
		unsigned channelMode = 0;
		switch (numChannels)
		{
		case 4:
			channelMode = GL_RGBA;
			break;
		case 3:
			channelMode = GL_RGB;
			break;
		case 2:
			channelMode = GL_RG;
			break;
		case 1:
			channelMode = GL_RED;
			break;
		default:
			std::cout << "Error loading texture from file " << filename << std::endl
				<< "Unknown color channel format." << std::endl;
			stbi_image_free(data);
			return nullptr;
			break;
		}

		// Create texture object from bitmap
		unsigned bufferID;
		glGenTextures(1, &bufferID);
		Texture* texture = new Texture(bufferID, filename);

		unsigned minFilter;
		unsigned magFilter;

		switch (mode)
		{
		case TM_Bilinear:
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR;
			break;
		case TM_Trilinear:
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
			break;
		default:
			minFilter = magFilter = GL_NEAREST;
			break;
		}

		// Bind created texture
		glBindTexture(GL_TEXTURE_2D, bufferID);

		// Load the data
		glTexImage2D(GL_TEXTURE_2D, 0, channelMode, width,
			height, 0, channelMode, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture filtering mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Free the image data
		stbi_image_free(data);

		// Return finished texture
		return texture;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Creates a texture with the given buffer ID and name
	Texture::Texture(unsigned buffer, const std::string & name)
		: bufferID(buffer), name(name), graphics(*EngineGetModule(GraphicsEngine))
	{
	}

	// Loads a texture from an array
	void Texture::CreateTextureFromArray(const Array<Color> & colors, size_t width,
		size_t height, TextureFilterMode mode)
	{
		unsigned minFilter;
		unsigned magFilter;

		switch (mode)
		{
		case TM_Bilinear:
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR;
			break;
		case TM_Trilinear:
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
			break;
		default:
			minFilter = magFilter = GL_NEAREST;
			break;
		}

		// Allocate texture
		glGenTextures(1, &bufferID);

		// Bind created texture
		glBindTexture(GL_TEXTURE_2D, bufferID);

		// Load the data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(width),
			static_cast<GLsizei>(height), 0, GL_RGBA, GL_FLOAT, colors.Data());

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture filtering mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::CreateRedTextureFromArray(const unsigned char* buffer, size_t width, size_t height, TextureFilterMode mode)
	{
		unsigned minFilter;
		unsigned magFilter;

		switch (mode)
		{
		case TM_Bilinear:
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR;
			break;
		case TM_Trilinear:
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
			break;
		default:
			minFilter = magFilter = GL_NEAREST;
			break;
		}

		// Allocate texture
		glGenTextures(1, &bufferID);

		// Bind created texture
		glBindTexture(GL_TEXTURE_2D, bufferID);

		// Load the data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<GLsizei>(width),
			static_cast<GLsizei>(height), 0, GL_RED, GL_UNSIGNED_BYTE, buffer);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture filtering mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
