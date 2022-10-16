#version 330 core

//IN
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTextureCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in mat4 inInstanceMat;
layout(location = 7) in vec3 inTangent;//precalculated


//OUT
out Varying
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	mat3 TBN;
} oData;

//UNIFORMS
layout(std140) uniform Matricies
{
	mat4 Projection_X_View;
};


uniform mat4 worldMat;
uniform mat3 normalMat;//note: this is per call not per instance. Fix this later...
uniform bool useInstancedMat;


void main()
{
	//add vertex data to interface block
	oData.texCoord = inTextureCoord;
	oData.normal = normalMat * inNormal;
	
	vec4 pos = vec4(0.0);

	if(useInstancedMat) 
		pos = inInstanceMat * worldMat * vec4(inPos, 1.0);
	else
		pos = worldMat * vec4(inPos, 1.0);

	oData.position = pos.xyz;

	//more expensive doing it this way to make a note of it
	//calculate tbn matrix
	vec3 T = normalize(normalMat * inTangent);//normalize the tangent
	vec3 N = oData.normal;//get the normal
	// re-orthogonalize using grasm schmidt process
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(T, N));//calculate the bitangent
	oData.TBN = mat3(T, B, N);//construct orthagonal rotation matrix to go from tangent space to world space





	gl_Position = Projection_X_View * pos;
}