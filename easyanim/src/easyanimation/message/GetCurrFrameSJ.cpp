#include "GetCurrFrameSJ.h"
#include "message_id.h"

namespace eanim
{

GetCurrFrameSJ* GetCurrFrameSJ::m_instance = NULL;

GetCurrFrameSJ::GetCurrFrameSJ(int id)
	: Subject(id)
{
}

void GetCurrFrameSJ::Get(int& layer, int& frame)
{
	CurrFrame cf;
	Notify(&cf);
	layer = cf.layer;
	frame = cf.frame;
}

GetCurrFrameSJ* GetCurrFrameSJ::Instance()
{
	if (!m_instance) {
		m_instance = new GetCurrFrameSJ(MSG_GET_CURR_FRAME);
	}
	return m_instance;
}

}