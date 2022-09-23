#version 330 core

//IN
layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

//OUT
out Varying
{
	vec3 position;
	vec3 normal;
}oData;

//UNIFORMS
uniform mat4 worldMat;
uniform mat3 normalMat;

void main()
{
	//Get position in world space
	vec4 worldSpacePosition = worldMat * vec4(position, 1.0);

	//assign world space position
	oData.position = worldSpacePosition.xyz;

	//Undistort normal
	oData.normal = normalMat * normal;

	//pass the gl_position to the geometry shader as world space 
	gl_Position = worldSpacePosition;
}