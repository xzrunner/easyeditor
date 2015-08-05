#ifndef _EASYPARTICLE3D_SPRITE_H_
#define _EASYPARTICLE3D_SPRITE_H_

#include "Symbol.h"

#include <easy3d.h>

namespace eparticle3d
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// ICloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// ISprite interface
	//
	virtual bool Update(int version) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::ISymbol* symbol);
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

	Quaternion& GetDir() { return m_dir; }
	const Quaternion& GetDir() const { return m_dir; }

protected:
	Symbol* m_symbol;

private:
	Quaternion m_dir;

}; // Sprite

}

#endif // _EASYPARTICLE3D_SPRITE_H_