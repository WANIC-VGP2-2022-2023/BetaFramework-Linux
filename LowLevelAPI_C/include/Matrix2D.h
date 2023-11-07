//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Frequently, vector and matrix structures are declared publicly.  While an
// interface is typically provided with these structures, these are two common
// exceptions to the coding best practice of "hiding information".
typedef struct Matrix2D
{
	// The matrix is stored in row-major format, meaning the consecutive
	//   elements of rows are contiguous.
	//   m[row][column]
	float m[3][3];
}Matrix2D;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for matrix. Sets all data in matrix to 0.
BE_C_API Matrix2D Matrix2DZero();

// This function creates an identity matrix and returns it.
BE_C_API Matrix2D Matrix2DIdentity();

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
BE_C_API Matrix2D Matrix2DTransposed(const Matrix2D* mtx);

// This function creates a translation matrix from x & y and returns it.
BE_C_API Matrix2D Matrix2DTranslation(float x, float y);

// This function creates a scaling matrix from x & y and returns it.
BE_C_API Matrix2D Matrix2DScaling(float x, float y);

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// Return the resulting matrix.
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
BE_C_API Matrix2D Matrix2DRotationDegrees(float angle);

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// Return the resulting matrix.
BE_C_API Matrix2D Matrix2DRotationRadians(float angle);

// This function multiplies the current matrix with another and returns the result as a new matrix.
BE_C_API Matrix2D Matrix2DMultiply(const Matrix2D* m0, const Matrix2D* m1);

// This function multiplies the current matrix with another, storing the result in the current matrix.
BE_C_API void Matrix2DMultiplyAssign(Matrix2D* m0, const Matrix2D* m1);

// This function multiplies a matrix with a vector and returns the result as a new vector.
// Result = Mtx * Vec.
BE_C_API Vector2D Matrix2DMultiplyVector2D(const Matrix2D* mtx, const Vector2D* vec);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
