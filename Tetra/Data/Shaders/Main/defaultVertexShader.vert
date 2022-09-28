#version 330 core

//IN
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTextureCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in mat4 inInstanceMat;

//OUT
out Varying
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
} oData;

//UNIFORMS
layout(std140) uniform Matricies
{
	mat4 Projection_X_View;
};
uniform mat4 worldMat;
uniform mat3 normalMat;
uniform bool useInstancedMat;


void main()
{
	//add vertex data to interface block
	oData.texCoord = inTextureCoord;
	oData.normal = normalMat * inNormal;
	
	vec4 pos = vec4(0);
	
	if(useInstancedMat) 
		pos = inInstanceMat * worldMat * vec4(inPos, 1.0);
	else
		pos = worldMat * vec4(inPos, 1.0);

	oData.position = pos.xyz;
	gl_Position = Projection_X_View * pos;
}