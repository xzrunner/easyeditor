#include "S2_CameraMgr.h"

namespace s2
{

SINGLETON_DEFINITION(CameraMgr)

CameraMgr::CameraMgr() 
	: m_type(ORTHO)
{
}

CameraMgr::~CameraMgr() 
{
}

}