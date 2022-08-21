#include"Application.h"
#include"SandBoxLayer.h"

int main()
{
	/*
	Escape - Exit program
	U - Camera unfocus
	F - Camera focus
	*/

	Application application;
	application.PushLayer(std::make_shared<SandBoxLayer>());
	application.Run();
	return 0;
};