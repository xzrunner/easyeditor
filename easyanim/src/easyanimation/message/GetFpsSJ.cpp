#include "GetFpsSJ.h"
#include "message_id.h"

namespace eanim
{

GetFpsSJ* GetFpsSJ::m_instance = NULL;

GetFpsSJ::GetFpsSJ(int id)
	: Subject(id)
{
}

int GetFpsSJ::Get()
{
	int fps = 0;
	Notify(&fps);
	return fps;
}

GetFpsSJ* GetFpsSJ::Instance()
{
	if (!m_instance) {
		m_instance = new GetFpsSJ(MSG_GET_FPS);
	}
	return m_instance;
}

}