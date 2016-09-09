#ifndef _EASYPARTICLE3D_SPRITE_H_
#define _EASYPARTICLE3D_SPRITE_H_

#include <ee/Sprite.h>

#include <SM_Quaternion.h>
#include <sprite2/Particle3dSprite.h>
#include <sprite2/Particle3d.h>

struct p3d_sprite;

namespace eparticle3d
{

class Symbol;

class Sprite : public s2::Particle3dSprite, public ee::Sprite
{
public:
	Sprite();
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

	void Start();

	sm::Quaternion& GetDir() { return m_dir; }
	const sm::Quaternion& GetDir() const { return m_dir; }

	bool IsLoop() const;
	bool IsReuse() const { return m_reuse; }

	void OnActive();

	static ee::Sprite* Create(ee::Symbol* sym);

private:
	sm::Quaternion m_dir;

}; // Sprite

}

#endif // _EASYPARTICLE3D_SPRITE_H_