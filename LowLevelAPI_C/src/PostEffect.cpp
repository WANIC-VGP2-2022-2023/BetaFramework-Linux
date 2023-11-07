//------------------------------------------------------------------------------
//
// File Name:	PostEffect.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "PostEffect.h"

#include "../../LowLevelAPI/include/PostEffect.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// C-style Pseudo-Inheritance
typedef struct PostEffect : public Beta::PostEffect
{
public:
	// Constructor
	PostEffect(const char* fragmentShader, const char* vertexShader,
		EffectFunction init, EffectFunctionDt update, EffectFunction draw, void* data)
		: Beta::PostEffect(fragmentShader, vertexShader), data(data),
		init(init), update(update), draw(draw)
	{
	}

	// Destructor
	~PostEffect()
	{
		free(data);
	}

	// Override this to reset variables as needed.
	virtual void Initialize()
	{
		init(this);
	}

	// Override this to modify variables before they are sent to the shader.
	virtual void Update(float dt)
	{
		update(this, dt);
	}

	// Sends uniform data to shader. Called automatically by the Render function.
	// Override this to send custom data to the shader.
	virtual void Draw()
	{
		draw(this);
	}

	// Additional data
	void* data;

private:
	// Function pointers
	EffectFunction init; 
	EffectFunctionDt update; 
	EffectFunction draw;
}PostEffect;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor
// Params:
//   fragmentShader = The filename of the fragment shader for this effect.
//   vertexShader = The filename of the vertex shader for this effect. Defaults to simple vertex shader.
//   init = Pass in a function to reset variables as needed.
//   update = Pass in a function to modify variables before they are sent to the shader.
//   draw = Pass in a function to send custom data to the shader.
//   data = Pass in dynamically allocated extra data if needed. Will be freed when the effect is destroyed.
PostEffect* PostEffectCreate(const char* fragmentShader, const char* vertexShader,
	EffectFunction init, EffectFunctionDt update, EffectFunction draw, void* data)
{
	return new PostEffect(fragmentShader, vertexShader, init, update, draw, data);
}

// Destructor
void PostEffectDestroy(PostEffect** effect)
{
	// Delete subclass
	delete *effect;
	*effect = NULL;
}

// Test whether this effect is currently being used by the renderer.
bool PostEffectIsActive(const PostEffect* effect)
{
	return effect->IsActive();
}

// Get the shader program used by this effect.
const ShaderProgram* PostEffectGetProgram(const PostEffect* effect)
{
	return reinterpret_cast<const ShaderProgram*>(&effect->GetProgram());
}

// Get any extra data that was passed in when the effect was initialized.
void* PostEffectGetData(const PostEffect* effect)
{
	return effect->data;
}

//------------------------------------------------------------------------------
