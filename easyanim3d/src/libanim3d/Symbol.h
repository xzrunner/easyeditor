#ifndef _EASYANIM3D_SYMBOL_H_
#define _EASYANIM3D_SYMBOL_H_

#include <drag2d.h>
#include <easy3d.h>

namespace eanim3d
{

class Symbol : public d2d::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

	static d2d::Symbol* Create() { return new Symbol(); }

	void SetModel(e3d::IModel* model);

	void SetAABB(const e3d::AABB& aabb) { m_aabb = aabb; }
	const e3d::AABB& GetAABB() const { return m_aabb; }

protected:
	virtual void LoadResources();

private:
	e3d::IModel* m_model;

	e3d::AABB m_aabb;

}; // Symbol

}

#endif // _EASYANIM3D_SYMBOL_H_