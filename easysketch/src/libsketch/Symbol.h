#ifndef _LIBSKETCH_SYMBOL_H_
#define _LIBSKETCH_SYMBOL_H_


#include <easy3d.h>

namespace libsketch
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
	virtual void ReloadTexture() const;
	virtual void Draw(const ee::Matrix& mt, const ee::ColorTrans& color = ee::ColorTrans(), 
		const ee::Sprite* spr = NULL, const ee::Sprite* root = NULL) const;
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

#endif // _LIBSKETCH_SYMBOL_H_