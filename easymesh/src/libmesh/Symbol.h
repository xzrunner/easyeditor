#ifndef _EASYMESH_SYMBOL_H_
#define _EASYMESH_SYMBOL_H_

#include <ee/Symbol.h>
#include <ee/Rect.h>

namespace ee { class Image; }

namespace emesh
{

class Mesh;
class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	Symbol(ee::Symbol* base);
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const;

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const {
		return m_region;
	}

 	const Mesh* GetMesh() const { return m_mesh; }
 	Mesh* GetMesh() { return m_mesh; }
	void SetMesh(Mesh* mesh);

	void SetPause(bool pause) { m_pause = pause; }

	void CreateMesh();

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void InitBounding();

private:
	ee::Symbol* m_base;

	Mesh* m_mesh;

	bool m_pause;

	ee::Rect m_region;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_