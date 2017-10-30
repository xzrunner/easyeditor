#ifndef _EASYEDITOR_LAYER_H_
#define _EASYEDITOR_LAYER_H_

#include "ObjectRefVector.h"
#include "Sprite.h"
#include "Shape.h"

#include <string>
#include <cu/CU_RefCountObj.h>

#include <json/json.h>

namespace ee
{	

class Layer : public cu::RefCountObj
{
public:
	Layer();
	~Layer();

	void TraverseSprite(RefVisitor<Sprite>& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	bool Insert(const SprPtr& spr);
	bool Remove(const SprPtr& spr);

	void TraverseShape(RefVisitor<Shape>& visitor, bool order = true) const;
	bool Insert(const ShapePtr& shape);
	bool Remove(const ShapePtr& shape);

	void LoadFromFile(const Json::Value& val, const std::string& dir);
	void StoreToFile(Json::Value& val, const std::string& dir) const;

	void Clear();

public:
	std::string name;

	bool visible;
	bool editable;

private:
	ObjectRefVector<Sprite> m_sprs;
	ObjectRefVector<Shape> m_shapes;

}; // Layer

}

#endif // _EASYEDITOR_LAYER_H_