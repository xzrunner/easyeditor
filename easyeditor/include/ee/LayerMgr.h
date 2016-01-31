#ifndef _EASYEDITOR_LAYER_MGR_H_
#define _EASYEDITOR_LAYER_MGR_H_

#include "DataTraverseType.h"

#include <json/json.h>

#include <vector>

namespace ee
{	

class Layer;
class Visitor;

class LayerMgr
{
public:
	LayerMgr();
	~LayerMgr();

	void TraverseSprite(Visitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	void TraverseShape(Visitor& visitor, bool order = true) const;

	const std::vector<Layer*>& GetAllLayers() const { return m_layers; }

	void Insert(Layer* layer);
	void Remove(int idx);

	Layer* GetLayer(int idx);

	void Clear();

	void LoadFromFile(const Json::Value& val, const std::string& dir);
	void StoreToFile(Json::Value& val, const std::string& dir) const;

public:
	Layer* selected;

private:
	std::vector<Layer*> m_layers;

}; // LayerMgr

}

#endif // _EASYEDITOR_LAYER_MGR_H_