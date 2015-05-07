#ifndef _LR_LAYER_H_
#define _LR_LAYER_H_

#include <drag2d.h>

namespace lr
{

class Layer : public d2d::IDataContainer
{
public:
	Layer();
	
	//
	// IDataContainer interface
	//
	virtual void traverse(d2d::IVisitor& visitor, bool order = true) const;
	virtual void traverse(d2d::IVisitor& visitor, d2d::TraverseType type = d2d::e_allExisting, bool order = true) const;
	virtual void remove(Object* obj);
	virtual void insert(Object* obj);
	virtual void clear();
	virtual void resetOrder(const Object* obj, bool up);
	
private:
	std::vector<d2d::ISprite*> m_sprs;

}; // Layer

}

#endif // _LR_LAYER_H_