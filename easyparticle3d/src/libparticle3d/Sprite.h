#ifndef _EASYPARTICLE3D_SPRITE_H_
#define _EASYPARTICLE3D_SPRITE_H_

#include "Symbol.h"
#include "PS.h"

#include <ee/Sprite.h>

#include <SM_Quaternion.h>

namespace eparticle3d
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
	virtual bool Update(float dt);
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	void Start();

	void Draw(const sm::mat4& mt) const;

	void SetMatrix(const sm::mat4& mat);

	sm::Quaternion& GetDir() { return m_dir; }
	const sm::Quaternion& GetDir() const { return m_dir; }

	bool IsLoop() const;
	void SetLoop(bool loop);

	bool IsLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

	bool IsAlone() const { return m_alone; }
	void SetAlone(bool alone);

	bool IsReuse() const { return m_reuse; }
	void SetReuse(bool reuse);

	p3d_sprite* GetP3D() const { return m_spr; }

	void OnActive();

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	void CreatePS();

protected:
	Symbol* m_symbol;

private:
	p3d_sprite* m_spr;

	sm::mat4 m_mat;

	sm::Quaternion m_dir;

	bool m_alone;
	bool m_reuse;

	mutable RenderParams m_rp;

}; // Sprite

}

#endif // _EASYPARTICLE3D_SPRITE_H_