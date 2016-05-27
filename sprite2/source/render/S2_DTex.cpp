#include "S2_DTex.h"

namespace s2
{

SINGLETON_DEFINITION(DTex)

DTex::DTex() 
{
//	sl::ShaderMgr::Instance()->SetShader(sl::SPRITE2);
	Init();
}

DTex::~DTex() 
{
}

}