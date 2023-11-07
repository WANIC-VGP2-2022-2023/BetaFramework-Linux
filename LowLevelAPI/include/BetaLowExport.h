//------------------------------------------------------------------------------
//
// File Name:	BetaLowExport.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

// Easier declarations for DLL import/export
#ifndef BE_API
	#ifdef _BE_EXPORT
		#define BE_API __declspec(dllexport)
	#else
		#define BE_API //__declspec(dllimport)
        #define BE_C_API
	#endif
#endif
