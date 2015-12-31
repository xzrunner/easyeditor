#ifndef _DRAG2D_LAYER_H_
#define _DRAG2D_LAYER_H_

#include "common/Object.h"
#include "view/ObjectVector.h"

#include "ISprite.h"
#include "IShape.h"

#include <json/json.h>

namespace d2d
{	

class Layer : public Object
{
public:
	Layer();
	~Layer();

	void TraverseSprite(IVisitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	bool Insert(ISprite* sprite);
	bool Remove(ISprite* sprite);

	void TraverseShape(IVisitor& visitor, bool order = true) const;
	bool Insert(IShape* shape);
	bool Remove(IShape* shape);

	void LoadFromFile(const Json::Value& val, const std::string& dir);
	void StoreToFile(Json::Value& val, const std::string& dir) const;

	void Clear();

public:
	std::string name;

	bool visible;
	bool editable;

private:
	ObjectVector<ISprite> m_sprites;
	ObjectVector<IShape> m_shapes;

}; // Layer

}

#endif // _DRAG2D_LAYER_H_