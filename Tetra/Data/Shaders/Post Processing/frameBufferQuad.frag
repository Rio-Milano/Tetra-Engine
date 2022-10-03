#version 330 core

//IN
in vec2 varying_texCoord;

//OUT
out vec4 FragColor;

//STRUCTS1
struct Material
{
	sampler2D diffuseMap;
	bool hasDiffuseMap;
	vec3 defaultDiffuseColor;
};

//basic 1 step post processing effects
uniform bool invertFragColor;
uniform bool greyScaleFragColor;

//convolution/Kernal based post processing effects
uniform float XsamplingOffset;
uniform float YsamplingOffset;
uniform bool sharpenKernel;
uniform bool weirdKernel;
uniform bool blurKernel;
uniform bool edgeDetectionKernel;
uniform Material material;
uniform float gamma;

void main()
{
	vec3 fragColor = texture(material.diffuseMap, varying_texCoord).xyz;

	if(invertFragColor)
		fragColor = vec3(1.0) - fragColor;

	if(greyScaleFragColor)
	{
		//using weighted channels
		float averageColorComponentColorValue = fragColor.r * 0.2126 + fragColor.g *0.7152 + 0.0722 * fragColor.b;
		fragColor = vec3(averageColorComponentColorValue, averageColorComponentColorValue, averageColorComponentColorValue);
	}

	//define a buffer to hold kernel weights
	float Kernel[9] = float[](0, 0, 0, 0, 0, 0, 0, 0, 0);

	//decide if any kernel is being used
	bool usingAKernel = sharpenKernel || weirdKernel || blurKernel || edgeDetectionKernel;

	if(usingAKernel)//if a kernel is being used
	{
		//apply using kernel weights to kernal
		if(sharpenKernel)
		{
			Kernel[0] += -1;
			Kernel[1] += -1;
			Kernel[2] += -1;

			Kernel[3] += -1;
			Kernel[4] += 9;
			Kernel[5] += -1;

			Kernel[6] += -1;
			Kernel[7] += -1;
			Kernel[8] += -1;
		};

		if(weirdKernel)
		{
			Kernel[0] += 2;
			Kernel[1] += 2;
			Kernel[2] += 2;

			Kernel[3] += 2;
			Kernel[4] += -15;
			Kernel[5] += 2;

			Kernel[6] += 2;
			Kernel[7] += 2;
			Kernel[8] += 2;
		};

		if(blurKernel)
		{
			Kernel[0] += 1.0 / 16.0;
			Kernel[1] += 2.0 / 16.0;
			Kernel[2] += 1.0 / 16.0;

			Kernel[3] += 2.0 / 16.0;
			Kernel[4] += 4.0 / 16.0;
			Kernel[5] += 2.0 / 16.0;

			Kernel[6] += 1.0 / 16.0;
			Kernel[7] += 2.0 / 16.0;
			Kernel[8] += 1.0 / 16.0;
		};

		if(edgeDetectionKernel)
		{
			Kernel[0] += 1;
			Kernel[1] += 1;
			Kernel[2] += 1;

			Kernel[3] += 1;
			Kernel[4] += -8;
			Kernel[5] += 1;

			Kernel[6] += 1;
			Kernel[7] += 1;
			Kernel[8] += 1;
		};


		fragColor *= Kernel[4];//apply kernel weight to centre fragment

		int kernelIndex = 0;//index for looking up kernel weights

		for(int y = 1; y >= -1; y--)//loop arround current fragment
		{
			for(int x = -1; x <= 1; x++)
			{
				//skip centre fragment
				if(y == 0 && x == 0)
				{
					kernelIndex++;
					continue;
				}
				
				//get the texture coord for the surrounding fragment
				vec2 newTexCoord = vec2(XsamplingOffset * x, YsamplingOffset * y) + varying_texCoord;
				//get the fragment color of surrounding fragment
				vec3 newFragColor = texture(material.diffuseMap, + newTexCoord).xyz;
				//apply kernel effect on surrounding fragment and add to overall frag color
				fragColor += newFragColor * Kernel[kernelIndex++];

			}
		}

	}

	FragColor = vec4(fragColor, 1.0);//note that this includes the alpha value of the texture so blending is applied if enabled
	
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));


}