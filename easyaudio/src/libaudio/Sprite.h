#ifndef _EASYAUDIO_SPRITE_H_
#define _EASYAUDIO_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/AudioSprite.h>

namespace eaudio
{

class Symbol;

class Sprite : public s2::AudioSprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	virtual bool Update(const s2::UpdateParams& up) override;

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "") override;
	virtual void Store(Json::Value& val, const std::string& dir = "") const override;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override;

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

private:
	bool LoadResourcesStatic(const std::string& filepath);
	bool LoadResourcesStream(const std::string& filepath);

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYAUDIO_SPRITE_H_