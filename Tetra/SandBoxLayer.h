#ifndef SANDBOXLAYER_H
#define SANDBOXLAYER_H

#include"BaseLayer.h"
#include"TestEntity.h"
class SandBoxLayer : public BaseLayer
{
public:
	void Start() override final;
	void Update(float dt) override final;
	void Render() override final;
	void End() override final;

private:
	TestEntity m_entity;
};

#endif