#include "LayersMgr.h"
#include "Layer.h"

#include "message/messages.h"
#include "view/ViewMgr.h"
#include "view/KeysPanel.h"
#include "dataset/DataMgr.h"

#include <algorithm>

namespace eanim
{

LayersMgr::LayersMgr()
{
	RegistSubject(InsertLayerSJ::Instance());
	RegistSubject(RemoveLayerSJ::Instance());
	RegistSubject(ReorderLayerSJ::Instance());
}

LayersMgr::~LayersMgr()
{
	Clear();
}

Layer* LayersMgr::GetLayer(size_t index) const
{
	if (index < 0 || index >= m_layers.size()) return NULL;
	return m_layers[index];
}

int LayersMgr::QueryIndex(const Layer* layer) const
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		if (m_layers[i] == layer) {
			return i;
		}
	}
	return -1;
}

int LayersMgr::GetFrameCount() const
{
	int count = 0;
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		int tmp = layer->GetMaxFrameTime();
		if (tmp > count) count = tmp;
	}
	return count;
}

bool LayersMgr::Clear()
{
	bool ret = !m_layers.empty();
	for_each(m_layers.begin(), m_layers.end(), cu::RemoveRefFunctor<Layer>());
	m_layers.clear();
	return ret;
}

// void LayersMgr::removeSprite(ee::Sprite* spr)
// {
// 	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
// 	{
// 		if (m_layers[i]->removeSprite(spr))
// 			break;
// 	}
// }

int LayersMgr::GetMaxFrameTime() const
{
	int max_frame = -1;
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		max_frame = std::max(max_frame, m_layers[i]->GetMaxFrameTime());
	}
	return max_frame;
}

void LayersMgr::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_INSERT_LAYER:
		{
			Layer* layer = ud ? (Layer*)ud : new Layer;
			m_layers.push_back(layer);
			if (!ud) {
				layer->InsertKeyFrame(1);
			}
		}
		break;
	case MSG_REMOVE_LAYER:
		{
			int layer = *(int*)ud;
			assert(layer >= 0 && layer < m_layers.size());
			delete m_layers[layer];
			m_layers.erase(m_layers.begin() + layer);
		}
		break;
	case MSG_REORDER_LAYER:
		{
			ReorderLayerSJ::Params* p = (ReorderLayerSJ::Params*)ud;
			if (p->from >= 0 && p->from < m_layers.size() &&
				p->to >= 0 && p->to < m_layers.size()) {
				Layer* layer = m_layers[p->from];
				m_layers.erase(m_layers.begin() + p->from);
				m_layers.insert(m_layers.begin() + p->to, layer);
			}
		}
		break;
	}
}

} // eanim