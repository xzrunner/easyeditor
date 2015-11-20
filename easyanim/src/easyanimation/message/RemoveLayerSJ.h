#ifndef _EASYANIM_REMOVE_LAYER_SJ_H_
#define _EASYANIM_REMOVE_LAYER_SJ_H_

#include <drag2d.h>

namespace eanim
{

class RemoveLayerSJ : public d2d::Subject
{
public:
	void Remove();

	static RemoveLayerSJ* Instance();

private:
	RemoveLayerSJ(int id);

private:
	static RemoveLayerSJ* m_instance;

}; // RemoveLayerSJ

}

#endif // _EASYANIM_REMOVE_LAYER_SJ_H_