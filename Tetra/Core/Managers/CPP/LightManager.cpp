#include "../H/LightManager.h"

#include"../../Core/Managers/H/ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()
#include"../../Framework/H/BaseLayer.h"
#define NEAR_PLANE 0.1f


LightManager::~LightManager()
{
}

void LightManager::Initialize()
{
	//resize vector of lights
	m_lights.resize(NUMBER_OF_LIGHTS);

	//define custom vertz for the light mesh
	std::vector<glm::vec3>* positions = new std::vector<glm::vec3>
	{
		//back face
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		// front face
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left
		// left face
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-right
		// right face
		glm::vec3(0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		// bottom face
		glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // top-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
		// top face
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(0.5f, 0.5f, 0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, 0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(-0.5f, 0.5f, 0.5f), // bottom-left

	};


	//create a shader to draw the lights
	Shader cubeShader;
	cubeShader.Create("Data/Shaders/Basic Position uColor/lightCubeVertexShader.vert", "Data/Shaders/Basic Position uColor/lightCubeFragmentShader.frag");
	//add the shader to storage
	ShaderManager.AddShader("lightCubeShader", cubeShader);

	//generate the mesh used for drawing the lights
	m_meshForLight.GenerateMesh(positions, {}, {}, {}, {}, 1, GL_STATIC_DRAW);

	
	InitializeShadowMaps();
	SetTextureUnitsForShaderShadowMaps();
}

void LightManager::SetShader(Shader* shader)
{
	//set the shader to recieve lighting state and aksi
	m_shader = shader;
}

void LightManager::SetDirectionalLight(const glm::vec3& direction, const glm::vec3& color, const float& intensity, int index)
{
	//if no index set
	if (index == -1)
	{
		//get new index
		int freeLightIndex = GetFreeLight();

		//new index is unavaliable
		if (freeLightIndex == -1)
		{
			std::cout << "No lights avaliable!\n";
			return;
		}
		//set new index
		index = freeLightIndex;
	}
	//get light at index
	Light& light = m_lights[index];

	//assign values of light
	light.m_inUse = true;
	light.m_lightColor = color;
	light.m_direction = direction;
	light.m_lightIntensity = intensity;
	light.m_lightType = LightType::Directional;
}


void LightManager::SetSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, const float& intensity, int index, float innerCutOff, float outerCutoff, const float& range)
{
	//if no index set
	if (index == -1)
	{
		//get new index
		int freeLightIndex = GetFreeLight();

		//new index is unavaliable
		if (freeLightIndex == -1)
		{
			std::cout << "No lights avaliable!\n";
			return;
		}
		//set new index
		index = freeLightIndex;
	}

	//get light from index
	Light& light = m_lights[index];
	//assign values of light
	light.m_inUse = true;
	light.m_lightColor = color;
	light.m_direction = direction;
	light.m_lightIntensity = intensity;
	light.m_range = range;
	light.m_position = position;
	light.m_innerCutOffAngle = innerCutOff;
	light.m_outerCutOffAngle = outerCutoff;
	light.m_lightType = LightType::Spot;
}

void LightManager::SetPointLight(const glm::vec3& position, const glm::vec3& color, const float& intensity, const float& range, int index)
{
	//if no index set
	if (index == -1)
	{
		//get new index
		int freeLightIndex = GetFreeLight();

		//new index is unavaliable
		if (freeLightIndex == -1)
		{
			std::cout << "No lights avaliable!\n";
			return;
		}
		//set new index
		index = freeLightIndex;
	}

	//get light at index
	Light& light = m_lights[index];

	//set light attributes
	light.m_inUse = true;
	light.m_position = position;
	light.m_lightColor = color;
	light.m_lightIntensity = intensity;
	light.m_range = range;
	light.m_lightType = LightType::Point;
}

Light& LightManager::GetLight(const int& index)
{
	//throw break point if index out of range
	_ASSERT(index < NUMBER_OF_LIGHTS && index >= 0);
	
	return m_lights[index];
}

void LightManager::UpdateLightsUBO(const float& dt)
{

	//get the uniform buffer object
	std::shared_ptr<UniformBufferObject> lights_ubo = ShaderManager.GetUniformBufferObject("Lights");
	
	//validate ubo
	_ASSERT(lights_ubo != nullptr);

	
	//offset in bytes for current element
	GLsizei offset = 0;
	

	//loop each light
	for (size_t i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		//get light number
		const size_t lightNumber = i + 1ull;
		
		//get light string prefix
		std::string prefix = "Light:" + std::to_string(lightNumber) + ":";

		//get current light
		Light& light = m_lights[i];

		//set elements
		lights_ubo->SetBufferElement(prefix + "Position", &light.m_position);
		lights_ubo->SetBufferElement(prefix + "Color", &light.m_lightColor);
		lights_ubo->SetBufferElement(prefix + "Direction", &light.m_direction);
		lights_ubo->SetBufferElement(prefix + "Intensity", &light.m_lightIntensity);
		lights_ubo->SetBufferElement(prefix + "Range", &light.m_range);
		
		float innerCutoff = static_cast<float>(cos(glm::radians(light.m_innerCutOffAngle)));
		lights_ubo->SetBufferElement(prefix + "InnerCutoff",&innerCutoff);
		
		float outerCutoff = static_cast<float>(cos(glm::radians(light.m_outerCutOffAngle)));
		lights_ubo->SetBufferElement(prefix + "OuterCutoff", &outerCutoff);
		
		lights_ubo->SetBufferElement(prefix + "Type", &light.m_lightType);
		lights_ubo->SetBufferElement(prefix + "InUse", &light.m_inUse);
		
		lights_ubo->SetBufferElement(prefix + "Light-Space", &light.m_directionalShadows.m_lightSpace);

		lights_ubo->SetBufferElement(prefix + "farPlane", (void*)& Light::farPlane);





		//if light is a spot light
		if (light.m_lightType == LightType::Spot)
		{
			//if spot light is dynamic
			if (light.simulateLight)
			{
				//have it rotate arround the origin using a set angle
				float x = cos(glm::radians(light.lightAngle)) * light.radius;
				float z = sin(glm::radians(light.lightAngle)) * light.radius;
				light.m_position = glm::vec3(x, light.m_position.y, z);

				//make angle bigger
				light.lightAngle += light.rotationSpeed * dt;
				if (light.lightAngle > 360.f)
					light.lightAngle = 0.f;

				//set light to allways face origin
				light.m_direction = glm::normalize(glm::vec3(0.0f) - light.m_position);
			}
		}
	}

}

void LightManager::DrawLights(Renderer& renderer)
{
	//get the shader used to draw the lights
	Shader& shader = ShaderManager.GetShader("lightCubeShader");

	//loop lights
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		//get current light
		Light& light = m_lights[i];

		//if light is on
		if (light.m_inUse && light.m_drawLight)
		{
			Shader& shader = ShaderManager.GetShader("lightCubeShader");

			//if light a point or spot light
			if (light.m_lightType != LightType::Directional)
			{
				//translate to light position
				glm::mat4 transform(1.0f);
				transform = glm::translate(transform, m_lights[i].m_position);
				
				//if light is spot
				if (light.m_lightType == LightType::Spot)
				{
					//compute a rotation matrix used to align vectors
					// 
					// this will rotate the light direction at the origin to the light direction in memory
					
					//code taken from :https://iquilezles.org/articles/noacos/
					const glm::vec3   axi = glm::normalize(glm::cross(light.m_direction, glm::vec3(-1, 0, 0)));
					const float  ang = acosf(glm::clamp(glm::dot(light.m_direction, glm::vec3(-1, 0, 0)), -1.0f, 1.0f));
					const float  co = cosf(ang);
					const float  si = sinf(ang);
					const float  ic = 1.0f - co;
					//calculate a rotation matrix to rotate vector A to B
					const glm::mat4 rot =
					{ 
						axi.x * axi.x * ic + co,			axi.y * axi.x * ic - si * axi.z,	axi.z * axi.x * ic + si * axi.y,	0,
						axi.x * axi.y * ic + si * axi.z,	axi.y * axi.y * ic + co,			axi.z * axi.y * ic - si * axi.x,	0,
						axi.x * axi.z * ic - si * axi.y,	axi.y * axi.z * ic + si * axi.x,	axi.z * axi.z * ic + co,			0,
						0,									0,									0,									1 
					};
					transform *= rot;//align the mesh direction with light direction

					glm::vec3 spotLightScale(1.f, 0.3f, 0.3f);
					transform = glm::scale(transform, spotLightScale);//turn the mesh into an oblong

					glm::vec3 shiftTransform(-0.5f, 0.0f, 0.0f);
					transform = glm::translate(transform, shiftTransform);//have the mesh start at 0 and finish along the -x axis

					//this will be the smaller part of the model of the spot light that looks like a bulb
					
					glm::mat4 transform_2(1.0f);
					transform_2 = glm::translate(transform_2, light.m_position);//translate mesh to light position
					transform_2 *= rot;//aligh it with direction of light
					transform_2 = glm::scale(transform_2, glm::vec3(0.6f, 0.6f, 0.6f));//scale down mesh
					
					glm::vec3 overallLightColor = light.m_lightColor * light.m_lightIntensity;
					shader.SetUniform3fv(shader.GetLocation("cubeColor"), overallLightColor);//set bulb color to light color
					//render the bulb
					renderer.RenderMesh(m_meshForLight, transform_2, shader);

				}
				else
				{
					//point light scaling
					transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
				}
				//calculate light color for light holder mesh
				glm::vec3 overallLightColor = light.m_lightColor * light.m_lightIntensity;
				//set bulb holder color
				shader.SetUniform3fv(shader.GetLocation("cubeColor"), overallLightColor);
				//render bulb holder
				renderer.RenderMesh(m_meshForLight, transform, shader);
				
			}
		}
 	}
}

