#include "QuerySelectedSpriteLayerSJ.h"
#include "subject_id.h"

namespace d2d
{

QuerySelectedSpriteLayerSJ* QuerySelectedSpriteLayerSJ::m_instance = NULL;

QuerySelectedSpriteLayerSJ::QuerySelectedSpriteLayerSJ(int id)
	: Subject(id)
{
}

int QuerySelectedSpriteLayerSJ::Query()
{
	int layer = INT_MAX;
	Notify(&layer);
	return layer;
}

QuerySelectedSpriteLayerSJ* QuerySelectedSpriteLayerSJ::Instance()
{
	if (!m_instance) {
		m_instance = new QuerySelectedSpriteLayerSJ(MAG_QUERY_SPR_LAYER);
	}
	return m_instance;
}

}