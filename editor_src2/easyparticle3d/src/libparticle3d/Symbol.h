#ifndef _EASYPARTICLE3D_SYMBOL_H_
#define _EASYPARTICLE3D_SYMBOL_H_

#include <drag2d.h>

namespace eparticle3d
{

class ParticleSystem;

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* clone() const;

	//
	// ISerializable interface
	//
	virtual void loadFromTextFile(std::ifstream& fin) {}
	virtual void storeToTextFile(std::ofstream& fout) const {}

	//
	// ISymbol interfaces
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	//
	// ListItem interface
	//
	virtual void refresh();
	
protected:
	virtual void loadResources();

private:
	ParticleSystem* m_ps;

	d2d::Rect m_region;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_SYMBOL_H_
