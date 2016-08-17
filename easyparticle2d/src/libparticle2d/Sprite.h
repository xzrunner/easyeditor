#ifndef _EASYPARTICLE2D_SPRITE_H_
#define _EASYPARTICLE2D_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/Particle2dSprite.h>

namespace eparticle2d
{

class Symbol;
class ParticleSystem;

class Sprite : public s2::Particle2dSprite, public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite(Symbol* sym);
	virtual ~Sprite();

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual bool Update(const s2::RenderParams& params, float dt);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual Sprite* EEClone() const { return new Sprite(*this); }

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	void Draw(const sm::mat4& mt) const;

	void SetMatrix(const sm::mat4& mat) { m_mat = mat; }

	bool GetLoop() const;
	void SetLoop(bool loop);

	bool GetLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

	static ee::Sprite* Create(ee::Symbol* sym);

private:
	ParticleSystem* m_ps;

	sm::mat4 m_mat;

}; // Sprite

}

#endif // _EASYPARTICLE2D_SPRITE_H_