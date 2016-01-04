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
	virtual bool Update(int version);
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::ISymbol* symbol);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual d2d::IPropertySetting* CreatePropertySetting(d2d::EditPanelImpl* stage);

	void Start();

	void Draw(const d2d::Matrix& mt) const;

	void SetMatrix(const d2d::Matrix& mat) { m_mat = mat; }

	Quaternion& GetDir() { return m_dir; }
	const Quaternion& GetDir() const { return m_dir; }

	bool GetLoop() const;
	void SetLoop(bool loop);

	bool GetLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

	bool GetUseBuffer() const;
	void SetUseBuffer(bool use);

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

protected:
	Symbol* m_symbol;

private:
	ParticleSystem* m_ps;

	d2d::Matrix m_mat;

	Quaternion m_dir;

	bool m_use_buffer;

}; // Sprite

}

#endif // _EASYPARTICLE3D_SPRITE_H_