//------------------------------------------------------------------------------
//
// File Name:	Stream.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "Stream.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

// Array for storing “tokens” (single words).
static char tokenBuffer[1024];

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Open a file for reading.
// (NOTE: Use fopen_s() with the "rt" option.)
// (NOTE: You must generate the following trace message if the file was not opened successfully:
//	  TraceMessage("Error: StreamOpen could not open file %s; %s", fileName, errorMsg);
//	  Where errorMsg is a char array containing the error message obtained using strerror_s().)
// Params:
//	 filename = Name of the file to be opened for reading.
// Returns:
//	 NULL if the file name is null or the file could not be opened.
//	 Pointer to a FILE object if the file was opened successfully.
Stream StreamOpen(const char * fileName)
{
	Stream file = NULL;

	if (fileName)
	{
		errno_t error = fopen_s(&file, fileName, "rt");
		if (error)
		{
			char errorMsg[1024];
			strerror_s(errorMsg, _countof(errorMsg), error);
			printf("Error: StreamOpen could not open file %s; %s\n", fileName, errorMsg);
		}
	}
	return file;
}

// Read a single boolean value from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for a float.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return a boolean value read from the file,
//	   else return 0.
bool StreamReadBoolean(Stream stream)
{
	bool result = false;
	if (stream)
	{
		int value;
		fscanf_s(stream, "%d", &value);
		result = (value != 0);
	}
	return result;
}

// Read a single hexadecimal integer from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an hexadecimal integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return an unsigned integer value read from the file,
//	   else return 0.
unsigned StreamReadHex(Stream stream)
{
	unsigned result = 0;

	if (stream)
	{
		fscanf_s(stream, "%x", &result);
	}
	return result;
}

// Read a single integer from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return an integer value read from the file,
//	   else return 0.
int StreamReadInt(Stream stream)
{
	int result = 0;

	if (stream)
	{
		fscanf_s(stream, "%d", &result);
	}
	return result;
}

// Read a single float value from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for a float.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return a float value read from the file,
//	   else return 0.
float StreamReadFloat(Stream stream)
{
	float result = 0;

	if (stream)
	{
		fscanf_s(stream, "%f", &result);
	}
	return result;
}

// Read the data for a Vector2D from a stream.
// (NOTE: Verify that the stream and vector pointer are valid first.)
// (HINT: Use StreamReadFloat() to read the x and y values, in sequence.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream and vector pointer are both valid,
//	   then fill the vector with two float values (x & y),
//	   else do nothing (optionally, write an error message to the trace log).
void StreamReadVector2D(Stream stream, Vector2D * vector)
{
	if (stream && vector)
	{
		vector->x = StreamReadFloat(stream);
		vector->y = StreamReadFloat(stream);
	}
}

// Read a token (a single word) from a file.
// Suggested steps:
//	 - Set the first value in tokenBuffer to 0
//	 - If the stream was opened successfully,
//	   - Read a string ("%s") into tokenBuffer using fscanf_s()
//	 - Return tokenBuffer
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 Pointer to tokenBuffer
const char * StreamReadToken(Stream stream)
{
	tokenBuffer[0] = '\0';
	if (stream)
	{
		fscanf_s(stream, "%s", tokenBuffer, (unsigned)(_countof(tokenBuffer)));
	}
	return tokenBuffer;
}

// Close an opened stream.
// (NOTE: Do not attempt to close the stream if the pointer is null.)
// Params:
//	 stream = The file stream to be closed.
void StreamClose(Stream * stream)
{
	if (stream)
	{
		fclose(*stream);
		*stream = 0;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

