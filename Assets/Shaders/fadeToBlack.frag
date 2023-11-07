// Pixel shader for Fade To Black effect
#version 330 core

in vec2 textureCoordinate;

out vec4 fragColor;

uniform sampler2D sourceTexture;

uniform float timer;			// Current time in fade. Starts at effectDuration and decreases.
uniform float effectDuration;	// Duration of the fade

void main()
{
	fragColor = vec4(texture2D(sourceTexture, textureCoordinate).rgb, 1);

	if(timer > 0.0)
		fragColor *= timer / effectDuration;
	else fragColor *= 0.0;
}
