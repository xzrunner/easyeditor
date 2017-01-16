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

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL, 
		const s2::Sprite* root = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

	void SetModel(m3::Model* model);

	void SetAABB(const m3::AABB& aabb) { m_aabb = aabb; }
	const m3::AABB& GetAABB() const { return m_aabb; }

protected:
	virtual bool LoadResources();

private:
	m3::Model* m_model;

	m3::AABB m_aabb;

}; // Symbol

}

#endif // _EASYSKETCH_SYMBOL_H_