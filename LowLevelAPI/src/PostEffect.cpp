//------------------------------------------------------------------------------
//
// File Name:	PostEffect.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "PostEffect.h"

// Dependencies
#include <glad.h>
#include "../../glfw/src/glfw3.h"

// Systems
#include "ShaderProgram.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   fragmentShader = The filename of the fragment shader for this effect.
	//   vertexShader = The filename of the vertex shader for this effect. Defaults to simple vertex shader.
	PostEffect::PostEffect(const std::string& fragmentShader, const std::string& vertexShader)
		: program(nullptr), active(false)
	{
		program = ShaderProgram::CreateProgramFromFile(vertexShader, fragmentShader);
		if (program == nullptr)
		{
			std::cout << "Error creating post-processing effect. Shader program creation failed." << std::endl;
		}
	}

	PostEffect::~PostEffect()
	{
		delete program;
	}

	// Override this to reset variables as needed.
	void PostEffect::Initialize()
	{
	}

	// Override this to modify variables before they are sent to the shader.
	void PostEffect::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Test whether this effect is currently being used by the renderer.
	bool PostEffect::IsActive() const
	{
		return active;
	}

	const ShaderProgram& PostEffect::GetProgram() const
	{
		return *program;
	}

	// Sends uniform data to shader. Called automatically by the Render function.
	// Override this to send custom data to the shader.
	void PostEffect::Draw()
	{
	}

	// Renders from the source texture to the target using the given effect.
	// Params:
	//   sourceTexture = The texture we will be affecting.
	//   width = Width of the source/target textures.
	//   height = Height of the source/target textures.
	void PostEffect::Render(unsigned sourceTexture, unsigned width, unsigned height)
	{
		if (program == nullptr)
			return;

		// Copy current result to diffuseTexture1 (GL_COLOR_ATTACHMENT1)
		GLuint attachments[1];
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		attachments[0] = GL_COLOR_ATTACHMENT1;
		glDrawBuffers(1, attachments);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		// Switch back to drawing to diffuseTexture0
		attachments[0] = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, attachments);

		// Clear the buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Use shader
		program->Use();

		// Set uniforms
		Draw();

		// Use the texture we rendered to earlier
		glBindTexture(GL_TEXTURE_2D, sourceTexture);
		// Draw all the things
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

//------------------------------------------------------------------------------
