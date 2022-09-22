#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Varying
{
	vec2 textureCord;
	vec3 normal;
	vec3 position;
} inData[];

out Varying
{
	vec2 textureCord;
	vec3 normal;
	vec3 position;
} outData;

layout(std140) uniform Matricies
{
	mat4 Projection_X_View;
};
uniform mat4 worldMat;


void PassThrough(int faceVertex)
{
	if(faceVertex < 0 && faceVertex > 3) return;

	outData.textureCord = inData[faceVertex].textureCord;
	outData.normal = inData[faceVertex].normal;
	//put position in world space
	outData.position = (worldMat * vec4(inData[faceVertex].position, 1.0)).xyz;

	//put position in world space then view space then clip space
	gl_Position = Projection_X_View * worldMat * gl_in[faceVertex].gl_Position;

	EmitVertex();
}


void main()
{
	PassThrough(0);
	PassThrough(1);
	PassThrough(2);
	EndPrimitive();
}