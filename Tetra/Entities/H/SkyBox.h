#ifndef SKYBOX_H
#define SKYBOX_H

#include"Entity.h"

#include<memory>

class Mesh;
class CubeMap;

class SkyBox : public Entity
{
public:
	SkyBox(const std::string& entityName = "SkyBox");
	~SkyBox();

	void Init() override final;
	void Update(const float& dt) override final;
	void Render(Renderer& renderer, Shader* overrideShader) override final;

	void SetFaceLocations(const std::vector<std::string>& faceLocations);
	const std::shared_ptr<CubeMap>& GetActiveCubeMap()const;

private:
	std::vector<std::string> m_faceLocations{};
	std::shared_ptr<Mesh> m_skyBoxMesh{nullptr};
	std::shared_ptr<CubeMap> m_cubeMapForSkyBox;


};

#endif