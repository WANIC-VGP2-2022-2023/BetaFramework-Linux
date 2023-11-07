#version 330 core

in vec2 textureCoordinate;

out vec4 fragColor;

uniform sampler2D sourceTexture;

uniform float timer;
uniform float invertTime;

void main()
{
	vec4 color = texture2D(sourceTexture, textureCoordinate);
	vec4 inverted = vec4(1.0 - color.rgb, 1.0);

	float pi = 3.14;
	float period =  pi / invertTime;

    float t = (cos(timer * period) + 1.0) / 2.0;

	fragColor = mix(color, inverted, t);
}
