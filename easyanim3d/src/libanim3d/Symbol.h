#ifndef _EASYANIM3D_SYMBOL_H_
#define _EASYANIM3D_SYMBOL_H_

#include <ee/Symbol.h>

#include <easy3d.h>

namespace eanim3d
{

class Symbol : public ee::Symbol, public s2::DummySymbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* sprite = NULL) const;

	void SetModel(e3d::IModel* model);

	void SetAABB(const e3d::AABB& aabb) { m_aabb = aabb; }
	const e3d::AABB& GetAABB() const { return m_aabb; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	e3d::IModel* m_model;

	e3d::AABB m_aabb;

}; // Symbol

}

#endif // _EASYANIM3D_SYMBOL_H_