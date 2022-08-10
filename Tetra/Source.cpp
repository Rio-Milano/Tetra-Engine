#include"Application.h"
#include"SandBoxLayer.h"

int main()
{
	Application application;
	application.PushLayer(std::make_shared<SandBoxLayer>());
	application.Run();
	return 0;
};