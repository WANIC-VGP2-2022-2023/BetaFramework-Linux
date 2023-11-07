//------------------------------------------------------------------------------
//
// File Name:	Shader.cpp
// Author(s):	Jeremy Kings
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2018-2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ShaderProgram.h"

// Libraries
#include <glad.h>
#include "../../glfw/src/glfw3.h"
#include "glm_include.h"
#include <sstream>

// Math
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix2D.h"
#include "Matrix3D.h"
#include "Color.h"

// Systems
#include "EngineCore.h"	  // GetFilePath
#include "GraphicsEngine.h" // IsInitialized

#include "Array.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

#if _DEBUG
	static Array<std::string> uniformErrorLog;
	static Array<std::string> attributeErrorLog;
#endif

	std::string ShaderProgram::shaderPath = "Shaders/";

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	ShaderProgram::~ShaderProgram()
	{
		unsigned returnedShaders[2];
		int shaderCount = 0;

		memset(returnedShaders, 0, 2 * sizeof(GLuint));

		glGetAttachedShaders(id,
			2,
			&shaderCount,
			returnedShaders);

		for (int shaderIdx = 0; shaderIdx < shaderCount; ++shaderIdx)
			glDeleteShader(returnedShaders[shaderIdx]);

		glDeleteProgram(id);
	}

	void ShaderProgram::Use() const
	{
		glUseProgram(id);
	}

	void ShaderProgram::SetUniform(const std::string & name, int value) const
	{
		int location = GetUniformLocation(name);

#if BE_GL_4_3_API
		glProgramUniform1i(id, location, value);
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform1i(location, value);
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, float value) const
	{
		int location = GetUniformLocation(name);
#if BE_GL_4_3_API
		glProgramUniform1f(id, location, value);
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform1f(location, value);
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, bool value) const
	{
		int location = GetUniformLocation(name);
#if BE_GL_4_3_API
		glProgramUniform1i(id, location, value);
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform1i(location, value);
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Vector2D & value) const
	{
		int location = GetUniformLocation(name);
#if BE_GL_4_3_API
		glProgramUniform2f(id, location, value.x, value.y);
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform2f(location, value.x, value.y);
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Vector3D & value) const
	{
		int location = GetUniformLocation(name);
		glm::vec3& v = static_cast<glm::vec3&>(*static_cast<glm::vec3*>(value.data));
#if BE_GL_4_3_API
		glProgramUniform3f(id, location, v.x, v.y, v.z);
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform3f(location, v.x, v.y, v.z);
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Color & value) const
	{
		int location = GetUniformLocation(name);
#if BE_GL_4_3_API
		glProgramUniform4f(id, location, value.r, value.g, value.b, value.a);
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform4f(location, value.r, value.g, value.b, value.a);
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Matrix2D & transform) const
	{
		int location = GetUniformLocation(name);
#if BE_GL_4_3_API
		glProgramUniformMatrix3fv(id, location, 1, GL_FALSE, &transform.m[0][0]);
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniformMatrix3fv(location, 1, GL_TRUE, &transform.m[0][0]);
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Matrix3D & transform) const
	{
		int location = GetUniformLocation(name);
		glm::mat4& matrix = static_cast<glm::mat4&>(*static_cast<glm::mat4*>(transform.data));
#if BE_GL_4_3_API
		glProgramUniformMatrix4fv(id, location, 1, GL_FALSE, glm::value_ptr(matrix));
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Array<int> & values) const
	{
		int location = GetUniformLocation(name);
#if BE_GL_4_3_API
		glProgramUniform1iv(id, location, static_cast<GLsizei>(values.Size()), values.Data());
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform1iv(location, static_cast<GLsizei>(values.Size()), values.Data());
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Array<float> & values) const
	{
		int location = GetUniformLocation(name);
#if BE_GL_4_3_API
		glProgramUniform1fv(id, location, static_cast<GLsizei>(values.Size()), values.Data());
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform1fv(location, static_cast<GLsizei>(values.Size()), values.Data());
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Array<Vector2D> & values) const
	{
		int location = GetUniformLocation(name);
		Array<float> floatValues;
		size_t size = values.Size();
		floatValues.Reserve(size * 2);
		for (size_t i = 0; i < size; ++i)
		{
			floatValues.PushBack(values[i].x);
			floatValues.PushBack(values[i].y);
		}
#if BE_GL_4_3_API
		glProgramUniform2fv(id, location, static_cast<GLsizei>(values.Size()), floatValues.Data());
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform2fv(location, static_cast<GLsizei>(values.Size()), floatValues.Data());
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Array<Vector3D> & values) const
	{
		int location = GetUniformLocation(name);
		Array<float> floatValues;
		size_t size = values.Size();
		floatValues.Reserve(size * 3);
		for (size_t i = 0; i < size; ++i)
		{
			const glm::vec3& v = *static_cast<const glm::vec3*>(values[i].data);
			floatValues.PushBack(v.x);
			floatValues.PushBack(v.y);
			floatValues.PushBack(v.z);
		}
#if BE_GL_4_3_API
		glProgramUniform3fv(id, location, static_cast<GLsizei>(values.Size()), floatValues.Data());
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform3fv(location, static_cast<GLsizei>(values.Size()), floatValues.Data());
		glUseProgram(lastProgram);
#endif
	}

	void ShaderProgram::SetUniform(const std::string & name, const Array<Color> & values) const
	{
		int location = GetUniformLocation(name);
		Array<float> floatValues;
		size_t size = values.Size();
		floatValues.Reserve(size * 4);
		for (size_t i = 0; i < size; ++i)
		{
			floatValues.PushBack(values[i].r);
			floatValues.PushBack(values[i].g);
			floatValues.PushBack(values[i].b);
			floatValues.PushBack(values[i].a);
		}
#if BE_GL_4_3_API
		glProgramUniform4fv(id, location, static_cast<GLsizei>(values.Size()), floatValues.Data());
#else
		int lastProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		Use();
		glUniform4fv(location, static_cast<GLsizei>(values.Size()), floatValues.Data());
		glUseProgram(lastProgram);
#endif
	}

	bool ShaderProgram::operator==(const ShaderProgram & other) const
	{
		return (vertexShader == other.vertexShader && pixelShader == other.pixelShader);
	}

	ShaderProgram* ShaderProgram::CreateProgramFromFile(const std::string & vertexShaderFile, const std::string & fragmentShaderFile)
	{

		// Don't load shader if graphics is not initialized
		if (EngineGetModule(GraphicsEngine) == nullptr)
		{
			std::cout << "Error loading shader " << vertexShaderFile << ", " << fragmentShaderFile << "; graphics system not "
				<< "yet initialized." << std::endl;
			return nullptr;
		}

		// Get id for program
		GLuint id = glCreateProgram();

		const std::string& enginePath = EngineCore::GetInstance().GetFilePath();

		// Add vertex and fragment shaders
		bool vertSuccess = AddShaderFromFile(id, enginePath + shaderPath + vertexShaderFile, GL_VERTEX_SHADER);
		bool fragSuccess = AddShaderFromFile(id, enginePath + shaderPath + fragmentShaderFile, GL_FRAGMENT_SHADER);

		// Attempt to link
		glLinkProgram(id);

		// Print log info
		std::cout << "Shader program " << id << ": " << vertexShaderFile << ", " << fragmentShaderFile << std::endl;

		int status;
		glGetProgramiv(id, GL_LINK_STATUS, &status);
		if (status == GL_TRUE && fragSuccess && vertSuccess)
		{
			std::cout << "Shaders linked successfully." << std::endl;

			// Return completed program
			return new ShaderProgram(id, vertexShaderFile, fragmentShaderFile);
		}
		else
		{
			std::cout << "Linking unsuccessful." << std::endl;

			GLint logLength = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength != 0)
			{
				// The maxLength includes the NULL character
				Array<GLchar> errorLog(logLength);
				glGetShaderInfoLog(id, logLength, &logLength, &errorLog[0]);

				// Print log info
				std::string errorLogString(errorLog.Begin(), errorLog.End());
				std::cout << "Linker error/warning info: " << errorLogString << std::endl;
			}

			// Delete the program to avoid leaks.
			glDeleteProgram(id);

			return nullptr;
		}
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	ShaderProgram::ShaderProgram(unsigned id, const std::string & vertexShader, const std::string & pixelShader)
		: id(id), vertexShader(vertexShader), pixelShader(pixelShader)
	{
	}

	int ShaderProgram::GetUniformLocation(const std::string & name) const
	{
		int location = -1;

       // uniformLocations.;

		std::map<std::string, int>::const_iterator it = uniformLocations.find(name);

		// We've tried to locate this before
		if (it != uniformLocations.end())
			location = it->second;
		// This is the first lookup
		else
		{
			location = glGetUniformLocation(id, name.c_str());
			uniformLocations.emplace(name, location);
		}
#if _DEBUG
		if (location == -1)
		{
			// Have we sent this error recently?
			std::string fullName = name + std::to_string(id);
			if (uniformErrorLog.Find(fullName) == uniformErrorLog.End())
			{
				// If not, display it and log it
				std::cout << "GLSL ERROR: Uniform variable " << name << " not found in Shader " << id << std::endl
					<< "(Variable may have been optimized out by compiler if not used.)" << std::endl << std::endl;
				uniformErrorLog.PushBack(fullName);
			}
		}
#endif
		return location;
	}

	int ShaderProgram::GetAttributeLocation(const std::string & name) const
	{
		int location = -1;
		std::map<std::string, int>::const_iterator it = attributeLocations.find(name);

		// We've tried to locate this before
		if (it != attributeLocations.end())
			location = it->second;
		// This is the first lookup
		else
		{
			location = glGetAttribLocation(id, name.c_str());
			attributeLocations.emplace(name, location);
		}
#if _DEBUG
		if (location == -1)
		{
			// Have we sent this error recently?
			std::string fullName = name + std::to_string(id);
			// Have we sent this error recently?
			if (attributeErrorLog.Find(fullName) == attributeErrorLog.End())
			{
				std::cout << "GLSL ERROR: Attribute variable " << name << " not found in Shader " << id << std::endl
					<< "(Variable may have been optimized out by compiler if not used.)" << std::endl << std::endl;
				attributeErrorLog.PushBack(fullName);
			}
		}
#endif
		return location;
	}

	bool ShaderProgram::AddShaderFromString(unsigned program, const std::string & source, int glType)
	{
		if (source == "")
		{
			std::cout << "Compilation unsuccessful - no source provided." << std::endl;
			return false;
		}

		// DEBUG INFO
		std::string shaderType;
		switch (glType)
		{
		case GL_VERTEX_SHADER:
			shaderType = "vertex shader";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "fragment shader";
			break;
		default:
			break;
		}

		// COMPILATION BEGINS
		const char* const data = source.c_str();
		unsigned id = glCreateShader(glType);
		std::cout << "Compiling shader program " << id << " " << shaderType << "..." << std::endl;
		glShaderSource(id, 1, &data, NULL);
		glCompileShader(id);
		int status;
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		if (status == GL_TRUE)
		{
			glAttachShader(program, id);
			std::cout << "Shader compiled successfully." << std::endl;
			return true;
		}
		else
		{
			std::cout << "Compilation unsuccessful." << std::endl;

			GLint logLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength != 0)
			{
				// The maxLength includes the NULL character
				Array<GLchar> errorLog(logLength);
				glGetShaderInfoLog(id, logLength, &logLength, &errorLog[0]);

				// Print log info
				std::string errorLogString(errorLog.Begin(), errorLog.End());
				std::cout << errorLogString << std::endl;
			}

			glDeleteShader(id);
			return false;
		}
	}

	bool ShaderProgram::AddShaderFromFile(unsigned program, const std::string & filename, int glType)
	{
		std::string source = ReadFromFile(filename);
		return AddShaderFromString(program, source, glType);
	}

	std::string ShaderProgram::ReadFromFile(const std::string & filename)
	{
		std::ifstream file(filename);
		std::stringstream buffer;
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				buffer << line << "\r\n";
			}
		}
		else
		{
			std::cout << "Error opening shader file " << filename
				<< ". Check that the file exists and that the specified path is correct." << std::endl;
		}
		return buffer.str();
	}
}

//------------------------------------------------------------------------------
