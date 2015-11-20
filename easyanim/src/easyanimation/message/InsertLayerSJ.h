#ifndef _EASYANIM_INSERT_LAYER_SJ_H_
#define _EASYANIM_INSERT_LAYER_SJ_H_

#include <drag2d.h>

namespace eanim
{

class Layer;

class InsertLayerSJ : public d2d::Subject
{
public:
	void Insert();
	void Insert(Layer* layer);

	static InsertLayerSJ* Instance();

private:
	InsertLayerSJ(int id);

private:
	static InsertLayerSJ* m_instance;

}; // InsertLayerSJ

}

#endif // _EASYANIM_INSERT_LAYER_SJ_H_