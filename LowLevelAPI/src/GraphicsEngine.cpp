//------------------------------------------------------------------------------
//
// File Name:	GraphicsEngine.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GraphicsEngine.h"

// EXTERNAL LIBRARIES
// OpenGl
#include <glad.h>
#include <GLFW/glfw3.h>

// Math
#include <glm_include.h>

// Fonts
#include <ft2build.h>
#include FT_FREETYPE_H

// BETA LOW-LEVEL
// Systems
#include "ShaderProgram.h"	// SetUniform
#include "Renderer.h"

// Math
#include "Vector2D.h"	// for texture coordinates

// Resources
#include "Texture.h"	// GetBufferID, default constructor
#include "Camera.h"		// for creating view matrix

//------------------------------------------------------------------------------
// Libraries:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

namespace Beta
{
	// FreeType defines
	static const int FT_Success = 0;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

#if BE_GL_4_3_API
	void APIENTRY GlErrorCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
#ifdef _DEBUG
		UNREFERENCED_PARAMETER(userParam);
		UNREFERENCED_PARAMETER(length);

		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << "---------------" << std::endl;
		std::cout << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << std::endl;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;
#else
		UNREFERENCED_PARAMETER(source);
		UNREFERENCED_PARAMETER(type);
		UNREFERENCED_PARAMETER(id);
		UNREFERENCED_PARAMETER(severity);
		UNREFERENCED_PARAMETER(message);
		UNREFERENCED_PARAMETER(userParam);
		UNREFERENCED_PARAMETER(length);
#endif
	}
#endif
	void CheckForOpenGLErrors()
	{
#ifdef _DEBUG
#if !BE_GL_4_3_API
		//static unsigned errorsGiven = 0;
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "ERROR: OpenGL error detected (" << err << "):" << std::endl;

			switch (err)
			{
			case GL_INVALID_ENUM:
				std::cout << "An invalid enumeration parameter was given." << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cout << "An invalid value parameter was given." << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cout << "Current state does not support the parameters given, or combination of parameters was incompatible." << std::endl;
				break;
			case GL_STACK_OVERFLOW:
				std::cout << "Stack pushing operation cannot be done because it would overflow the limit of that stack's size." << std::endl;
				break;
			case GL_STACK_UNDERFLOW:
				std::cout << "Stack popping operation cannot be done because the stack is already at its lowest point." << std::endl;
				break;
			case GL_OUT_OF_MEMORY:
				std::cout << "Memory cannot be allocated." << std::endl;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cout << "Attempted to write to incomplete frame buffer." << std::endl;
				break;
			default:
				std::cout << "Non-standard error." << std::endl;
				break;
			}
			//++errorsGiven;
			//std::cout << errorsGiven << " OpenGL errors detected so far." << std::endl;
		}
