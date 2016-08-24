#ifndef _EASYTEXT_SPRITE_H_
#define _EASYTEXT_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/Color.h>
#include <sprite2/TextboxSprite.h>

namespace etext
{

class Symbol;

class Sprite : public s2::TextboxSprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }
	virtual bool Update(const s2::RenderParams& params, float dt);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	const std::string& GetText() const { return m_text; }
	void SetText(const std::string& text) { m_text = text; }

	const std::string& GetTID() const { return m_tid; }
	void SetTID(const std::string& tid) { m_tid = tid; }	

	static ee::Sprite* Create(ee::Symbol* sym);

private:
	std::string m_tid;

}; // Sprite

}

#endif // _EASYTEXT_SPRITE_H_