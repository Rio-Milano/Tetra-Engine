#version 330 core

layout(triangles) in;
in VARYING
{	vec2 texCoord;
}iData[];


layout (triangle_strip, max_vertices = 18) out;
out VARYING
{
	vec4 FragPos;
	vec2 texCoord;
}oData;

uniform mat4 lightSpaceDirections[6];

void main()
{
	for(int i = 0; i < 6; i++)
	//loop through each cube map side
	{
		gl_Layer = i;//set the face of the cube map
		
		for(int j = 0; j < 3; j++)
		//loop through each vertex and construct a triangle at the perspective of the light direction index
		{
			oData.texCoord = iData[j].texCoord;
			oData.FragPos = gl_in[j].gl_Position;
			gl_Position = lightSpaceDirections[i] *  gl_in[j].gl_Position;
			EmitVertex();
		}
		EndPrimitive();

	}
}