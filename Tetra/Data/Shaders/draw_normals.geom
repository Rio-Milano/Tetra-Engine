#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

in VARYING
{
	vec3 position;
	vec3 normal;
} iData [];

layout(std140) uniform Matricies
{
	mat4 Projection_X_View;
};



void main()
{
	const float magnitude = 0.25;

	vec3 c = (iData[0].position + iData[1].position + iData[2].position) / 3.0;
	gl_Position = Projection_X_View * vec4(c, 1.0);
	EmitVertex();

	gl_Position = Projection_X_View * vec4(c + normalize(iData[0].normal) * 0.2, 1.0);
	EmitVertex();

	EndPrimitive();
};