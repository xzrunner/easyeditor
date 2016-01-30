#ifndef _DRAG2D_LAYER_H_
#define _DRAG2D_LAYER_H_

#include "common/Object.h"
#include "view/ObjectVector.h"

#include "Sprite.h"
#include "Shape.h"

#include <json/json.h>

namespace d2d
{	

class Layer : public Object
{
public:
	Layer();
	~Layer();

	void TraverseSprite(IVisitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	bool Insert(Sprite* sprite);
	bool Remove(Sprite* sprite);

	void TraverseShape(IVisitor& visitor, bool order = true) const;
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

#endif // _DRAG2D_LAYER_H_