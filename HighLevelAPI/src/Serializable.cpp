//------------------------------------------------------------------------------
//
// File Name:	Serializable.cpp
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
#include "Serializable.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Loads object data from a file.
	void Serializable::Deserialize(FileStream& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	// Saves object data to a file.
	void Serializable::Serialize(FileStream& stream) const
	{
		UNREFERENCED_PARAMETER(stream);
	}

	Serializable::Serializable()
	{
	}

	Serializable::~Serializable()
	{
	}

	Serializable::Serializable(const Serializable&)
	{
	}

	Serializable& Serializable::operator=(const Serializable&)
	{
		return *this;
	}
}

//------------------------------------------------------------------------------
