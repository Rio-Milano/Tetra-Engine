#include"Helper.h"



void Helper::Status_::DisplayUsefulInfo()
{
	int maxAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
	std::cout << "The maximun number of vertex Attributes supported by your hardware is :" << maxAttribs << std::endl;
}

