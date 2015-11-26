#include "LayersMgr.h"
#include "Layer.h"

#include "frame/Controller.h"

#include "message/message_id.h"
#include "message/InsertLayerSJ.h"
#include "message/RemoveLayerSJ.h"

namespace eanim
{

LayersMgr::LayersMgr(Controller* ctrl)
	: m_ctrl(ctrl)
{
	InsertLayerSJ::Instance()->Register(this);
	RemoveLayerSJ::Instance()->Register(this);
}

LayersMgr::~LayersMgr()
{
	clear();

	InsertLayerSJ::Instance()->UnRegister(this);
	RemoveLayerSJ::Instance()->UnRegister(this);
}

void LayersMgr::Notify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_INSERT_LAYER:
		{
			Layer* layer = ud ? (Layer*)ud : new Layer(m_ctrl);
			if (!ud) {
				layer->InsertKeyFrame(1);
			}
			Insert(layer);
		}
		break;
	case MSG_REMOVE_LAYER:
		Remove();
		break;
	}
}

void LayersMgr::changeLayerOrder(int from, int to)
{
	if (from < 0 || from >= m_layers.size()
		|| to < 0 || to >= m_layers.size()) 
		return;

	Layer* layer = m_layers[from];
	m_layers.erase(m_layers.begin() + from);
	m_layers.insert(m_layers.begin() + to, layer);
}

Layer* LayersMgr::getLayer(size_t index) const
{
	if (index < 0 || index >= m_layers.size()) return NULL;
	return m_layers[index];
}

int LayersMgr::getFrameCount() const
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

bool LayersMgr::clear()
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

void LayersMgr::Insert(Layer* layer)
{
	//int curr_idx = m_layers.size();
	//Layer* layer = m_layers.newLayer();
	//setCurrFrame(curr_idx, frame());
	//layer->InsertKeyFrame(1);
	//GetStagePanel()->d2d::SetCanvasDirtySJ::Instance()->SetDirty();

	m_ctrl->setCurrFrame(m_layers.size(), 1);
	m_layers.push_back(layer);
}

void LayersMgr::Remove()
{
	int idx = m_ctrl->layer();
	assert(idx >= 0 && idx < m_layers.size());
	delete m_layers[idx];
	m_layers.erase(m_layers.begin() + idx);
	if (m_layers.size() == 0) {
		m_ctrl->setCurrFrame(-1, m_ctrl->frame());
	} else if (m_ctrl->layer() > 0) {
		m_ctrl->setCurrFrame(m_ctrl->layer() - 1, m_ctrl->frame());
	}
}

} // eanim