#ifndef _EASYEDITOR_SPRITE_H_
#define _EASYEDITOR_SPRITE_H_

#include "Cloneable.h"
#include "UserDataImpl.h"
#include "Object.h"

#include <SM_Matrix.h>
#include <sprite2/Sprite.h>

#include <json/json.h>

namespace ee
{

class Rect;
class Symbol;
class BoundingBox;
class PropertySetting;
class EditPanelImpl;
class SpriteObserver;

class Sprite : public s2::Sprite, public Cloneable, public UserDataImpl, public Object
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	virtual ~Sprite();

	//
	// Cloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// UserDataImpl interface
	//	
	virtual void ClearUserData(bool deletePtr);

	virtual const Symbol& GetSymbol() const = 0;
	virtual void SetSymbol(Symbol* symbol) = 0;

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual void BuildBounding();

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	virtual void SetTransform(const sm::vec2& position, float angle);
	const sm::vec2& GetPosition() const { return m_position; }
	float GetAngle() const { return m_angle; }

	const sm::vec2& GetScale() const { return m_scale; }
	void SetScale(const sm::vec2& scale);

	const sm::vec2& GetShear() const { return m_shear; }
	void SetShear(const sm::vec2& shear);

	const sm::vec2& GetOffset() const { return m_offset; }
	void SetOffset(const sm::vec2& offset);

	bool IsContain(const sm::vec2& pos) const;
	bool IsIntersect(const Rect& rect) const;

	void Translate(const sm::vec2& offset);
	void Rotate(float delta);

	void SetMirror(bool x_mirror, bool y_mirror);
	const sm::bvec2& GetMirror() const { return m_mirror; }

	const sm::vec2& GetPerspective() const { return m_perspective; }
	void SetPerspective(const sm::vec2& perspective) { m_perspective = perspective; }

	sm::vec2 GetCenter() const;

	const s2::RenderColor& GetColor() const { return m_color; }
	s2::RenderColor& GetColor() { return m_color; }
	const s2::RenderShader& GetShader() const { return m_shader; }
	s2::RenderShader& GetShader() { return m_shader; }
	const s2::RenderCamera& GetCamera() const { return m_camera; }

	BoundingBox* GetBounding() const {
		return m_bounding;
	}
	Rect GetRect() const;

	void SetObserver(SpriteObserver* observer) {
		m_observer = observer;
	}

	void GetTransMatrix(sm::mat4& mt) const;
	sm::mat4 GetTransInvMatrix() const;

	bool IsAnchor() const { return m_is_anchor; }
	void SetAnchor(bool anchor) { m_is_anchor = anchor; }

protected:
	template<typename T>
	void SetSymbol(T** dst, Symbol* src);

private:
	const Sprite& operator = (const Sprite& spr) { return spr; }

public:
	// info
	std::string name;
	std::string tag;
	bool clip;

	// edit
	bool visiable;
	bool editable;

protected:
	sm::vec2	m_offset;
	bool		m_offset_valid;
	sm::bvec2	m_mirror;
	sm::vec2	m_perspective;

	BoundingBox* m_bounding;
	
	SpriteObserver* m_observer;

private:
	bool m_is_anchor;

}; // Sprite

template<typename T>
void Sprite::SetSymbol(T** dst, Symbol* src)
{
	if (src == *dst) {
		return;
	}
	T* sym = dynamic_cast<T*>(src);
	if (!sym) {
		return;
	}
	obj_assign<T>(*dst, sym);
	BuildBounding();
}

class SpriteCmp
{
public:
	enum Type
	{
		e_file,
		e_x,
		e_y,
		e_x_invert,
		e_y_invert
	};

public:
	SpriteCmp(Type type = e_file);

	bool operator() (const Sprite* s0, const Sprite* s1) const;

private:
	Type m_type;

}; // SpriteCmp

}

#endif // _EASYEDITOR_SPRITE_H_