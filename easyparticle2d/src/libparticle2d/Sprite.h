#ifndef _EASYPARTICLE2D_SPRITE_H_
#define _EASYPARTICLE2D_SPRITE_H_

#include "Symbol.h"

#include <ee/Sprite.h>
#include <ee/Matrix.h>

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
	virtual bool Update(int version);
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	void Draw(const ee::Matrix& mt) const;

	void SetMatrix(const ee::Matrix& mat) { m_mat = mat; }

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

	ee::Matrix m_mat;

}; // Sprite

}

#endif // _EASYPARTICLE2D_SPRITE_H_