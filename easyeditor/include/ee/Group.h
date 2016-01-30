#ifndef _DRAG2D_GROUP_H_
#define _DRAG2D_GROUP_H_

#include "common/Object.h"
#include "view/ObjectVector.h"
#include "dataset/Sprite.h"
#include "dataset/Shape.h"

namespace d2d
{

class Group : public Object
{
public:
	Group(const std::string& name, bool visible = true, bool editable = true);
	virtual ~Group();

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	void TraverseSprite(IVisitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;

	bool Insert(Sprite* sprite);
	bool Remove(Sprite* sprite);

	void Clear();

	bool GetEditable() const { return m_editable; }
	void SetEditable(bool editable) { m_editable = editable; }

	bool GetVisible() const { return m_visible; }
	void SetVisible(bool visible) { m_visible = visible; }

private:
	std::string m_name;

	bool m_visible;
	bool m_editable;

	ObjectVector<Sprite> m_sprites;
	ObjectVector<Shape> m_shapes;

}; // Group

}

#endif // _DRAG2D_GROUP_H_