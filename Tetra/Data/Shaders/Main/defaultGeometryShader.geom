#version 330 core

//IN
layout(triangles) in;
in Varying
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	mat3 TBN;
} inData[];

//OUT
layout (triangle_strip, max_vertices = 3) out;
out Varying
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	mat3 TBN;
} outData;

//UNIFORM
uniform bool verticesAreDynamic;//if vertices can change then precomputed tangents cant be valid so compute TBN matriix in the geometry shader

//FUNCTIONS
void ProcessVertex(int i, mat3 _TBN)
{
	outData.position = inData[i].position;
	outData.texCoord = inData[i].texCoord;
	outData.normal = inData[i].normal;
	outData.TBN = _TBN;

	gl_Position = gl_in[i].gl_Position;

	EmitVertex();
}

void ProcessTriangle(bool overrideTBN, vec3 _tangent)
{
	if(!overrideTBN)
		for(int i = 0; i < 3; i++)	ProcessVertex(i, inData[i].TBN);
	else
	{
		for(int i = 0; i < 3; i++)	
		{
			//use grahm shmidt process to reorthogonalize tangent in respect to normal
			vec3 T = _tangent;
			vec3 N = inData[i].normal;

			T = normalize(T - dot(T, N) * N);

			//Create a bitangent from the normal and tangent
			vec3 B = cross(T, N);

			//compute TBN matrix for surface
			mat3 TBN_ = mat3(T, B, N);

			ProcessVertex(i, TBN_);
		}
	}

	EndPrimitive();
}

void main()
{

	if(!verticesAreDynamic)
		ProcessTriangle(false, vec3(0));
	else
	{
		//Generate a tangent for the surface

		//get vertex positions in world space
		vec3 
			Apos = inData[0].position,
			Bpos = inData[1].position,
			Cpos = inData[2].position;

		//get vertex texture coords
		vec2
			Atex = inData[0].texCoord,
			Btex = inData[1].texCoord,
			Ctex = inData[2].texCoord;


		//calculate world space edge
		vec3	
			edge_1 = Bpos - Apos,
			edge_2 = Cpos - Apos;

		//calculate delta tex coords for 2 edges
		float
			deltaU1 = Btex.x - Atex.x,
			deltaV1 = Btex.y - Atex.y,

			deltaU2 = Ctex.x - Atex.x,
			deltaV2 = Ctex.y - Atex.y;

		float UVmatrixDeterminant = deltaU1 * deltaV2 - deltaU2 * deltaV1;
		float cofactor = 1.0f / UVmatrixDeterminant;

		vec3 tangent;
		tangent.x = cofactor * (deltaV2 * edge_1.x  - deltaV1 * edge_2.x);
		tangent.y = cofactor * (deltaV2 * edge_1.y - deltaV1 * edge_2.y);
		tangent.z = cofactor * (deltaV2 * edge_1.z - deltaV1 * edge_2.z);
		tangent = normalize(tangent);

		ProcessTriangle(true, tangent);
	}
}