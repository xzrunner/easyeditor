#ifndef _EASYANIM_LAYERS_MGR_H_
#define _EASYANIM_LAYERS_MGR_H_

#include <drag2d.h>

namespace eanim
{

class Layer;

class LayersMgr : public d2d::Observer
{
public:
	LayersMgr();
	~LayersMgr();

	void ChangeLayerOrder(int from, int to);

	size_t Size() const {
		return m_layers.size();
	}

	Layer* GetLayer(size_t index) const;
	int QueryIndex(const Layer* layer) const;

	int GetFrameCount() const;

	bool Clear();

//		void removeSprite(d2d::ISprite* sprite);

	const std::vector<Layer*>& CetAllLayers() const {
		return m_layers;
	}

	int GetMaxFrame() const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void Insert(Layer* layer);
	void Remove();

private:
	std::vector<Layer*> m_layers;

}; // LayersMgr

}

#endif // _EASYANIM_LAYERS_MGR_H_