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

layout(std140) uniform World
{
	float time;
};
uniform mat4 worldMat;



vec3 ComputeNormal()
{
	vec3 A = inData[0].position;
	vec3 B = inData[1].position;
	vec3 C = inData[2].position;

	vec3 AB = B-A;
	vec3 AC = C-A;

	vec3 normal = normalize(cross(AB, AC));

	return normal;
}

void ExplodeVertex(int faceVertex, vec3 normal)
{
	//validate face vertex
	if(faceVertex < 0 && faceVertex > 3) return;

	//assign out data for current vertex
	outData.textureCord = inData[faceVertex].textureCord;
	outData.normal = inData[faceVertex].normal;
	

	float magnitude = ((sin(time)+1.0) / 2) ;

	vec3 direction = normal * magnitude;

	//get new position then put in world space
	outData.position = (worldMat * vec4(inData[faceVertex].position + direction, 1.0)).xyz;
	
	//get position, add direction to get new position, put new position in world space then view space then clip space
	gl_Position = Projection_X_View * worldMat * (gl_in[faceVertex].gl_Position + vec4(direction, 0.0));

	EmitVertex();

}

void main()
{
	vec3 normal = ComputeNormal();
	ExplodeVertex(0, normal);
	ExplodeVertex(1, normal);
	ExplodeVertex(2, normal);
	EndPrimitive();

}