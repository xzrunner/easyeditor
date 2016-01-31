#ifndef _EASYEDITOR_LAYER_H_
#define _EASYEDITOR_LAYER_H_

#include "Object.h"
#include "ObjectVector.h"
#include "Sprite.h"
#include "Shape.h"

#include <json/json.h>

namespace ee
{	

class Layer : public Object
{
public:
	Layer();
	~Layer();

	void TraverseSprite(Visitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	bool Insert(Sprite* sprite);
	bool Remove(Sprite* sprite);

	void TraverseShape(Visitor& visitor, bool order = true) const;
	bool Insert(Shape* shape);
	bool Remove(Shape* shape);

	void LoadFromFile(const Json::Value& val, const std::string& dir);
	void StoreToFile(Json::Value& val, const std::string& dir) const;

	void Clear();

public:
	std::string name;

	bool visible;
	bool editable;

private:
	ObjectVector<Sprite> m_sprites;
	ObjectVector<Shape> m_shapes;

}; // Layer

}

#endif // _EASYEDITOR_LAYER_H_