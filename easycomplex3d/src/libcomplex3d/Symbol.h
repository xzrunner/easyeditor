#ifndef _EASYCOMPLEX3D_SYMBOL_H_
#define _EASYCOMPLEX3D_SYMBOL_H_

#include <ee/Symbol.h>

namespace e3d { class IModel; }

namespace ecomplex3d
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
		const s2::Sprite* root = NULL) const override;

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

	void SetModel(m3::Model* model);

	void SetAABB(const m3::AABB& aabb) { m_aabb = aabb; }
	const m3::AABB& GetAABB() const { return m_aabb; }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const override;

	virtual bool LoadResources() override;

private:
	m3::Model* m_model;

	m3::AABB m_aabb;

}; // Symbol

}

#endif // _EASYCOMPLEX3D_SYMBOL_H_
