#include "SetFpsSJ.h"
#include "message_id.h"

namespace eanim
{

SetFpsSJ* SetFpsSJ::m_instance = NULL;

SetFpsSJ::SetFpsSJ(int id)
	: Subject(id)
{
}

void SetFpsSJ::Set(int fps)
{
	Notify(&fps);
}

SetFpsSJ* SetFpsSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SetFpsSJ(MSG_SET_FPS);
	}
	return m_instance;
}

}