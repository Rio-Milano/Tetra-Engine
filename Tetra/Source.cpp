#include"Application.h"
#include"SandBoxLayer.h"

//for when changes arent included in build
//https://stackoverflow.com/questions/17283839/visual-studio-c-does-not-update-new-code-after-building


int main()
{
	/* 
	
	--- CONTROLS ---
	 
	Escape - Exit program
	U - Camera unfocus
	F - Camera focus
	*/

	Application application;//create framework for layers
	application.PushLayer(std::make_shared<SandBoxLayer>());//add experemental layer
	application.Run();//execute framework
	return 0;
};