#include "InsertLayerSJ.h"
#include "message_id.h"

namespace eanim
{

InsertLayerSJ* InsertLayerSJ::m_instance = NULL;

InsertLayerSJ::InsertLayerSJ(int id)
	: Subject(id)
{
}

void InsertLayerSJ::Insert()
{
	Notify(NULL);
}

void InsertLayerSJ::Insert(Layer* layer)
{
	Notify(layer);
}

InsertLayerSJ* InsertLayerSJ::Instance()
{
	if (!m_instance) {
		m_instance = new InsertLayerSJ(MSG_INSERT_LAYER);
	}
	return m_instance;
}

}