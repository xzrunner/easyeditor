#include "QueryWindowViewSizeSJ.h"
#include "message_id.h"

namespace eui
{
namespace window
{

QueryWindowViewSizeSJ* QueryWindowViewSizeSJ::m_instance = NULL;

QueryWindowViewSizeSJ::QueryWindowViewSizeSJ(int id)
	: Subject(id)
{
}

void QueryWindowViewSizeSJ::Query(int& width, int& height)
{
	Params p;
	Notify(&p);
	width = p.width;
	height = p.height;
}

QueryWindowViewSizeSJ* QueryWindowViewSizeSJ::Instance()
{
	if (!m_instance) {
		m_instance = new QueryWindowViewSizeSJ(MSG_QUERY_WINDOW_VIEW_SIZE);
	}
	return m_instance;
}

}
}