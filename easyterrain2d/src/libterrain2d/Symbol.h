#ifndef _EASYTERRAIN2D_SYMBOL_H_
#define _EASYTERRAIN2D_SYMBOL_H_

#include <ee/Symbol.h>

#include <vector>

#include <time.h>

namespace eterrain2d
{

class OceanMesh;

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const s2::SymbolVisitor& visitor) {}
	virtual s2::RenderReturn Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

	std::vector<OceanMesh*>& GetOceans() { return m_oceans; }
	const std::vector<OceanMesh*>& GetOceans() const { return m_oceans; }

	void SetUpdateOpen(bool open) { m_update = open; }

	float GetOceanAngle() const { return m_angle; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources();

private:
	void Clear();

private:
	bool m_update;

	std::vector<OceanMesh*> m_oceans;

	float m_angle;

	mutable clock_t m_time;

}; // Symbol

}

#endif // _EASYTERRAIN2D_SYMBOL_H_