#ifndef _LR_LAYER_H_
#define _LR_LAYER_H_

#include <drag2d.h>

namespace lr
{

class LibraryPanel;

class Layer : public d2d::Object
{
public:
	Layer(LibraryPanel* library);
	
	virtual void TraverseSprite(d2d::IVisitor& visitor, bool order = true) const;
	virtual void TraverseSprite(d2d::IVisitor& visitor, d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;
	virtual bool RemoveSprite(Object* obj);
	virtual bool InsertSprite(Object* obj);
	virtual bool ClearSprite();
	virtual bool ResetOrderSprite(const Object* obj, bool up);

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

private:
	static bool IsValidFloat(float f);

	void LoadSprites(const Json::Value& val, const std::string& dir, 
		const std::string& base_path = "");
	void LoadShapes(const Json::Value& val, const std::string& dir, 
		const std::string& base_path = "");

	void LoadFromBaseFile(int layer_idx, const std::string& filepath, const std::string& dir);

	void CheckSpriteName(d2d::ISprite* spr);

private:
	struct UserData
	{
		std::string base_path;

		UserData(const std::string& path) : base_path(path) {}
	};

	class QuerySpriteVisitor : public d2d::IVisitor
	{
	public:
		QuerySpriteVisitor(const std::string& name);
		virtual void Visit(Object* object, bool& bFetchNext);
		d2d::ISprite* GetSpr() { return m_spr; }
	private:
		std::string m_name;
		d2d::ISprite* m_spr;
	}; // QueryByNameVisitor

protected:
	LibraryPanel* m_library;

private:
	std::string m_name;

	bool m_editable, m_visible;

	d2d::ObjectVector<d2d::ISprite> m_sprites;
	d2d::ObjectVector<d2d::IShape> m_shapes;

	std::string m_base_filepath;

	int m_next_id;
	std::set<std::string> m_name_set;

}; // Layer

}

#endif // _LR_LAYER_H_