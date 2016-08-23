#ifndef _SPRITE2_DRAW_MESH_H_
#define _SPRITE2_DRAW_MESH_H_

#include <SM_Matrix.h>

namespace s2
{

class Mesh;
class RenderParams;
class Symbol;

class DrawMesh
{
public:
	static void DrawInfoUV(const Mesh* mesh, const sm::mat4* mt = NULL);
	static void DrawInfoXY(const Mesh* mesh, const sm::mat4* mt = NULL);

	static void DrawTexture(const Mesh* mesh, const RenderParams& params, 
		const Symbol* base_sym = NULL);

	static void DrawOnlyMesh(const Mesh* mesh, const RenderParams& params, int texid);

private:
	static void DrawMeshToTmp(const Mesh* mesh, const RenderParams& params,
		const Symbol* base_sym = NULL);
	static void DrawTmpToScreen(const Mesh* mesh, const RenderParams& params);

}; // DrawMesh

}

#endif // _SPRITE2_DRAW_MESH_H_