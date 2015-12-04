#include "LayersMgr.h"
#include "Layer.h"

#include "message/messages.h"
#include "view/view_utility.h"

namespace eanim
{

LayersMgr::LayersMgr()
{
	RegistSubject(InsertLayerSJ::Instance());
	RegistSubject(RemoveLayerSJ::Instance());
}

LayersMgr::~LayersMgr()
{
	Clear();
}

void LayersMgr::ChangeLayerOrder(int from, int to)
{
	if (from < 0 || from >= m_layers.size()
		|| to < 0 || to >= m_layers.size()) {
		return;
	}

	Layer* layer = m_layers[from];
	m_layers.erase(m_layers.begin() + from);
	m_layers.insert(m_layers.begin() + to, layer);
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
	for_each(m_layers.begin(), m_layers.end(), DeletePointerFunctor<Layer>());
	m_layers.clear();
	return ret;
}

// void LayersMgr::removeSprite(d2d::ISprite* sprite)
// {
// 	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
// 	{
// 		if (m_layers[i]->removeSprite(sprite))
// 			break;
// 	}
// }

int LayersMgr::GetMaxFrame() const
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
			Insert(layer);
			if (!ud) {
				layer->InsertKeyFrame(1);
			}
		}
		break;
	case MSG_REMOVE_LAYER:
		Remove();
		break;
	}
}

void LayersMgr::Insert(Layer* layer)
{
	m_layers.push_back(layer);
	SetCurrFrameSJ::Instance()->Set(m_layers.size() - 1, 0);
}

void LayersMgr::Remove()
{
	int layer = get_curr_layer_index();
	assert(layer >= 0 && layer < m_layers.size());
	delete m_layers[layer];
	m_layers.erase(m_layers.begin() + layer);

	if (layer > 0) {
		SetCurrFrameSJ::Instance()->Set(layer - 1, -1);
	} else if (layer == 0) {
		SetCurrFrameSJ::Instance()->Set(1, -1);
	}
}

} // eanim