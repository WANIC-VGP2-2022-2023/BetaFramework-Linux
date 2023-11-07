//------------------------------------------------------------------------------
//
// File Name:	Camera.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Camera.h"

// Libraries
#include "glm_include.h" // vec3, mat4
#include <glad.h>
#include "../../glfw/src/glfw3.h" // glReadPixels

// Math
#include "Matrix2D.h"

// Systems
#include "ShaderProgram.h"	// SetUniform
#include "EngineCore.h"		// GetModule
#include "GraphicsEngine.h" // GetViewport

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Private Structures and Variables:
	//------------------------------------------------------------------------------

	// These settings were chosen so that default camera views
	// close to the same area regardless of projection used (ortho/persp).
	namespace
	{
		const float defaultFOV = 90.0f;
		const float defaultSize = 5.0f;
		const float defaultDistance = defaultSize / 2.0f;

		const float minSize = 0.1f;
		const float maxSize = 5000.0f;

		const float minFOV = 1.0f;
		const float maxFOV = 180.0f;
	}

	class Camera::Implementation
	{
	public:
		Implementation() :
			distance(defaultDistance), nearClip(0.1f), farClip(1000.0f), rotation(0.0f), fieldOfView(defaultFOV), size(defaultSize),
			viewportWidth(800), viewportHeight(600), aspectRatio(8.0f / 6.0f), isDirtyProjection(true), isDirtyView(true),
			viewMatrix(Matrix2D()), projectionMatrix(Matrix2D()), mode(PM_Orthographic) {}

		// Returns the view matrix constructed using this camera's attributes.
		const Matrix3D& GetViewMatrix() const
		{
			if (isDirtyView)
			{
				ComputeViewMatrix();
			}

			return viewMatrix;
		}

		// Returns the projection matrix constructed using this camera's attributes.
		const Matrix3D& GetProjectionMatrix() const
		{
			// Grab viewport from Graphics
			Vector2D viewport = EngineGetModule(GraphicsEngine)->GetViewport();

			if (!(viewport.x == viewportWidth && viewport.y == viewportHeight))
			{
				viewportWidth = viewport.x;
				viewportHeight = viewport.y;
				float aspect = 0.0f;
				if (viewportHeight != 0.0f)
					aspect = viewportWidth / viewportHeight;

				aspectRatio = aspect;
				isDirtyProjection = true;
			}

			if (isDirtyProjection)
			{
				ComputeProjectionMatrix();
			}

			return projectionMatrix;
		}

		// Camera transform
		Vector2D translation;
		float distance;
		float nearClip;
		float farClip;
		float rotation;
		float fieldOfView;
		float size;

		// Projection
		mutable float viewportWidth;
		mutable float viewportHeight;
		mutable float aspectRatio;
		mutable bool isDirtyProjection;
		mutable bool isDirtyView;

		ProjectionMode mode;

	private:
		// Helper functions
		void ComputeViewMatrix() const
		{
			glm::vec3 up(sinf(rotation), cosf(rotation), 0.0f);
			glm::vec3 eye(translation.x, translation.y, distance);
			glm::vec3 target(translation.x, translation.y, 0.0f);

			glm::mat4& view = static_cast<glm::mat4&>(*static_cast<glm::mat4*>(viewMatrix.data));
			view = glm::lookAt(eye, target, up);

			isDirtyView = false;
		}

		void ComputeProjectionMatrix() const
		{
			glm::mat4& proj = GlmMatrix(projectionMatrix);

			// Start with identity matrix
			proj = glm::mat4(1.0f);

			if (mode == PM_Orthographic)
			{
				float depth = farClip - nearClip;
				float top = size * 0.5f;
				float right = top * aspectRatio;

				proj[0][0] = 1.0f / right;
				proj[1][1] = 1.0f / top;
				proj[2][2] = -1.0f / depth;
				proj[2][3] = -nearClip / depth;
			}
			else
			{
				proj = glm::perspective(glm::radians(fieldOfView),
					aspectRatio, nearClip, farClip);
			}
			isDirtyProjection = false;
		}

		mutable Matrix3D viewMatrix;
		mutable Matrix3D projectionMatrix;
	};

	namespace
	{
		struct Ray
		{
			// Constructors
			Ray(const glm::vec3& start, const glm::vec3& end);

			// Start of the ray
			glm::vec3 start;

			// Direction of the ray, precomputed from start and end.
			glm::vec3 direction;
		};

		// Finds an intersection between the given ray and the plane Z = 0, if it exists.
		// Params:
		//   ray = The ray to intersect with the Z plane.
		//   intersectionPoint = The intersection point, if any.
		// Returns:
		//   True if there is an intersection, false otherwise.
		bool RayZPlaneIntersection(const Ray& ray, Vector2D& intersectionPoint);
	}

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Camera::Camera()
		: pimpl(new Implementation())
	{
	}

	Camera::~Camera()
	{
		delete pimpl;
	}

	void Camera::Use() const
	{
		const ShaderProgram* program = &EngineGetModule(GraphicsEngine)->GetSpriteShader();
		program->SetUniform("projectionMatrix", pimpl->GetProjectionMatrix());
		program->SetUniform("viewMatrix", pimpl->GetViewMatrix());

		program = &EngineGetModule(GraphicsEngine)->GetFontShader();
		program->SetUniform("projectionMatrix", pimpl->GetProjectionMatrix());
		program->SetUniform("viewMatrix", pimpl->GetViewMatrix());
	}

	const Vector2D& Camera::GetTranslation() const
	{
		return pimpl->translation;
	}

	void Camera::SetTranslation(const Vector2D& _translation)
	{
		if (AlmostEqual(pimpl->translation, _translation))
			return;

		pimpl->translation = _translation;
		pimpl->isDirtyView = true;
	}

	float Camera::GetFOV() const
	{
		return pimpl->fieldOfView;
	}

	void Camera::SetFOV(float angle)
	{
		if (AlmostEqual(angle, pimpl->fieldOfView))
			return;

		pimpl->fieldOfView = angle;
		pimpl->fieldOfView = std::max(minFOV, pimpl->fieldOfView);
		pimpl->fieldOfView = std::min(maxFOV, pimpl->fieldOfView);
		pimpl->isDirtyProjection = true;
	}

	float Camera::GetVerticalSize() const
	{
		return pimpl->size;
	}

	void Camera::SetVerticalSize(float _size)
	{
		if (AlmostEqual(pimpl->size, _size))
			return;

		pimpl->size = _size;
		pimpl->size = std::max(minSize, pimpl->size);
		pimpl->size = std::min(maxSize, pimpl->size);
		pimpl->isDirtyProjection = true;
	}

	ProjectionMode Camera::GetProjectionMode() const
	{
		return pimpl->mode;
	}

	void Camera::SetProjectionMode(ProjectionMode mode)
	{
		if (pimpl->mode == mode)
			return;

		pimpl->mode = mode;

		// Make sure projection matrix gets recalculated
		pimpl->isDirtyProjection = true;
	}

	// Sets camera properties to default values
	// (Translation of [0, 0], FOV of 37 degrees)
	void Camera::Reset()
	{
		SetTranslation(Vector2D(0.0f, 0.0f));
		SetFOV(defaultFOV);
		SetVerticalSize(defaultSize);
		pimpl->rotation = 0.0f;
	}

	const BoundingRectangle Camera::GetScreenWorldDimensions() const
	{
		float right = pimpl->viewportWidth / 2.0f;
		float top = pimpl->viewportHeight / 2.0f;
		Vector2D center = GetTranslation();
		Vector2D extents(right, top);

		// If using perspective, we have a lot more work to do...
		center = ScreenToWorldPosition(extents);
		Vector2D topRight = ScreenToWorldPosition(Vector2D(static_cast<float>(pimpl->viewportWidth), 0.0f));
		Vector2D bottomLeft = ScreenToWorldPosition(Vector2D(0.0f, static_cast<float>(pimpl->viewportHeight)));
		extents = (topRight - bottomLeft) * 0.5f;

		return BoundingRectangle(center, extents);
	}

	Vector2D Camera::ScreenToWorldPosition(const Vector2D & screenPosition) const
	{
		Vector2D worldPoint(screenPosition);
		float winY = static_cast<float>(pimpl->viewportHeight) - screenPosition.y;
		glm::vec4 viewport(0, 0, pimpl->viewportWidth, pimpl->viewportHeight);

		const glm::mat4& camProj = GlmMatrix(pimpl->GetProjectionMatrix());
		const glm::mat4& camView = GlmMatrix(pimpl->GetViewMatrix());

		// Create screen-to-world ray
		glm::vec3 start = glm::unProject(glm::vec3(screenPosition.x, winY, -1.0f), camView, camProj, viewport);
		glm::vec3 end = glm::unProject(glm::vec3(screenPosition.x, winY, 1.0f), camView, camProj, viewport);
		Ray ray(start, end);

		// Intersect ray with plane Z = 0
		RayZPlaneIntersection(ray, worldPoint);

		// Return world coordinates
		return worldPoint;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Helper for screen to world
	namespace
	{
		// Ray constructor
		Ray::Ray(const glm::vec3& start, const glm::vec3& end)
			: start(start), direction(end - start)
		{
		}

		// Finds an intersection between the given ray and the plane Z = 0, if it exists.
		// Params:
		//   ray = The ray to intersect with the Z plane.
		//   intersectionPoint = The intersection point, if any.
		// Returns:
		//   True if there is an intersection, false otherwise.
		bool RayZPlaneIntersection(const Ray& ray, Vector2D& intersectionPoint)
		{
			// Plane
			// ax + by + cz = d
			// z = 0
			float z = 0;

			// Line
			// (x, y, z) = (start) + t[direction]
			// Solve for t
			// z = start.z + t * direction.z
			// t * direction.z = z - start.z
			// t = (z - start.z) / direction.z
			float t = (z - ray.start.z) / ray.direction.z;

			// Check that t is valid
			if (t > 1.0f || t < 0.0f)
				return false;

			// Find intersection point
			glm::vec3 worldPoint = ray.start + t * ray.direction;
			intersectionPoint = Vector2D(worldPoint.x, worldPoint.y);

			return true;
		}
	}
}

//------------------------------------------------------------------------------
