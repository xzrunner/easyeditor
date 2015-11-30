#include "SetCurrFrameSJ.h"
#include "message_id.h"

namespace eanim
{

SetCurrFrameSJ* SetCurrFrameSJ::m_instance = NULL;

SetCurrFrameSJ::SetCurrFrameSJ(int id)
	: Subject(id)
{
}

void SetCurrFrameSJ::Set(int layer, int frame)
{
	CurrFrame cf;
	cf.layer = layer;
	cf.frame = frame;
	Notify(&cf);
}

SetCurrFrameSJ* SetCurrFrameSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SetCurrFrameSJ(MSG_SET_CURR_FRAME);
	}
	return m_instance;
}

}