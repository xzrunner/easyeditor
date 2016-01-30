#ifndef _EASYPARTICLE2D_SPRITE_H_
#define _EASYPARTICLE2D_SPRITE_H_

#include "Symbol.h"

namespace eparticle2d
{

class Sprite : public d2d::Sprite
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
	virtual void SetSymbol(d2d::Symbol* symbol);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual d2d::IPropertySetting* CreatePropertySetting(d2d::EditPanelImpl* stage);

	void Draw(const d2d::Matrix& mt) const;

	void SetMatrix(const d2d::Matrix& mat) { m_mat = mat; }

	bool GetLoop() const;
	void SetLoop(bool loop);

	bool GetLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

	static d2d::Sprite* Create(d2d::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

protected:
	Symbol* m_symbol;

private:
	ParticleSystem* m_ps;

	d2d::Matrix m_mat;

}; // Sprite

}

#endif // _EASYPARTICLE2D_SPRITE_H_