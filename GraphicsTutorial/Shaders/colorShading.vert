#version 130
//set version number
//The vertex shader operates on each vertex

//create a 2D vector for the initial position from vbo data
in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

//output to fragment shader, use flat to stop color interpolation
out vec4 fragmentColor;
out vec3 fragmentPosition;
out vec2 fragmentUV;

uniform mat4 camMatrix;

void main() {
	//set position on screen
	
	gl_Position.xyz = (camMatrix * vec4(vertexPosition, 1.0)).xyz;
	
	//z is zero for 2D games
	//gl_Position.z = 0.0;
	
	//default to w is 1
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	//invert y coordinate so that sprite renders rightside up
	fragmentUV = vec2( vertexUV.x, 1.0-vertexUV.y);
}