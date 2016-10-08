#ifndef _EASYANIM_SPRITE_H_
#define _EASYANIM_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/AnimSprite.h>

namespace eanim
{

class Symbol;

class Sprite : public s2::AnimSprite, public ee::Sprite
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

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	bool IsLoop() const { return m_loop; }
	float GetInterval() const { return m_interval; }

	int GetFPS() const { return m_fps; }

	int GetStartTime() const { return m_start_frame; }

	int GetStaticTime() const { return m_static_time; }
	void SetStaticTime(int static_time);

	bool IsActive() const { return m_curr.IsActive(); }

	static ee::Sprite* Create(ee::Symbol* sym);

private:
	int m_static_time;

}; // Sprite

}

#endif // _EASYANIM_SPRITE_H_