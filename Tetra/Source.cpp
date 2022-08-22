#include"Application.h"
#include"SandBoxLayer.h"

//for when changes arent included in build
//https://stackoverflow.com/questions/17283839/visual-studio-c-does-not-update-new-code-after-building

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