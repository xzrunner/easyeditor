#include "ResetViewportSJ.h"
#include "subject_id.h"

namespace d2d
{

ResetViewportSJ* ResetViewportSJ::m_instance = NULL;

ResetViewportSJ::ResetViewportSJ(int id)
	: Subject(id)
{
}

void ResetViewportSJ::Reset()
{
	Notify(NULL);
}

ResetViewportSJ* ResetViewportSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ResetViewportSJ(MSG_RESET_VIEWPORT);
	}
	return m_instance;
}

}