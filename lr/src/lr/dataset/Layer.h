#ifndef _LR_LAYER_H_
#define _LR_LAYER_H_

#include <drag2d.h>

namespace lr
{

class Layer : public d2d::IDataContainer
{
public:
	Layer();
	virtual ~Layer();
	
	//
	// IDataContainer interface
	//
	virtual void Traverse(d2d::IVisitor& visitor, bool order = true) const;
	virtual void Traverse(d2d::IVisitor& visitor, d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;
	virtual bool Remove(Object* obj);
	virtual void Insert(Object* obj);
	virtual void Clear();
	virtual bool ResetOrder(const Object* obj, bool up);

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	bool IsEditable() const { return m_editable; }
	bool IsVisible() const { return m_visible; }

	void SetEditable(bool editable) { m_editable = editable; }
	void SetVisible(bool visible) { m_visible = visible; }

	void LoadFromFile(const Json::Value& val, const std::string& dir);
	void StoreToFile(Json::Value& val, const std::string& dir) const;

	void RefreshViewList(d2d::ViewlistList* list) const;

private:
	std::string m_name;

	bool m_editable, m_visible;

	std::vector<d2d::ISprite*> m_sprs;

}; // Layer

}

#endif // _LR_LAYER_H_