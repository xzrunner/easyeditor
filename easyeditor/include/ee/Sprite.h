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

	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
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
	/* extend                                                               */
	/************************************************************************/
	void SetObserver(SpriteObserver* observer) {
		m_observer = observer;
	}

private:
	class PartialUpdate
	{
	public:
		PartialUpdate() : m_find(false) {}
		void Begin();
		void End();
	private:
		bool m_find;
	}; // PartialUpdate

private:
	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
	sm::vec2			m_perspective;

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	std::string			m_tag;
	bool				m_clip;
	bool				m_anchor;

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