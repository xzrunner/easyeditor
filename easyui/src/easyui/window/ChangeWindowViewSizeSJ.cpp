#include "ChangeWindowViewSizeSJ.h"
#include "message_id.h"

namespace eui
{
namespace window
{

ChangeWindowViewSizeSJ* ChangeWindowViewSizeSJ::m_instance = NULL;

ChangeWindowViewSizeSJ::ChangeWindowViewSizeSJ(int id)
	: Subject(id)
{
}

void ChangeWindowViewSizeSJ::Change(int width, int height)
{
	Params p;
	p.width = width;
	p.height = height;
	Notify(&p);
}

ChangeWindowViewSizeSJ* ChangeWindowViewSizeSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ChangeWindowViewSizeSJ(MSG_CHANGE_WINDOW_VIEW_SIZE);
	}
	return m_instance;
}

}
}