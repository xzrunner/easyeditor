#include "RefreshPanelSJ.h"
#include "subject_id.h"

namespace d2d
{

RefreshPanelSJ* RefreshPanelSJ::m_instance = NULL;

RefreshPanelSJ::RefreshPanelSJ(int id)
	: Subject(id)
{
}

void RefreshPanelSJ::Refresh()
{
	Notify(NULL);
}

RefreshPanelSJ* RefreshPanelSJ::Instance()
{
	if (!m_instance) {
		m_instance = new RefreshPanelSJ(MSG_REFRESH_PANEL);
	}
	return m_instance;
}

}