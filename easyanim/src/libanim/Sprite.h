#ifndef _LIBANIM_SPRITE_H_
#define _LIBANIM_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/AnimSprite.h>

namespace libanim
{

class Symbol;

class Sprite : public s2::AnimSprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);


	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const CU_STR& dir = "");
	virtual void Store(Json::Value& val, const CU_STR& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	bool IsLoop() const { return m_loop; }
	float GetInterval() const { return m_interval; }

	int GetFPS() const { return m_fps; }

	bool IsStartRandom() const { return m_start_random; }

	int GetStaticTime() const { return m_static_time; }
	void SetStaticTime(int static_time);

	bool IsActive() const;

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

private:
	int m_static_time;

	SPRITE_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _LIBANIM_SPRITE_H_