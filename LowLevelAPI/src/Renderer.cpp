//------------------------------------------------------------------------------
//
// File Name:	Renderer.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
// Code lifted from the following tutorial:
// https://learnopengl.com/Advanced-OpenGL/Framebuffers
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Renderer.h"

// Dependencies
#include <glad.h>
#include "../../glfw/src/glfw3.h"

// Resources
#include "PostEffect.h"
#include "ShaderProgram.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Renderer::Renderer()
		: width(0), height(0), spriteShader(0), frameBuffer(0), diffuseTexture0(0), diffuseTexture1(0),
		quadVertexArray(0), quadVertexBuffer(0), bufferToScreenShader(0), fontShader(0)
	{
	}

	Renderer::~Renderer()
	{
		delete spriteShader;
		delete bufferToScreenShader;
		delete fontShader;

		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &diffuseTexture0);
		glDeleteTextures(1, &diffuseTexture1);
		glDeleteBuffers(1, &quadVertexBuffer);
		glDeleteVertexArrays(1, &quadVertexArray);
	}

	void Renderer::Init()
	{
		// Load first and final stage shaders
		spriteShader = ShaderProgram::CreateProgramFromFile("spriteShader.vert", "spriteShader.frag");
		bufferToScreenShader = ShaderProgram::CreateProgramFromFile("passthroughShader.vert", "passthroughShader.frag");
		fontShader = ShaderProgram::CreateProgramFromFile("spriteShader.vert", "fontShader.frag");

		// GENERATE VAO FOR SCREEN QUAD
		glGenVertexArrays(1, &quadVertexArray);
		glGenBuffers(1, &quadVertexBuffer);
		glBindVertexArray(quadVertexArray);

		// Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		GLfloat quadVertices[] =
		{
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		GLint numVertices = 6;
		GLint floatsPerPosition = 2;
		GLint floatsPerUV = 2;
		GLint floatsPerVertex = floatsPerPosition + floatsPerUV;

		glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices * floatsPerVertex * sizeof(float), &quadVertices, GL_STATIC_DRAW);
		// Bind buffer 0: Position
		GLint positionBuffer = 0;
		glEnableVertexAttribArray(positionBuffer);
		glVertexAttribPointer(positionBuffer, floatsPerPosition, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), nullptr);
		// Bind buffer 1: Texture Coordinates
		GLint uvBuffer = 1;
		glEnableVertexAttribArray(uvBuffer);
		glVertexAttribPointer(uvBuffer, floatsPerUV, GL_FLOAT, GL_FALSE,
			floatsPerVertex * sizeof(float), (void*)(floatsPerPosition * sizeof(float)));

		// Unbind buffer and VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Renderer::FrameStart()
	{
		// Use sprite shader
		spriteShader->Use();

		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		// Clear the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::FrameEnd()
	{
		// Disable depth test for now
		glDisable(GL_DEPTH_TEST);

		// Bind the vertex array object
		glBindVertexArray(quadVertexArray);

		// Apply all post-processing
		ApplyEffects();

		// Use the default framebuffer (the screen) instead of our custom buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use final stage shader
		bufferToScreenShader->Use();

		// Use the texture we rendered to earlier
		glBindTexture(GL_TEXTURE_2D, diffuseTexture0);
		// Draw all the things
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Unbind the VAO
		glBindVertexArray(0);
	}

	// Get the default shader (usually for the purposes of setting uniforms)
	const ShaderProgram& Renderer::GetSpriteShader() const
	{
		return *spriteShader;
	}

	const ShaderProgram& Renderer::GetFontShader() const
	{
		return *fontShader;
	}

	void Renderer::SetDimensions(unsigned _width, unsigned _height)
	{
		if (width == _width && height == _height)
			return;
		if (_width == 0 || _height == 0)
			return;

		width = _width; height = _height;

		// If they already exist, delete them
		if (frameBuffer != 0)
		{
			glDeleteFramebuffers(1, &frameBuffer);
			glDeleteTextures(1, &diffuseTexture0);
			glDeleteTextures(1, &diffuseTexture1);
		}

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		// The textures we're going to render to
		glGenTextures(1, &diffuseTexture0);
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, diffuseTexture0);
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		// Poor filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Set first texture as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuseTexture0, 0);

		glGenTextures(1, &diffuseTexture1);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Set second texture as our colour attachement #1
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffuseTexture1, 0);

		// Depth attachment
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			exit(-1);

		// Unbind the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::PushEffect(PostEffect & effect)
	{
		if (effects.Find(&effect) != effects.End())
			return;

		effect.active = true;
		effect.Initialize();
		effects.PushBack(&effect);
	}

	void Renderer::PopEffect()
	{
		if (effects.IsEmpty())
			return;

		effects.Back()->active = false;
		effects.PopBack();
	}

	void Renderer::RemoveEffect(PostEffect & effect)
	{
		auto location = effects.Find(&effect);

		if (location == effects.End())
			return;

		(*location)->active = false;
		effects.Erase(location);
	}

	void Renderer::ClearEffects()
	{
		for (auto it = effects.Begin(); it != effects.End(); ++it)
		{
			(*it)->active = false;
		}
		effects.Clear();
	}

	void Renderer::ApplyEffects()
	{
		// For each effect
		for (auto it = effects.Begin(); it != effects.End(); ++it)
		{
			// Apply the effect
			(*it)->Render(diffuseTexture1, width, height);
		}
	}
}
