#version 330 core

//GEOM IN OUT
layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

//in
in Varying
{
	vec3 position;
	vec3 normal;
} iData [];

//UNIFORMS
layout(std140) uniform Matricies
{
	mat4 Projection_X_View;
};

void main()
{
	//For each triangle recived by the shader
	const float magnitude = 0.25;

	//Get the centre point on the triangle
	vec3 c = (iData[0].position + iData[1].position + iData[2].position) / 3.0;
	
	//send out a vertex from the centre point and put into propper space
	gl_Position = Projection_X_View * vec4(c, 1.0);
	EmitVertex();

	//Send out a vertex from the centre point + a normal offset and put into propper space
	gl_Position = Projection_X_View * vec4(c + normalize(iData[0].normal) * 0.2, 1.0);
	EmitVertex();

	EndPrimitive();
};