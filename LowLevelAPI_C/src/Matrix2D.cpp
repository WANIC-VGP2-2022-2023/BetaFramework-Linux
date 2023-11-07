//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <cstring>
#include "stdafx.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

// Multiplies a row of the current matrix by a column in another and returns the result.
float RowColumnMultiply(const Matrix2D* m0, const Matrix2D* m1, unsigned row, unsigned col);

// Swaps two floating point values.
void SwapFloat(float& first, float& second);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for matrix. Sets all data in matrix to 0.
Matrix2D Matrix2DZero()
{
	Matrix2D mtx;
	std::memset(&mtx, 0, sizeof(Matrix2D));
	return mtx;
}

// This function creates an identity matrix and returns it.
Matrix2D Matrix2DIdentity()
{
	Matrix2D identity = Matrix2DZero();
	identity.m[0][0] = 1;
	identity.m[1][1] = 1;
	identity.m[2][2] = 1;
	return identity;
}

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
Matrix2D Matrix2DTransposed(const Matrix2D* mtx)
{
	Matrix2D transposed = *mtx;
	SwapFloat(transposed.m[0][1], transposed.m[1][0]);
	SwapFloat(transposed.m[0][2], transposed.m[2][0]);
	SwapFloat(transposed.m[1][2], transposed.m[2][1]);
	return transposed;
}

// This function creates a translation matrix from x & y and returns it.
Matrix2D Matrix2DTranslation(float x, float y)
{
	Matrix2D translate = Matrix2DIdentity();
	translate.m[0][2] = x;
	translate.m[1][2] = y;
	return translate;
}

// This function creates a scaling matrix from x & y and returns it.
Matrix2D Matrix2DScaling(float x, float y)
{
	Matrix2D scale = Matrix2DIdentity();
	scale.m[0][0] = x;
	scale.m[1][1] = y;
	return scale;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// Return the resulting matrix.
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
Matrix2D Matrix2DRotationDegrees(float angle)
{
	return Matrix2DRotationRadians((angle * (float)M_PI) / 180.0f);
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// Return the resulting matrix.
Matrix2D Matrix2DRotationRadians(float angle)
{
	Matrix2D rotation = Matrix2DIdentity();
	rotation.m[0][0] = cosf(angle); rotation.m[0][1] = -sinf(angle);
	rotation.m[1][0] = sinf(angle); rotation.m[1][1] = cosf(angle);
	return rotation;
}

// This function multiplies the current matrix with another and returns the result as a new matrix.
Matrix2D Matrix2DMultiply(const Matrix2D* m0, const Matrix2D* m1)
{
	Matrix2D temp;

	// Multiply matrices
	temp.m[0][0] = RowColumnMultiply(m0, m1, 0, 0);
	temp.m[0][1] = RowColumnMultiply(m0, m1, 0, 1);
	temp.m[0][2] = RowColumnMultiply(m0, m1, 0, 2);
	temp.m[1][0] = RowColumnMultiply(m0, m1, 1, 0);
	temp.m[1][1] = RowColumnMultiply(m0, m1, 1, 1);
	temp.m[1][2] = RowColumnMultiply(m0, m1, 1, 2);
	temp.m[2][0] = RowColumnMultiply(m0, m1, 2, 0);
	temp.m[2][1] = RowColumnMultiply(m0, m1, 2, 1);
	temp.m[2][2] = RowColumnMultiply(m0, m1, 2, 2);

	return temp;
}

// This function multiplies the current matrix with another, storing the result in the current matrix.
void Matrix2DMultiplyAssign(Matrix2D* m0, const Matrix2D* m1)
{
	// Creating a temporary to avoid accidentally changing
	// the first matrix half way through the multiplication.
	Matrix2D temp = Matrix2DMultiply(m0, m1);
	*m0 = temp;
}

// This function multiplies a matrix with a vector and returns the result as a new vector.
// Result = Mtx * Vec.
Vector2D Matrix2DMultiplyVector2D(const Matrix2D* mtx, const Vector2D* vec)
{
	// Only create temporary Matrix if necessary
	Vector2D temp;

	// Multiply vector by matrix
	temp.x = mtx->m[0][0] * vec->x + mtx->m[0][1] * vec->y + mtx->m[0][2] * 1;
	temp.y = mtx->m[1][0] * vec->x + mtx->m[1][1] * vec->y + mtx->m[1][2] * 1;

	return temp;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Implementations:
//------------------------------------------------------------------------------

// Multiplies a row of the current matrix by a column in another and returns the result.
float RowColumnMultiply(const Matrix2D* m0, const Matrix2D* m1, unsigned row, unsigned col)
{
	return m0->m[row][0] * m1->m[0][col] + m0->m[row][1] * m1->m[1][col] + m0->m[row][2] * m1->m[2][col];
}

void SwapFloat(float& first, float& second)
{
	float temp = first;
	first = second;
	second = temp;
}
