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
	Sprite();
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	virtual bool Update(const s2::UpdateParams& up);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const CU_STR& dir = "");
	virtual void Store(Json::Value& val, const CU_STR& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	const std::string& GetTID() const { return m_tid; }
	void SetTID(const std::string& tid) { m_tid = tid; }	

	bool IsExport() const { return m_export; }
	void SetExport(bool exp) { m_export = exp; }

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

private:
	std::string m_tid;

	bool m_export;

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYTEXT_SPRITE_H_