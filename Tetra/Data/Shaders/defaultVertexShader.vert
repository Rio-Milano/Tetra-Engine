#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTextureCord;
layout(location = 2) in vec3 inNormal;

out Varying
{
	vec2 textureCord;
	vec3 normal;
	vec3 position;
} outData;

uniform mat4 worldMat;
uniform mat4 Projection_X_View;
uniform mat3 normalMat;

void main()
{
	outData.textureCord = inTextureCord;

	//varying_normal =  mat3(worldMat) * inNormal;//only works for balanced scaling
	outData.normal = normalMat * inNormal;//works with both ballanced and unbalanced scaling !!look into how this works

	outData.position = (worldMat * vec4(inPos, 1)).xyz;//translate position in world space

	gl_Position = Projection_X_View * worldMat * vec4(inPos, 1.0);
}