#ifndef APPLICATION_H
#define APPLICATION_H

#include"BaseLayer.h"

#include<memory>

class Application
{
public:
	void PushLayer(const std::shared_ptr<BaseLayer>& newLayer);
	void Run();

private:
	std::shared_ptr<BaseLayer> m_baseLayer{nullptr};

};

#endif