void LightManager::DrawSceneToDepthBuffer(BaseLayer* baseLayer)
{
	GLint viewPort[4];
	glGetIntegerv(GL_VIEWPORT, viewPort);

	for (GLuint lightIndex = 0; lightIndex < m_lights.size(); lightIndex++)
	{
		Light& light = m_lights[lightIndex];

		if (light.m_inUse)
		{
			if (light.m_lightType == LightType::Directional || light.m_lightType == LightType::Spot)
				DrawSceneToDepthBufferDirectionalShadows(lightIndex, baseLayer);
			else
				DrawSceneToDepthBufferOmnidirectionalShadows(lightIndex, baseLayer);
		}
	}
	glViewport(0, 0, viewPort[2], viewPort[3]);
}


int LightManager::GetFreeLight()
{
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		if (!m_lights[i].m_inUse)
			return i;
	}
	return -1;
}
void LightManager::InitializeShadowMaps()
{
	for (GLuint lightIndex = 0; lightIndex < m_lights.size(); lightIndex++)
	{
		GenerateDirectionalShadowMappingBuffers(lightIndex);
		GenerateOmnidirectionalShadowMappingBuffers(lightIndex);
	}
}
void LightManager::GenerateDirectionalShadowMappingBuffers(const GLuint& lightIndex)
{
	Light& light = m_lights[lightIndex];
	DirectionalShadows& directionalShadows = light.m_directionalShadows;

	//Generate a shadow map used for directional shadows
	{
		glGenTextures(1, &directionalShadows.m_depthBuffer);
		glBindTexture(GL_TEXTURE_2D, directionalShadows.m_depthBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, light.SHADOW_WIDTH, light.SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		static const GLfloat borderClampColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderClampColor);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Generate and bind to a frame buffer used for rendering the scene from the perspective of the light
	glGenFramebuffers(1, &directionalShadows.m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, directionalShadows.m_fbo);
	//attach the depth map
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, directionalShadows.m_depthBuffer, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindBuffer(GL_FRAMEBUFFER, 0);

	//check that the frame buffer is complete
	_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}
void LightManager::GenerateOmnidirectionalShadowMappingBuffers(const GLuint& lightIndex)
{
	Light& light = m_lights[lightIndex];
	OmnidirectionalShadows& shadows = light.m_omnidirectionalShadows;

	//Generate the depth cube map
	{
		glGenTextures(1, &shadows.depthCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadows.depthCubeMap);

		for (GLuint i = 0; i < 6u; i += 1u)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, light.SHADOW_WIDTH, light.SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		//Filter
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		//Wrapping
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}



	//Create the frame buffer
	{
		glGenFramebuffers(1, &shadows.framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, shadows.framebuffer);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadows.depthCubeMap, 0);

		_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}
void LightManager::DrawSceneToDepthBufferDirectionalShadows(const GLuint& lightIndex, BaseLayer* baseLayer)
{
	static const float
		size = 50.0f,
		left = -size / 2.0f,
		right = size / 2.0f,
		top = -size / 2.0f,
		bottom = size / 2.0f;

	Light& light = m_lights[lightIndex];


	glViewport(0, 0, light.SHADOW_WIDTH, light.SHADOW_HEIGHT);//resize the view port

	//Bind to the depth fbo and clear its depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, light.m_directionalShadows.m_fbo);
	glClear(GL_DEPTH_BUFFER_BIT);


	glm::mat4 projection(0.0f);
	projection = glm::ortho(left, right, top, bottom, NEAR_PLANE, light.farPlane);

	glm::mat4 view(0.0f);
	if (light.m_lightType == LightType::Directional)
		view = glm::lookAt(-light.m_direction * 15.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	else
		view = glm::lookAt(light.m_position, light.m_position + light.m_direction, glm::vec3(0.0f, 1.0f, 0.0f));

	light.m_directionalShadows.m_lightSpace = projection * view;

	Shader& shadowMapping = ShaderManager.GetShader("Shadow-Mapping");
	shadowMapping.SetUniformMat4f(shadowMapping.GetLocation("Light_Projection_X_View"), light.m_directionalShadows.m_lightSpace);
	baseLayer->Render(&shadowMapping);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void LightManager::DrawSceneToDepthBufferOmnidirectionalShadows(const GLuint& lightIndex, BaseLayer* baseLayer)
{

	Light& light = m_lights[lightIndex];

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), static_cast<float>(light.SHADOW_WIDTH) / static_cast<float>(light.SHADOW_HEIGHT), NEAR_PLANE, light.farPlane);

	glm::mat4 viewMats[6] =
	{
		glm::lookAt(light.m_position, light.m_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),//LOOK RIGHT
		glm::lookAt(light.m_position, light.m_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),//LOOK LEFT
		glm::lookAt(light.m_position, light.m_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),//LOOK UP
		glm::lookAt(light.m_position, light.m_position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),//LOOK DOWN
		glm::lookAt(light.m_position, light.m_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),//look forward
		glm::lookAt(light.m_position, light.m_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),//LOOK BACK
	};

	Shader& shader = ShaderManager.GetShader("Point-Shadows");
	for (size_t i = 0; i < 6; i++)
	//loop through each side of the cube map and compute a projection view matrix from mats above then send that matrix into the shader
	{
		glm::mat4 projection_x_view =  projection * viewMats[i];
		shader.SetUniformMat4f(shader.GetLocation("lightSpaceDirections[" + std::to_string(i) + "]"), projection_x_view);
	}

	shader.SetUniform1i(shader.GetLocation("lightIndex"), lightIndex);

	glViewport(0, 0, light.SHADOW_WIDTH, light.SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, light.m_omnidirectionalShadows.framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	baseLayer->Render(&shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);






}
void LightManager::SetTextureUnitsForShaderShadowMaps()
{
	const GLuint startTextureUnit = 10u;

	for (GLuint lightIndex = 0; lightIndex < m_lights.size(); lightIndex++)
	{
		const GLuint textureUnit = startTextureUnit + lightIndex;
		const Light& light = m_lights[lightIndex];

		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, light.m_directionalShadows.m_depthBuffer);

		glActiveTexture(GL_TEXTURE0 + textureUnit + m_lights.size());
		glBindTexture(GL_TEXTURE_CUBE_MAP, light.m_omnidirectionalShadows.depthCubeMap);
	}



	for (std::unordered_map<std::string, Shader>::iterator i = ShaderManager.m_programNameProgramIDMap.begin(); i != ShaderManager.m_programNameProgramIDMap.end(); i++)
	{
		Shader& currentShader = i->second;
	
		for (GLuint lightIndex = 0; lightIndex < m_lights.size(); lightIndex++)
		{
			currentShader.SetUniform1i(currentShader.GetLocation("shadowMaps[" + std::to_string(lightIndex) + "]"), startTextureUnit + lightIndex);
			currentShader.SetUniform1i(currentShader.GetLocation("pointShadows[" + std::to_string(lightIndex) + "]"), startTextureUnit + m_lights.size() +lightIndex);

		}

	}
};
