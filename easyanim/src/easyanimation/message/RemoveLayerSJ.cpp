#include "RemoveLayerSJ.h"
#include "message_id.h"

namespace eanim
{

RemoveLayerSJ* RemoveLayerSJ::m_instance = NULL;

RemoveLayerSJ::RemoveLayerSJ(int id)
	: Subject(id)
{
}

void RemoveLayerSJ::Remove()
{
	Notify(NULL);
}

RemoveLayerSJ* RemoveLayerSJ::Instance()
{
	if (!m_instance) {
		m_instance = new RemoveLayerSJ(MSG_REMOVE_LAYER);
	}
	return m_instance;
}

}