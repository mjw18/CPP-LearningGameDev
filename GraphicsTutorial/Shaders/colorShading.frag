#version 130
//The fragment shader operates on each pixel in a given polygon

//input vec 4 fragment color from vertex shader
in vec4 fragmentColor;
in vec3 fragmentPosition;
in vec2 fragmentUV;

//uniform variables are the same for a given mesh
uniform sampler2D mySampler;

//takes into account alpha since vec4
out vec4 color;

void main()
{
	//get color from sampler texture using this function
	vec4 textureColor = texture(mySampler, fragmentUV);

	//Make crazy color based on position and "time"
	color = fragmentColor *textureColor;			 
}