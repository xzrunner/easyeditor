#include "LayerMgr.h"
#include "Layer.h"

#include <algorithm>

namespace d2d
{

LayerMgr::LayerMgr()
	: selected(NULL)
{
}

LayerMgr::~LayerMgr()
{
	Clear();
}

void LayerMgr::TraverseSprite(IVisitor& visitor, DataTraverseType type, bool order) const
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		m_layers[i]->TraverseSprite(visitor, type, order);
	}
}

void LayerMgr::Insert(Layer* layer)
{
	if (!selected) {
		selected = layer;
	}

	layer->Retain();
	m_layers.push_back(layer);
}

void LayerMgr::Remove(int idx)
{
	assert(idx >= 0 && idx < m_layers.size());
	m_layers[idx]->Release();
	m_layers.erase(m_layers.begin() + idx);
}

Layer* LayerMgr::GetLayer(int idx)
{
	if (idx >= 0 && idx < m_layers.size()) {
		return m_layers[idx];
	} else {
		return NULL;
	}
}

void LayerMgr::Clear()
{
	for_each(m_layers.begin(), m_layers.end(), ReleaseObjectFunctor<Layer>());
}

void LayerMgr::LoadFromFile(const Json::Value& val, const std::string& dir)
{
	int idx = 0;
	Json::Value l_val = val[idx++];
	while (!l_val.isNull()) {
		d2d::Layer* layer = new d2d::Layer;
		layer->LoadFromFile(l_val, dir);
		Insert(layer);
		layer->Release();
		l_val = val[idx++];
	}
}

void LayerMgr::StoreToFile(Json::Value& val, const std::string& dir) const
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		m_layers[i]->StoreToFile(val[i], dir);
	}	
}

}