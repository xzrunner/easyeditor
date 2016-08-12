#ifndef _EASYMESH_SYMBOL_H_
#define _EASYMESH_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/MeshSymbol.h>

namespace ee { class Image; }

namespace emesh
{

class Mesh;
class Symbol : public ee::Symbol, public s2::MeshSymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	Symbol(ee::Symbol* base);
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

 	const Mesh* GetMesh() const { return m_mesh; }
 	Mesh* GetMesh() { return m_mesh; }
	void SetMesh(Mesh* mesh);

	void SetPause(bool pause) { m_pause = pause; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	Mesh* m_mesh;

	bool m_pause;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_