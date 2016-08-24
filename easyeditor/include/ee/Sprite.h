#ifndef _EASYEDITOR_SPRITE_H_
#define _EASYEDITOR_SPRITE_H_

#include "UserDataImpl.h"

#include <SM_Matrix.h>
#include <SM_Rect.h>
#include <sprite2/S2_Sprite.h>

#include <json/json.h>

namespace ee
{

class Symbol;
class PropertySetting;
class EditPanelImpl;
class SpriteObserver;

class Sprite : public virtual s2::Sprite, public UserDataImpl
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);
	virtual ~Sprite();

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual void SetSymbol(Symbol* sym);

	/**
	 *  @interface
	 *    UserDataImpl
	 */
	virtual void ClearUserData(bool deletePtr);

	virtual void Translate(const sm::vec2& offset);
	virtual void Rotate(float delta);

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
	const sm::bvec2& GetMirror() const { return m_mirror; }
	void SetMirror(const sm::bvec2& mirror);

	const sm::vec2& GetPerspective() const { return m_perspective; }
	void SetPerspective(const sm::vec2& perspective) { m_perspective = perspective; }

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	const std::string& GetTag() const { return m_tag; }
	void SetTag(const std::string& tag) { m_tag = tag; }

	bool IsClip() const { return m_clip; }
	void SetClip(bool clip) { m_clip = clip; }

	bool IsAnchor() const { return m_anchor; }
	void SetAnchor(bool anchor) { m_anchor = anchor; }

	/************************************************************************/
	/* edit                                                                 */
	/************************************************************************/
	bool IsEditable() const { return m_editable; }
	void SetEditable(bool editable) { m_editable = editable; }

	/************************************************************************/
	/* extend                                                               */
	/************************************************************************/
	void SetObserver(SpriteObserver* observer) {
		m_observer = observer;
	}

private:
	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
	sm::bvec2			m_mirror;
	sm::vec2			m_perspective;

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	std::string			m_tag;
	bool				m_clip;
	bool				m_anchor;

	/************************************************************************/
	/* edit                                                                 */
	/************************************************************************/
	bool				m_editable;

	/************************************************************************/
	/* extend                                                               */
	/************************************************************************/
	SpriteObserver*		m_observer;

}; // Sprite

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