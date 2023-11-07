//------------------------------------------------------------------------------
//
// File Name:	FileStream.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "FileStream.h"

#include "Vector2D.h"
#include <assert.h>

//------------------------------------------------------------------------------

namespace Beta
{
	namespace Serialization
	{
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Opens a file for loading.
		// Returns:
		//   True if load was successful, false otherwise.
		FileStream::FileStream(const std::string& filename, std::ios_base::openmode mode)
			: stream(filename, mode), filename(filename), indentLevel(0)
		{
		}

		// Closes the currently open file.
		FileStream::~FileStream()
		{
			stream.close();
		}

		// Begins a new scope when writing to a file.
		// Outputs a curly brace and increases the tab count.
		void FileStream::BeginScope()
		{
			CheckFileOpen();

			for (unsigned i = 0; i < indentLevel; ++i)
				stream << tab;

			stream << "{" << std::endl;
			indentLevel++;
		}

		// Ends the current scope when writing to a file.
		// Outputs an end curly brace and decreases tab count.
		void FileStream::EndScope()
		{
			CheckFileOpen();

			indentLevel--;

			for (unsigned i = 0; i < indentLevel; ++i)
				stream << tab;

			stream << "}" << std::endl;
		}

		// Reads a piece of text from the currently open file
		// and skips to the next word afterwards.
		void FileStream::ReadSkip(const std::string& text)
		{
			CheckFileOpen();

			std::string nextWord;

			stream >> nextWord;

			if (nextWord != text)
				throw FileStreamException(filename, "Could not find variable " + text + " in file.\n");
		}

		// Skips characters in the stream up until the next
		// occurrence of the given delimiter.
		void FileStream::ReadSkip(char delimiter)
		{
			CheckFileOpen();
			stream.ignore(10, delimiter);
		}

		// Checks if the file was opened correctly. If not, throws an exception.
		void FileStream::CheckFileOpen()
		{
			if (!stream.is_open())
			{
				throw FileStreamException(filename, "Could not open specified file.");
			}
		}

		// Exception constructor
		FileStreamException::FileStreamException(const std::string& fileName, const std::string& errorDetails)
			: exception(("Error reading file " + fileName + ". " + errorDetails).c_str())
		{
		}
	}
}
