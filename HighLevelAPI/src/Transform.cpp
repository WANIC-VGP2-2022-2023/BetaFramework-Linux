//------------------------------------------------------------------------------
//
// File Name:	Transform.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"

// Math
#include <Vector2D.h>
#include <Matrix2D.h>
#include "Intersection2D.h"

// Systems
#include <GraphicsEngine.h>
#include "FileStream.h"
#include "Space.h"

#include "GameObject.h" // GetName

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new transform object.
	// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
	// Params:
	//	 x = Initial world position on the x-axis.
	//	 y = Initial world position on the y-axis.
	Transform::Transform(float x, float y)
		: Component("Transform"), translation(x, y), scale(1.0f, 1.0f),
		rotation(0.0f), isDirty(true)
	{
	}

	Transform::Transform(Vector2D translation, Vector2D scale, float rotation)
		: Component("Transform"), translation(translation), scale(scale),
		rotation(rotation), isDirty(true)
	{
	}

	// Get the transform matrix, based upon translation, rotation and scale settings.
	// Returns:
	//	 A reference to the transform's matrix.
	const Matrix2D& Transform::GetMatrix() const
	{
		CalculateMatrices();
		return matrix;
	}

	// Get the inverse of the transform matrix.
	// Returns:
	//	 A reference to the transform's inverse matrix.
	const Matrix2D& Transform::GetInverseMatrix() const
	{
		CalculateMatrices();
		return inverseMatrix;
	}

	// Set the translation of a transform component.
	// Params:
	//	 transform = Pointer to the transform component.
	//	 translation = Pointer to a translation vector.
	void Transform::SetTranslation(const Vector2D& translation_)
	{
		if (!AlmostEqual(translation, translation_))
			isDirty = true;

		translation = translation_;
	}

	void Transform::SetTranslationX(float x)
	{
		if (!AlmostEqual(x, translation.x))
			isDirty = true;

		translation.x = x;
	}

	void Transform::SetTranslationY(float y)
	{
		if (!AlmostEqual(y, translation.y))
			isDirty = true;

		translation.y = y;
	}

	// Get the translation of a transform component.
	// Params:
	//	 transform = Pointer to the transform object.
	// Returns:
	//	 
	//		A reference to the component's translation structure,
	//		
	const Vector2D& Transform::GetTranslation() const
	{
		return translation;
	}

	// Set the rotation of a transform component.
	// Params:
	//	 transform = Pointer to the transform component.
	//	 rotation = The rotation value (in radians).
	void Transform::SetRotation(float rotation_)
	{
		if (!AlmostEqual(rotation, rotation_))
			isDirty = true;

		rotation = rotation_;
	}

	// Get the rotation value of a transform component.
	// Params:
	//	 transform = Pointer to the transform object.
	// Returns:
	//		The component's rotation value (in radians).
	float Transform::GetRotation() const
	{
		return rotation;
	}

	// Set the scale of a transform component.
	// Params:
	//	 transform = Pointer to the transform component.
	//	 translation = Pointer to a scale vector.
	void Transform::SetScale(const Vector2D& scale_)
	{
		if (!AlmostEqual(scale, scale_))
			isDirty = true;

		scale = scale_;
	}

	void Transform::SetScaleX(float x)
	{
		if (!AlmostEqual(x, scale.x))
			isDirty = true;

		scale.x = x;
	}

	void Transform::SetScaleY(float y)
	{
		if (!AlmostEqual(y, scale.y))
			isDirty = true;

		scale.y = y;
	}

	// Get the scale of a transform component.
	// Params:
	//	 transform = Pointer to the transform object.
	// Returns:
	//   A reference to the component's scale structure,	
	const Vector2D& Transform::GetScale() const
	{
		return scale;
	}

	// Returns a bounding rectangle using translation and (half) scale.
	const BoundingRectangle Transform::GetBounds() const
	{
		Vector2D extents = scale / 2.0f;
		extents.x = fabsf(extents.x);
		extents.y = fabsf(extents.y);

		return BoundingRectangle(translation, extents);
	}

	// Tests whether the object is visible on screen.
	bool Transform::IsOnScreen() const
	{
		const BoundingRectangle& object = GetBounds();
		const BoundingRectangle& screen = GetOwner()->GetSpace()->GetCamera().GetScreenWorldDimensions();

		return Intersection2D::RectangleRectangleIntersection(object, screen);
	}

	// Save object data to file.
	// Params:
	//   stream = The stream object used to save the object's data.
	void Transform::Serialize(FileStream& stream) const
	{
		stream.WriteVariable("translation", translation);
		stream.WriteVariable("rotation", rotation);
		stream.WriteVariable("scale", scale);
	}

	// Load object data from file
	// Params:
	//   stream = The stream object used to load the object's data.
	void Transform::Deserialize(FileStream& stream)
	{
		stream.ReadVariable("translation", translation);
		stream.ReadVariable("rotation", rotation);
		stream.ReadVariable("scale", scale);
	}

	// Calculates the transform matrix and its inverse using translation, rotation, and scale.
	void Transform::CalculateMatrices() const
	{
		if (isDirty)
		{
			// TRANSFORM
			Matrix2D scaling = Matrix2D::ScalingMatrix(scale.x, scale.y);
			Matrix2D rot = Matrix2D::RotationMatrixRadians(rotation);
			Matrix2D trans = Matrix2D::TranslationMatrix(translation.x, translation.y);

			// Scaling is applied first, then rotation, then translation
			matrix = trans * rot * scaling;

			// INVERSE
			float inverseScaleX = (scale.x != 0.0f) ? (1.0f / scale.x) : 0.0f;
			float inverseScaleY = (scale.y != 0.0f) ? (1.0f / scale.y) : 0.0f;

			scaling = Matrix2D::ScalingMatrix(inverseScaleX, inverseScaleY);
			rot = Matrix2D::RotationMatrixRadians(-rotation);
			trans = Matrix2D::TranslationMatrix(-translation.x, -translation.y);

			// Reverse order for inverse
			inverseMatrix = scaling * rot * trans;
		}

		isDirty = false;
	}

	// RTTI
	COMPONENT_SUBCLASS_DEFINITION(Transform)
}
