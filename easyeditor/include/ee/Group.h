#ifndef _EASYEDITOR_GROUP_H_
#define _EASYEDITOR_GROUP_H_

#include <cu/CU_RefCountObj.h>

#include "ObjectRefVector.h"
#include "Sprite.h"
#include "Shape.h"
#include "Visitor.h"

namespace ee
{

class Group : public cu::RefCountObj
{
public:
	Group(const std::string& name, bool visible = true, bool editable = true);
	virtual ~Group();

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	void TraverseSprite(RefVisitor<Sprite>& visitor, DataTraverseType type = DT_ALL, bool order = true) const;

	bool Insert(const SprPtr& spr);
	bool Remove(const SprPtr& spr);

	void Clear();

	bool GetEditable() const { return m_editable; }
	void SetEditable(bool editable) { m_editable = editable; }

	bool GetVisible() const { return m_visible; }
	void SetVisible(bool visible) { m_visible = visible; }

private:
	std::string m_name;

	bool m_visible;
	bool m_editable;

	ObjectRefVector<Sprite> m_sprs;
	ObjectRefVector<Shape> m_shapes;

}; // Group

}

#endif // _EASYEDITOR_GROUP_H_