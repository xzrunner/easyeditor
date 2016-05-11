#ifndef _EASYSKETCH_SYMBOL_H_
#define _EASYSKETCH_SYMBOL_H_

#include <ee/Symbol.h>

#include <easy3d.h>

namespace e3d { class IModel; }

namespace esketch
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// IObject interface
	//	
	virtual Symbol* Clone() const;

	//
	// Symbol interface
	//
	virtual void Draw(const s2::RenderParams& trans, const ee::Sprite* spr = NULL, 
		const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

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

#endif // _EASYSKETCH_SYMBOL_H_