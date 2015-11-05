#include "ClearPanelSJ.h"
#include "subject_id.h"

namespace d2d
{

ClearPanelSJ* ClearPanelSJ::m_instance = NULL;

ClearPanelSJ::ClearPanelSJ(int id)
	: Subject(id)
{
}

void ClearPanelSJ::Clear(Observer* except)
{
	Notify(NULL, except);
}

ClearPanelSJ* ClearPanelSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ClearPanelSJ(MSG_CLEAR_PANEL);
	}
	return m_instance;
}

}