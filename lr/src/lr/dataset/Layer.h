#ifndef _LR_LAYER_H_
#define _LR_LAYER_H_

#include <drag2d.h>

namespace lr
{

class LibraryPanel;

class Layer : public d2d::Object
{
public:
	Layer(int id, LibraryPanel* library);
	
	virtual void TraverseSprite(d2d::IVisitor& visitor, bool order = true) const;
	virtual void TraverseSprite(d2d::IVisitor& visitor, d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;
	virtual bool RemoveSprite(Object* obj);
	virtual bool InsertSprite(Object* obj, int idx);
	virtual bool ClearSprite();
	virtual bool ResetOrderSprite(const Object* obj, bool up);
	virtual bool ResetOrderSpriteMost(const Object* obj, bool up);

	virtual void TraverseShape(d2d::IVisitor& visitor, bool order = true) const;
	virtual bool RemoveShape(Object* obj);
	virtual bool InsertShape(Object* obj);
	virtual bool ClearShape();

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	bool IsEditable() const { return m_editable; }
	bool IsVisible() const { return m_visible; }

	void SetEditable(bool editable) { m_editable = editable; }
	void SetVisible(bool visible) { m_visible = visible; }

	void LoadFromFile(const Json::Value& val, const std::string& dir, int layer_idx);
	void StoreToFile(Json::Value& val, const std::string& dir) const;

	bool Update(int version);

	d2d::ISprite* QuerySprite(const std::string& name) const;
	d2d::IShape* QueryShape(const std::string& name) const;

	int GetID() const { return m_id; }

	d2d::LayerMgr* GetLayerMgr() { return &m_layer_mgr; }

private:
	static bool IsValidFloat(float f);

	void LoadSprites(const Json::Value& val, const std::string& dir, 
		const std::string& base_path = "");
	void LoadShapes(const Json::Value& val, const std::string& dir, 
		const std::string& base_path = "");

	void LoadFromBaseFile(int layer_idx, const std::string& filepath, const std::string& dir);

	void CheckSpriteName(d2d::ISprite* spr);

	void LoadShapesUD(const Json::Value& spr_val, d2d::ISprite* spr) const;
	void StoreShapesUD(d2d::ISprite* spr, Json::Value& spr_val) const;
	
private:
	template<typename T>
	class QueryNameVisitor : public d2d::IVisitor
	{
	public:
		QueryNameVisitor(const std::string& name);
		virtual void Visit(Object* object, bool& bFetchNext);
		T* GetResult() { return m_result; }
	private:
		std::string m_name;
		T* m_result;
	}; // QueryNameVisitor

protected:
	LibraryPanel* m_library;

private:
	int m_id;

	std::string m_name;

	bool m_editable, m_visible;

	d2d::ObjectVector<d2d::ISprite> m_sprites;
	d2d::ObjectVector<d2d::IShape> m_shapes;

	std::string m_base_filepath;

	int m_next_id;
	std::set<std::string> m_name_set;

	d2d::LayerMgr m_layer_mgr;

}; // Layer

}

#endif // _LR_LAYER_H_