#endif
#endif
	}

	//------------------------------------------------------------------------------
	// Private Helper Variables and Functions:
	//------------------------------------------------------------------------------

	class GraphicsEngine::Implementation
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		Implementation();
		~Implementation();

		void Initialize();

		// Viewport clear color
		void SetBackgroundColor(const Color& color = Colors::Black);

		// Color to blend with whole screen
		void SetScreenTintColor(const Color& color = Colors::White);

		// Color to blend with the current sprite's color
		void SetSpriteBlendColor(const Color& color = Colors::White);

		// Set the dimensions of the viewport.
		// Params:
		//   width = The new width of the viewport.
		//   height = The new height of the viewport.
		void SetViewport(unsigned width, unsigned height);

		// Sets how sprites are blended
		void SetBlendMode(BlendMode mode, bool forceSet = false);

		//------------------------------------------------------------------------------
		// Public Variables
		//------------------------------------------------------------------------------

		// Colors and blending
		Color backgroundColor;
		Color tintColor;
		Color blendColor;
		BlendMode blendMode;

		// Textures
		const Texture* defaultTexture;
		Vector2D textureCoords;
		float alpha;

		// Viewport
		unsigned viewportWidth;
		unsigned viewportHeight;

		// Cameras
		Camera defaultCamera;

		// Renderer
		Renderer renderer;

		// Settings
		bool useVsync;

		// Fonts
		FontSystem* fontSystem;

	private:
		void InitRenderer();
	};

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	GraphicsEngine::GraphicsEngine()
		: pimpl(nullptr)
	{
		// Create data
		pimpl = new Implementation();
	}

	void GraphicsEngine::Initialize(unsigned width, unsigned height)
	{
		// Initialize OpenGL
		pimpl->Initialize();

#if BE_GL_4_3_API
		// Error handling
		glDebugMessageCallback(GlErrorCallback, nullptr);
#endif

		// Set the viewport
		SetViewport(width, height);
		CheckForOpenGLErrors();
	}

	GraphicsEngine::~GraphicsEngine()
	{
		delete pimpl;
	}

	void GraphicsEngine::FrameStart()
	{
		// Set the clear color and depth value
		pimpl->renderer.FrameStart();

		// Set camera to default
		GetDefaultCamera().Use();

		// Renderer disables depth test at frame end, so we need to re-enable it
		if (pimpl->defaultCamera.GetProjectionMode() == PM_Perspective)
		{
			glEnable(GL_DEPTH_TEST);
		}
	}

	void GraphicsEngine::FrameEnd()
	{
		// Wait till OpenGL is done and swap the frame buffer
		pimpl->renderer.FrameEnd();

		// Unbind current texture (if any)
		glBindTexture(GL_TEXTURE_2D, 0);

		CheckForOpenGLErrors();
	}

	const Color& GraphicsEngine::GetBackgroundColor() const
	{
		return pimpl->backgroundColor;
	}

	void GraphicsEngine::SetBackgroundColor(const Color& color)
	{
		pimpl->SetBackgroundColor(color);
	}

	const Color& GraphicsEngine::GetScreenTintColor() const
	{
		return pimpl->tintColor;
	}

	void GraphicsEngine::SetScreenTintColor(const Color& color)
	{
		pimpl->SetScreenTintColor(color);
	}

	void GraphicsEngine::SetSpriteBlendColor(const Color& color)
	{
		pimpl->SetSpriteBlendColor(color);
	}

	void GraphicsEngine::SetBlendMode(BlendMode mode, bool forceSet)
	{
		pimpl->SetBlendMode(mode, forceSet);
	}

	const Texture& GraphicsEngine::GetDefaultTexture() const
	{
		return *pimpl->defaultTexture;
	}

	void GraphicsEngine::SetTransform(const Matrix2D& matrix, float depth)
	{
		// Convert 2D matrix to 3D matrix
		Matrix3D worldMatrix(matrix);
		glm::mat4& world = static_cast<glm::mat4&>(*static_cast<glm::mat4*>(worldMatrix.data));
		world[3].z = depth;

		// Send world matrix to OpenGL
		GetSpriteShader().SetUniform("worldMatrix", worldMatrix);
		GetFontShader().SetUniform("worldMatrix", worldMatrix);
	}

	void GraphicsEngine::SetTransform(const Vector2D& translation, const Vector2D& scale, float rotation, float depth)
	{
		SetTransform(Matrix2D::TranslationMatrix(translation.x, translation.y)
			* Matrix2D::RotationMatrixRadians(rotation) * Matrix2D::ScalingMatrix(scale.x, scale.y), depth);
	}

	Camera& GraphicsEngine::GetDefaultCamera() const
	{
		return pimpl->defaultCamera;
	}

	const ShaderProgram& GraphicsEngine::GetSpriteShader() const
	{
		return pimpl->renderer.GetSpriteShader();
	}

	const ShaderProgram& GraphicsEngine::GetFontShader() const
	{
		return pimpl->renderer.GetFontShader();
	}

	void GraphicsEngine::PushEffect(PostEffect& effect)
	{
		pimpl->renderer.PushEffect(effect);
	}

	void GraphicsEngine::PopEffect()
	{
		pimpl->renderer.PopEffect();
	}

	// Removes a specific effect.
	void GraphicsEngine::RemoveEffect(PostEffect& effect)
	{
		pimpl->renderer.RemoveEffect(effect);
	}

	void GraphicsEngine::RemoveEffect(PostEffect* effect)
	{
		pimpl->renderer.RemoveEffect(*effect);
	}

	void GraphicsEngine::ClearEffects()
	{
		pimpl->renderer.ClearEffects();
	}

	// Test whether vertical sync is currently on
	bool GraphicsEngine::GetUseVsync() const
	{
		return pimpl->useVsync;
	}

	// Turns vertical sync on or off - will cause performance issues on some machines
	void GraphicsEngine::SetUseVSync(bool _useVsync)
	{
		if (_useVsync != pimpl->useVsync)
		{
			pimpl->useVsync = _useVsync;

			if (pimpl->useVsync) glfwSwapInterval(1);
			else glfwSwapInterval(0);
		}
	}

	Vector2D GraphicsEngine::GetViewport() const
	{
		return Vector2D(static_cast<float>(pimpl->viewportWidth), static_cast<float>(pimpl->viewportHeight));
	}

	// Set the dimensions of the viewport.
	// Params:
	//   width = The new width of the viewport.
	//   height = The new height of the viewport.
	void GraphicsEngine::SetViewport(unsigned width, unsigned height)
	{
		pimpl->SetViewport(width, height);
	}

	FontSystem* GraphicsEngine::GetFontSystem() const
	{
		return pimpl->fontSystem;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	GraphicsEngine::Implementation::Implementation()
		: defaultTexture(nullptr), blendMode(BM_Blend), useVsync(false),
		alpha(1.0f), viewportHeight(0), viewportWidth(0), fontSystem(nullptr)
	{
	}

	GraphicsEngine::Implementation::~Implementation()
	{
		delete defaultTexture;
		if (FT_Done_FreeType(fontSystem) != FT_Success)
		{
			std::cout << "ERROR: Could not shut down FreeType Library!" << std::endl;
		}
	}

	void GraphicsEngine::Implementation::Initialize()
	{
		// Initialize OpenGL extension wrangler (GLAD)
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		// If not using vsync, disable it
		if (!useVsync) glfwSwapInterval(0);
		else glfwSwapInterval(1);

		std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "OpenGL shader version supported: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		// Enable anti-aliasing for lines
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		// Enable multisampling(?)
		glEnable(GL_MULTISAMPLE);

		// Default texture
		defaultTexture = new Texture();

		// Renderers
		InitRenderer();

		// Fonts
		if (FT_Init_FreeType(&fontSystem) != FT_Success)
		{
			std::cout << "ERROR: Could not init FreeType Library!" << std::endl;
		}
	}

	void GraphicsEngine::Implementation::SetBackgroundColor(const Color& color)
	{
		backgroundColor = color.Clamp();

		// Tint applies to background as well
		glClearColor(backgroundColor.r * tintColor.r, backgroundColor.g * tintColor.g,
			backgroundColor.b * tintColor.b, backgroundColor.a * tintColor.a);
	}

	// Color to blend with whole screen
	void GraphicsEngine::Implementation::SetScreenTintColor(const Color& color)
	{
		tintColor = color.Clamp();

		// Tint sprites
		renderer.GetSpriteShader().SetUniform("tintColor", tintColor);
		renderer.GetFontShader().SetUniform("tintColor", tintColor);

		// Update clear color since tint affects it
		glClearColor(backgroundColor.r * tintColor.r, backgroundColor.g * tintColor.g,
			backgroundColor.b * tintColor.b, backgroundColor.a * tintColor.a);
	}

	// Color to blend with the current sprite's color
	void GraphicsEngine::Implementation::SetSpriteBlendColor(const Color& color)
	{
		blendColor = color.Clamp();
		renderer.GetSpriteShader().SetUniform("blendColor", blendColor);
		renderer.GetFontShader().SetUniform("blendColor", blendColor);
	}

	// Set the dimensions of the viewport.
	// Params:
	//   width = The new width of the viewport.
	//   height = The new height of the viewport.
	void GraphicsEngine::Implementation::SetViewport(unsigned width, unsigned height)
	{
		// Save viewport info
		viewportWidth = width; viewportHeight = height;

		// Send viewport info to OpenGL
		glViewport(0, 0, viewportWidth, viewportHeight);

		// Reset renderer
		renderer.SetDimensions(viewportWidth, viewportHeight);
	}

	// Sets how sprites are blended
	void GraphicsEngine::Implementation::SetBlendMode(BlendMode mode, bool forceSet)
	{
		// Don't do anything if setting isn't different
		if (mode == blendMode && !forceSet)
			return;

		blendMode = mode;

		switch (blendMode)
		{
		case BM_None:
			glDisable(GL_BLEND);
			break;

		case BM_Blend:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			break;

		case BM_Add:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBlendEquation(GL_FUNC_ADD);
			break;

		case BM_Multiply:
			glEnable(GL_BLEND);
			glBlendFuncSeparate(GL_DST_COLOR, GL_ZERO, GL_DST_ALPHA, GL_ZERO);
			glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
			break;

		case BM_Screen:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			glBlendEquation(GL_FUNC_ADD);
			break;
		}
	}

	void GraphicsEngine::Implementation::InitRenderer()
	{
		renderer.Init();
		SetBlendMode((BlendMode)blendMode, true);
		SetBackgroundColor();
		SetScreenTintColor();
		SetSpriteBlendColor();
	}
}

//------------------------------------------------------------------------------
