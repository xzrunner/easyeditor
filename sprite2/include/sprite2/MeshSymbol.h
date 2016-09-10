#ifndef _SPRITE2_MESH_SYMBOL_H_
#define _SPRITE2_MESH_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Mesh;

class MeshSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	MeshSymbol();
	virtual ~MeshSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const Mesh* GetMesh() const { return m_mesh; }
	Mesh* GetMesh() { return m_mesh; }
	void SetMesh(Mesh* mesh);

	void SetPause(bool pause) { m_pause = pause; }

protected:
	Mesh* m_mesh;

private:
	bool m_pause;

}; // MeshSymbol

}

#endif // _SPRITE2_MESH_SYMBOL_H_