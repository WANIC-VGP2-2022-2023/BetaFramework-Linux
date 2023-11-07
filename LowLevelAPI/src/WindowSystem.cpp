//------------------------------------------------------------------------------
//
// File Name:	WindowSystem.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		CS230
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "WindowSystem.h"

// Graphics
#include <GLFW/glfw3.h>

// Systems
#include "EngineCore.h"		// GetModule
#include "GraphicsEngine.h" // SetViewport
#include "Input.h"			// SetMouseScrollAmount, SetCursorPosition

// Misc
#include <cassert>		// assert
//#include <objbase>	// CoInitialize

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	const std::string BE_LL_VERSION = "0.7.2";

	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	// GLFW
	void ErrorCallback(int error, const char* description);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void WinCloseCallback(GLFWwindow* window);
	void ViewportChangeCallback(GLFWwindow* window, int width, int height);
	void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Initialize the WindowSystem (Windows, Event Handlers).
	// Params:
	//   settings = The settings to use for the window.
	WindowSystem::WindowSystem()
		: winExists(false), glfwWindow(nullptr), fullscreen(false),
		windowPositionX(0), windowPositionY(0)
	{
		// Windows COM Initialize
		/*HRESULT result = CoInitialize(nullptr);
		if (result < 0)
		{
			std::cout << "ERROR: Failed to initialize Windows COM!" << std::endl;
		}*/

		// Set GLFW error callback
		glfwSetErrorCallback(ErrorCallback);

		// Attempt to initialize GLFW
		if (!glfwInit())
		{
			std::cout << "ERROR: Failed to initialize GLFW!" << std::endl;
		}
	}

	// Shutdown the WindowSystem (Windows, Event Handlers).
	WindowSystem::~WindowSystem()
	{
		// Destroy the window
		glfwDestroyWindow(glfwWindow);

		// Terminate GLFW
		glfwTerminate();

		// Windows COM Shutdown
		//CoUninitialize();
	}

	// Initialize the WindowSystem (Windows, Event Handlers).
	// Params:
	//   settings = The settings to use for the window.
	void WindowSystem::Initialize(const StartupSettings& settings)
	{
		systemInfo = settings;

		// Creates the window and console
		InitHelper();

		// Change the window title.
		SetWindowTitle("Beta Engine " + BE_LL_VERSION);
	}

	// End the frame and draw
	void WindowSystem::Draw()
	{
		// Handle messages
		// handle windows message
		// * do it outside the frame start => frame end so that it will not effect
		//   the game loop timing.
		glfwPollEvents();

		// Swap the frame buffers
		glfwSwapBuffers(glfwWindow);
	}

	// Sets the title of the window being used by Beta Framework.
	// Params:
	//   text = The string that the window title will be set to.
	void WindowSystem::SetWindowTitle(const std::string & text)
	{
		glfwSetWindowTitle(glfwWindow, text.c_str());
	}

	// Does the window for this program still exist?
	bool WindowSystem::DoesWindowExist() const
	{
		return winExists;
	}

	// Test whether the application is in fullscreen mode.
	bool WindowSystem::IsFullScreen() const
	{
		return fullscreen;
	}

	// Sets whether the window is fullscreen.
	void WindowSystem::SetFullScreen(bool fullscreen_)
	{
		// Get window width height
		int width, height;
		glfwGetWindowSize(glfwWindow, &width, &height);

		// Full screen
		if (fullscreen_ && !fullscreen)
		{
			// Save the window position
			glfwGetWindowPos(glfwWindow, &windowPositionX, &windowPositionY);

			// Get resolution of monitor
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();

			// Get primary monitor size
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			float aspectRatio = static_cast<float>(mode->width) / static_cast<float>(mode->height);

			// Set window size, preserving aspect ratio
			width = static_cast<int>(static_cast<float>(height) * aspectRatio);
			glfwSetWindowMonitor(glfwWindow, monitor, 0, 0, width, height, 0);
		}
		// Windowed mode
		else if (!fullscreen_ && fullscreen)
		{
			// restore last window size and position
			glfwSetWindowMonitor(glfwWindow, nullptr, windowPositionX, windowPositionY, width, height, 0);
		}

		fullscreen = fullscreen_;
	}

	// Sets the resolution of the window.
	// Params:
	//   width = The new width of the window.
	//   height = The new height of the window.
	void WindowSystem::SetResolution(unsigned width, unsigned height)
	{
		if (fullscreen)
		{
			// Get reolution of monitor
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();

			// Get primary monitor size
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			float aspectRatio = static_cast<float>(mode->width) / static_cast<float>(mode->height);

			// Set window size, preserving aspect ratio
			width = static_cast<int>(static_cast<float>(height) * aspectRatio);
			glfwSetWindowSize(glfwWindow, width, height);
		}
		else
		{
			glfwSetWindowSize(glfwWindow, width, height);
		}

		systemInfo.windowWidth = width;
		systemInfo.windowHeight = height;
	}

	Vector2D WindowSystem::GetResolution() const
	{
		return Vector2D(static_cast<float>(systemInfo.windowWidth), static_cast<float>(systemInfo.windowHeight));
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Initializes the Beta systems. Uses the specified options to initialize Beta and
	// the functionality handled by Beta.
	void WindowSystem::InitHelper()
	{
#if !_DEBUG
		systemInfo.debugConsole = false;
#endif

		FILE* file;
		if (systemInfo.debugConsole)
		{
			/*if (AllocConsole())
			{
				file = freopen("CONOUT$", "wt", stdout);
				file = freopen("CONOUT$", "wt", stderr);
				file = freopen("CONOUT$", "wt", stdin);

				SetConsoleTitle("Beta Framework - Debug Console");
			}*/
		}
#ifdef _DEBUG
		else
		{
			file = freopen("trace.log", "wt", stdout);
			file = freopen("trace.log", "wt", stderr);
			file = freopen("trace.log", "wt", stdin);
		}
#endif

		// Print engine info to console
		std::cout << "Beta Framework Low-Level API version: " << BE_LL_VERSION << std::endl << std::endl;

		// Window creation
		CreateWindowGLFW();

		// Window currently exists
		winExists = true;
	}

	// Create window using GLFW functions
	void WindowSystem::CreateWindowGLFW()
	{
		// Create a window
#if BE_GL_4_3_API
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
		glfwWindowHint(GLFW_RESIZABLE, 0);

		// Use native resolution if viewport width/height set to 0
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		systemInfo.windowWidth = systemInfo.windowWidth == 0 ? mode->width : systemInfo.windowWidth;
		systemInfo.windowHeight = systemInfo.windowHeight == 0 ? mode->height : systemInfo.windowHeight;

		// Use monitor when full screen
		GLFWmonitor* monitor = systemInfo.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
		glfwWindow = glfwCreateWindow(systemInfo.windowWidth, systemInfo.windowHeight, "Beta Engine", monitor, nullptr);

		if (!glfwWindow)
		{
			// Window or OpenGL context creation failed
			return;
		}

		// Set up callbacks
		glfwSetWindowCloseCallback(glfwWindow, WinCloseCallback);
		glfwSetKeyCallback(glfwWindow, KeyCallback);
		glfwSetFramebufferSizeCallback(glfwWindow, ViewportChangeCallback);
		glfwSetCursorPosCallback(glfwWindow, CursorPositionCallback);
		glfwSetScrollCallback(glfwWindow, MouseScrollCallback);

		if (systemInfo.allowMaximize)
		{
			// Ugly non-portable windows code, but it's the best we can do
			// (AFAIK) if we want maximize but not drag to resize
			/*HWND windowHandle = GetActiveWindow();
			long Style = GetWindowLong(windowHandle, GWL_STYLE);
			Style |= WS_MAXIMIZEBOX;
			SetWindowLong(windowHandle, GWL_STYLE, Style);*/
		}

		// Show the window
		if (systemInfo.showWindow)
		{
			glfwShowWindow(glfwWindow);
		}
		else
		{
			glfwHideWindow(glfwWindow);
		}

		// Make the OpenGL context current
		glfwMakeContextCurrent(glfwWindow);
	}

	//------------------------------------------------------------------------------
	// Private Helper Functions:
	//------------------------------------------------------------------------------

	void ErrorCallback(int error, const char* description)
	{
		std::cerr << "Error " << error << ": " << description << std::endl;
	}

	void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		UNREFERENCED_PARAMETER(window);
		UNREFERENCED_PARAMETER(scancode);

		// Alt-Enter - Toggle Fullscreen
		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && mods == GLFW_MOD_ALT)
		{
			WindowSystem* system = EngineGetModule(WindowSystem);

			system->SetFullScreen(!system->IsFullScreen());
		}
	}

	void WinCloseCallback(GLFWwindow * window)
	{
		UNREFERENCED_PARAMETER(window);
		EngineGetModule(WindowSystem)->winExists = false;
	}

	void ViewportChangeCallback(GLFWwindow * window, int width, int height)
	{
		UNREFERENCED_PARAMETER(window);
		EngineGetModule(GraphicsEngine)->SetViewport(width, height);
	}

	void CursorPositionCallback(GLFWwindow * window, double xpos, double ypos)
	{
		UNREFERENCED_PARAMETER(window);
		EngineGetModule(Input)->
			SetCursorPosition(Vector2D(static_cast<float>(xpos), static_cast<float>(ypos)));
	}

	void MouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
	{
		UNREFERENCED_PARAMETER(window);
		UNREFERENCED_PARAMETER(xoffset);
		EngineGetModule(Input)->
			SetMouseScrollAmount(static_cast<float>(yoffset));
	}
}
