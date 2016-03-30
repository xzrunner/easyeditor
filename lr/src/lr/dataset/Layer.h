#ifndef _LR_LAYER_H_
#define _LR_LAYER_H_

#include <ee/Object.h>
#include <ee/Visitor.h>
#include <ee/DataTraverseType.h>
#include <ee/ObjectVector.h>
#include <ee/LayerMgr.h>
#include <ee/Sprite.h>
#include <ee/Shape.h>
#include <ee/CameraModes.h>

#include <json/json.h>

#include <string>
#include <set>

namespace ee { class Sprite; class Shape; class LayerMgr; }

namespace lr
{

class LibraryPanel;

class Layer : public ee::Object
{
public:
	Layer(int id, LibraryPanel* library, ee::CameraMode cam);
	
	virtual void TraverseSprite(ee::Visitor& visitor, bool order = true) const;
	virtual void TraverseSprite(ee::Visitor& visitor, ee::DataTraverseType type = ee::DT_ALL, bool order = true) const;
	virtual bool RemoveSprite(Object* obj);
	virtual bool InsertSprite(Object* obj, int idx);
	virtual bool ClearSprite();
	virtual bool ResetOrderSprite(const Object* obj, bool up);
	virtual bool ResetOrderSpriteMost(const Object* obj, bool up);

	virtual void TraverseShape(ee::Visitor& visitor, bool order = true) const;
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

	ee::Sprite* QuerySprite(const std::string& name) const;
	ee::Shape* QueryShape(const std::string& name) const;

	int GetID() const { return m_id; }

	ee::LayerMgr* GetLayerMgr() { return &m_layer_mgr; }

private:
	static bool IsValidFloat(float f);

	void LoadSprites(const Json::Value& val, const std::string& dir, 
		const std::string& base_path = "");
	void LoadShapes(const Json::Value& val, const std::string& dir, 
		const std::string& base_path = "");

	void LoadFromBaseFile(int layer_idx, const std::string& filepath, const std::string& dir);

	void CheckSpriteName(ee::Sprite* spr);

	void LoadShapesUD(const Json::Value& spr_val, ee::Sprite* spr) const;
	void StoreShapesUD(ee::Sprite* spr, Json::Value& spr_val) const;

	ee::Sprite* LoadGroup(const Json::Value& val, const std::string& dir, const std::string& base_path);
	void StoreGroup(ee::Sprite* spr, Json::Value& val, const std::string& dir) const;

	ee::Sprite* LoadSprite(const Json::Value& val, const std::string& dir, const std::string& base_path);
	bool StoreSprite(ee::Sprite* spr, Json::Value& val, const std::string& dir) const;
	
private:
	template<typename T>
	class QueryNameVisitor : public ee::Visitor
	{
	public:
		QueryNameVisitor(const std::string& name);
		virtual void Visit(Object* object, bool& next);
		T* GetResult() { return m_result; }
	private:
		std::string m_name;
		T* m_result;
	}; // QueryNameVisitor

protected:
	LibraryPanel* m_library;

private:
	int m_id;

	ee::CameraMode m_cam_mode;

	std::string m_name;

	bool m_editable, m_visible;

	ee::ObjectVector<ee::Sprite> m_sprites;
	ee::ObjectVector<ee::Shape> m_shapes;

	std::string m_base_filepath;

	int m_next_id;
	std::set<std::string> m_name_set;

	ee::LayerMgr m_layer_mgr;

}; // Layer

}

#endif // _LR_LAYER_H_