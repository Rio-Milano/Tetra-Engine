#ifndef SANDBOXLAYER_H
#define SANDBOXLAYER_H

#include"BaseLayer.h"
#include"TestEntity.h"
#include"LightManager.h"

class SandBoxLayer : public BaseLayer
{
public:
	SandBoxLayer() = default;
	~SandBoxLayer() = default;

	void Start() override final;
	void Update(float dt) override final;
	void Render() override final;
	void End() override final;
	void ImGUI() override final;

private:
	LightManager m_lightManager;
	
	
	TestEntity m_entity;
	std::shared_ptr<Model> backPack = nullptr;
	std::shared_ptr<Model> aquaPig = nullptr;

	bool m_wireframeMode{false};
};

#endif