#ifndef _DRAG2D_LAYER_MGR_H_
#define _DRAG2D_LAYER_MGR_H_

#include "view/DataTraverseType.h"

#include <vector>
#include <json/json.h>

namespace d2d
{	

class Layer;
class IVisitor;

class LayerMgr
{
public:
	LayerMgr();
	~LayerMgr();

	void TraverseSprite(IVisitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;

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

#endif // _DRAG2D_LAYER_MGR_H_