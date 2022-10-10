#version 330 core

//take in triangles
layout(triangles) in;
in VARYING
{	vec2 texCoord;
}iData[];


//output triangles to each of the cube map faces
layout (triangle_strip, max_vertices = 18) out;
out VARYING
{
	vec4 FragPos;//pass the frag position for depth calculations
	vec2 texCoord;//pass the texture cord for alpha discard calculations
}oData;

//projection view mats for each cube map face
uniform mat4 lightSpaceDirections[6];

void main()
{
	//loop through each cube map face
	for(int i = 0; i < 6; i++)
	{
		gl_Layer = i;//set the face of the cube map
		
		//loop through each vertex and construct a triangle at the perspective of the light direction index
		for(int j = 0; j < 3; j++)
		{
			oData.texCoord = iData[j].texCoord;
			oData.FragPos = gl_in[j].gl_Position;
			gl_Position = lightSpaceDirections[i] *  gl_in[j].gl_Position;
			EmitVertex();
		}
		EndPrimitive();

	}
}