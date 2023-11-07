//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <cstring>
#include "stdafx.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	static void SwapFloat(float& first, float& second);

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Matrix2D::Matrix2D()
	{
		memset(this, 0, sizeof(Matrix2D));
	}

	// This function sets the matrix equal to the identity matrix.
	Matrix2D Matrix2D::IdentityMatrix()
	{
		Matrix2D identity;
		identity.m[0][0] = 1;
		identity.m[1][1] = 1;
		identity.m[2][2] = 1;
		return identity;
	}

	// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
	// (NOTE: Care must be taken when pResult = pMtx.)
	Matrix2D Matrix2D::Transposed() const
	{
		Matrix2D transposed = *this;
		SwapFloat(transposed.m[0][1], transposed.m[1][0]);
		SwapFloat(transposed.m[0][2], transposed.m[2][0]);
		SwapFloat(transposed.m[1][2], transposed.m[2][1]);
		return transposed;
	}

	// This function creates a translation matrix from x & y and saves it in Result.
	Matrix2D Matrix2D::TranslationMatrix(float x, float y)
	{
		Matrix2D translate = IdentityMatrix();
		translate.m[0][2] = x;
		translate.m[1][2] = y;
		return translate;
	}

	// This function creates a scaling matrix from x & y and saves it in Result.
	Matrix2D Matrix2D::ScalingMatrix(float x, float y)
	{
		Matrix2D scale = IdentityMatrix();
		scale.m[0][0] = x;
		scale.m[1][1] = y;
		return scale;
	}

	// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
	// Save the resultant matrix in Result.
	// Converting from degrees to radians can be performed as follows:
	//	 radians = (angle * M_PI) / 180.0f
	// M_PI is defined in "math.h", which may be included as follows:
	//   #define _USE_MATH_DEFINES
	//   #include <math.h>
	Matrix2D Matrix2D::RotationMatrixDegrees(float angle)
	{
		return RotationMatrixRadians((angle * (float)M_PI) / 180.0f);
	}

	// This matrix creates a rotation matrix from "Angle" whose value is in radians.
	// Save the resultant matrix in Result.
	Matrix2D Matrix2D::RotationMatrixRadians(float angle)
	{
		Matrix2D rotation = IdentityMatrix();
		rotation.m[0][0] = cosf(angle); rotation.m[0][1] = -sinf(angle);
		rotation.m[1][0] = sinf(angle); rotation.m[1][1] = cosf(angle);
		return rotation;
	}

	// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
	// Result = Mtx0*Mtx1
	// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
	Matrix2D Matrix2D::operator*(const Matrix2D & other) const
	{
		Matrix2D temp;

		// Multiply matrices
		temp.m[0][0] = RowColumnMultiply(other, 0, 0);
		temp.m[0][1] = RowColumnMultiply(other, 0, 1);
		temp.m[0][2] = RowColumnMultiply(other, 0, 2);
		temp.m[1][0] = RowColumnMultiply(other, 1, 0);
		temp.m[1][1] = RowColumnMultiply(other, 1, 1);
		temp.m[1][2] = RowColumnMultiply(other, 1, 2);
		temp.m[2][0] = RowColumnMultiply(other, 2, 0);
		temp.m[2][1] = RowColumnMultiply(other, 2, 1);
		temp.m[2][2] = RowColumnMultiply(other, 2, 2);

		return temp;
	}

	// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
	// Result = Mtx0*Mtx1
	// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
	Matrix2D& Matrix2D::operator*=(const Matrix2D & other)
	{
		// Creating a temporary to avoid accidentally changing
		// the first matrix half way through the multiplication.
		Matrix2D temp(*this * other);
		*this = temp;
		return *this;
	}

	// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
	// Result = Mtx * Vec.
	// (NOTE: Care must be taken when pResult = pVec.)
	Vector2D Matrix2D::operator*(const Vector2D & vec) const
	{
		// Only create temporary Matrix if necessary
		Vector2D temp;

		// Multiply vector by matrix
		temp.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * 1;
		temp.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * 1;

		return temp;
	}

	//------------------------------------------------------------------------------
	// Global Functions:
	//------------------------------------------------------------------------------

	// Prints the contents of the matrix to an output stream.
	std::ostream& operator<<(std::ostream & os, const Matrix2D & matrix)
	{
		for (unsigned row = 0; row < 3; ++row)
			std::cout << "[" << matrix.m[row][0] << " " << matrix.m[row][1] << " " << matrix.m[row][2] << "]" << std::endl;
		std::cout << std::endl;
		return os;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	float Matrix2D::RowColumnMultiply(const Matrix2D & other, unsigned row, unsigned col) const
	{
		return m[row][0] * other.m[0][col] + m[row][1] * other.m[1][col] + m[row][2] * other.m[2][col];
	}

	void SwapFloat(float& first, float& second)
	{
		float temp = first;
		first = second;
		second = temp;
	}
}
