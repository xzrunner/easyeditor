#ifndef _DRAG2D_GROUP_H_
#define _DRAG2D_GROUP_H_

#include "common/Object.h"
#include "view/ObjectVector.h"
#include "dataset/ISprite.h"
#include "dataset/IShape.h"

namespace d2d
{

class Group : public Object
{
public:
	Group(const std::string& name);
	virtual ~Group();

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	void TraverseSprite(IVisitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;

	bool Insert(ISprite* sprite);
	bool Remove(ISprite* sprite);

	void Clear();

private:
	std::string m_name;

	ObjectVector<ISprite> m_sprites;
	ObjectVector<IShape> m_shapes;

}; // Group

}

#endif // _DRAG2D_GROUP_H_