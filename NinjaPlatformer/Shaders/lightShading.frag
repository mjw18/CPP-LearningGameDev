#version 130
//The fragment shader operates on each pixel in a given polygon

//input vec 4 fragment color from vertex shader
in vec4 fragmentColor;
in vec2 fragmentPosition;
in vec2 fragmentUV;

//takes into account alpha since vec4
out vec4 color;

void main()
{
	float distance = length(fragmentUV);
	color = vec4(fragmentColor.rgb, fragmentColor.a * (pow(0.01, distance) - 0.01));	 
}