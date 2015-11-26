#include "SetCanvasDirtySJ.h"
#include "subject_id.h"

namespace d2d
{

SetCanvasDirtySJ* SetCanvasDirtySJ::m_instance = NULL;

SetCanvasDirtySJ::SetCanvasDirtySJ(int id)
	: Subject(id)
{
}

void SetCanvasDirtySJ::SetDirty()
{
	Notify(NULL);
}

SetCanvasDirtySJ* SetCanvasDirtySJ::Instance()
{
	if (!m_instance) {
		m_instance = new SetCanvasDirtySJ(MSG_SET_CANVAS_DIRTY);
	}
	return m_instance;
}

}