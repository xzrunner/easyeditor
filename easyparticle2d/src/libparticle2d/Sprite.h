#ifndef _EASYPARTICLE2D_SPRITE_H_
#define _EASYPARTICLE2D_SPRITE_H_

#include "Symbol.h"

#include <ee/Sprite.h>

namespace eparticle2d
{

class Sprite : public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// Cloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(const s2::RenderParams& params, float dt);
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	void Draw(const sm::mat4& mt) const;

	void SetMatrix(const sm::mat4& mat) { m_mat = mat; }

	bool GetLoop() const;
	void SetLoop(bool loop);

	bool GetLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

protected:
	Symbol* m_symbol;

private:
	ParticleSystem* m_ps;

	sm::mat4 m_mat;

}; // Sprite

}

#endif // _EASYPARTICLE2D_SPRITE_H_