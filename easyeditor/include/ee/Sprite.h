#ifndef _EASYEDITOR_SPRITE_H_
#define _EASYEDITOR_SPRITE_H_

#include "Cloneable.h"
#include "UserDataImpl.h"
#include "Object.h"

#include <SM_Matrix.h>
#include <SM_Rect.h>

#include <json/json.h>

namespace s2 { class Sprite; class RenderColor; class RenderShader; class RenderCamera; }

namespace ee
{

class Symbol;
class BoundingBox;
class PropertySetting;
class EditPanelImpl;
class SpriteObserver;

class Sprite : public Cloneable, public UserDataImpl, public Object
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

	virtual bool Update(float dt) = 0;

	virtual const Symbol& GetSymbol() const = 0;
	virtual void SetSymbol(Symbol* symbol) = 0;

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual void BuildBounding();

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	/************************************************************************/
	/* core                                                                 */
	/************************************************************************/
	const s2::Sprite* GetCore() const { return m_core; }
	s2::Sprite* GetCore() { return m_core; }

	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
	void SetTransform(const sm::vec2& position, float angle);
	const sm::vec2& GetPosition() const;
	float GetAngle() const;

	void Translate(const sm::vec2& offset);
	void Rotate(float delta);

	const sm::vec2& GetScale() const;
	void SetScale(const sm::vec2& scale);

	const sm::vec2& GetShear() const;
	void SetShear(const sm::vec2& shear);

	const sm::vec2& GetOffset() const { return m_offset; }
	void SetOffset(const sm::vec2& offset);

	void SetMirror(bool x_mirror, bool y_mirror);
	const sm::bvec2& GetMirror() const { return m_mirror; }

	const sm::vec2& GetPerspective() const { return m_perspective; }
	void SetPerspective(const sm::vec2& perspective) { m_perspective = perspective; }

	bool IsContain(const sm::vec2& pos) const;
	bool IsIntersect(const sm::rect& rect) const;

	sm::vec2 GetCenter() const;
	sm::rect GetRect() const;

	BoundingBox* GetBounding() const { return m_bounding; }
	void SetBounding(BoundingBox* bb);

	void GetTransMatrix(sm::mat4& mt) const;
	sm::mat4 GetTransInvMatrix() const;

	/************************************************************************/
	/* render                                                               */
	/************************************************************************/
	const s2::RenderColor& GetColor() const;
	s2::RenderColor& GetColor();
	const s2::RenderShader& GetShader() const;
	s2::RenderShader& GetShader();
	const s2::RenderCamera& GetCamera() const;
	s2::RenderCamera& GetCamera();

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetTag() const { return m_tag; }
	void SetTag(const std::string& tag) { m_tag = tag; }

	bool IsClip() const { return m_clip; }
	void SetClip(bool clip) { m_clip = clip; }

	bool IsAnchor() const { return m_is_anchor; }
	void SetAnchor(bool anchor) { m_is_anchor = anchor; }

	/************************************************************************/
	/* edit                                                                 */
	/************************************************************************/
	bool IsVisible() const { return m_visible; }
	void SetVisible(bool visible) { m_visible = visible; }
	bool IsEditable() const { return m_editable; }
	void SetEditable(bool editable) { m_editable = editable; }

	/************************************************************************/
	/* extend                                                               */
	/************************************************************************/
	void SetObserver(SpriteObserver* observer) {
		m_observer = observer;
	}

protected:
	template<typename T>
	void SetSymbol(T** dst, Symbol* src);

private:
	const Sprite& operator = (const Sprite& spr) { return spr; }

protected:
	/************************************************************************/
	/* core                                                                 */
	/************************************************************************/
	s2::Sprite*		m_core;

private:
	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
	sm::vec2		m_offset;
	sm::bvec2		m_mirror;
	sm::vec2		m_perspective;
	BoundingBox*	m_bounding;

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	std::string		m_name;
	std::string		m_tag;
	bool			m_clip;
	bool			m_is_anchor;

	/************************************************************************/
	/* edit                                                                 */
	/************************************************************************/
	bool			m_visible;
	bool			m_editable;

	/************************************************************************/
	/* extend                                                               */
	/************************************************************************/
	SpriteObserver* m_observer;

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