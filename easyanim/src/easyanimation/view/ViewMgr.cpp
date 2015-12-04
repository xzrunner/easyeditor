#include "ViewMgr.h"

namespace eanim
{

ViewMgr* ViewMgr::m_instance = NULL;

ViewMgr* ViewMgr::Instance()
{
	if (!m_instance) {
		m_instance = new ViewMgr;
	}
	return m_instance;
}

ViewMgr::ViewMgr()
{
	library = NULL;
	property = NULL;
	img_page = NULL;

	toolbar = NULL;
	stage = NULL;
	layers = NULL;
	keys = NULL;

	viewlist = NULL;
}

}