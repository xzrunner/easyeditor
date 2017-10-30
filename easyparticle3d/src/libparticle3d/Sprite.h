#ifndef _EASYPARTICLE3D_SPRITE_H_
#define _EASYPARTICLE3D_SPRITE_H_

#include <ee/Sprite.h>

#include <SM_Quaternion.h>
#include <sprite2/Particle3dSprite.h>
#include <sprite2/Particle3d.h>

namespace eparticle3d
{

class Symbol;

class Sprite : public s2::Particle3dSprite, public ee::Sprite
{
public:
	Sprite();
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	sm::Quaternion& GetDir() { return m_dir; }
	const sm::Quaternion& GetDir() const { return m_dir; }

// 	bool IsReuse() const { return m_reuse; }

	void OnActive();

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

private:
	sm::Quaternion m_dir;

	SPRITE_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYPARTICLE3D_SPRITE_H_