#ifndef _LR_LAYER_H_
#define _LR_LAYER_H_

#include <drag2d.h>

namespace lr
{

class Layer : public d2d::Object
{
public:
	Layer();
	virtual ~Layer();
	
	virtual void TraverseSprite(d2d::IVisitor& visitor, bool order = true) const;
	virtual void TraverseSprite(d2d::IVisitor& visitor, d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;
	virtual bool RemoveSprite(Object* obj);
	virtual void InsertSprite(Object* obj);
	virtual void ClearSprite();
	virtual bool ResetOrderSprite(const Object* obj, bool up);

	virtual void TraverseShape(d2d::IVisitor& visitor, bool order = true) const;
	virtual bool RemoveShape(Object* obj);
	virtual void InsertShape(Object* obj);
	virtual void ClearShape();

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	bool IsEditable() const { return m_editable; }
	bool IsVisible() const { return m_visible; }

	void SetEditable(bool editable) { m_editable = editable; }
	void SetVisible(bool visible) { m_visible = visible; }

	void LoadFromFile(const Json::Value& val, const std::string& dir);
	void StoreToFile(Json::Value& val, const std::string& dir) const;

private:
	static bool IsValidFloat(float f);

private:
	std::string m_name;

	bool m_editable, m_visible;

	d2d::ObjectVector<d2d::ISprite> m_sprites;
	d2d::ObjectVector<d2d::IShape> m_shapes;

}; // Layer

}

#endif // _LR_LAYER_H_