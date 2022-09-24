#version 330 core

//GEOM IN AND OUT
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

//IN
in Varying
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
} iData[];

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

layout(std140) uniform World
{
	vec3 cameraPosition;
	float time;
};


void ExplodeVertex(int faceVertex)
{
	//validate face vertex
	if(faceVertex < 0 && faceVertex > 3) return;

	//assign out data for current vertex
	oData.texCoord = iData[faceVertex].texCoord;
	oData.normal = iData[faceVertex].normal;
	

	float magnitude = (sin(time)+1.0) / 8;

	vec3 direction = normalize(iData[0].normal) * magnitude;

	oData.position =  iData[faceVertex].position + direction;
	
	gl_Position = Projection_X_View * vec4(oData.position, 1.0);

	EmitVertex();

}

void main()
{
	ExplodeVertex(0);
	ExplodeVertex(1);
	ExplodeVertex(2);
	EndPrimitive();
	//Using Main/defaultFragmentShader
}