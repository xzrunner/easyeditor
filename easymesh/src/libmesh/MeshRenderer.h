#ifndef _EASYMESH_MESH_RENDERER_H_
#define _EASYMESH_MESH_RENDERER_H_

namespace ee { class RenderParams; }

namespace emesh
{

class Mesh;

class MeshRenderer
{
public:
	static void DrawInfoUV(const Mesh* mesh);
	static void DrawInfoXY(const Mesh* mesh);

	static void DrawTexture(const Mesh* mesh, const ee::RenderParams& params);

	static void DrawMesh(const Mesh* mesh, const ee::RenderParams& params, int texid);

private:
	static void DrawMeshToTmp(const Mesh* mesh, const ee::RenderParams& params);
	static void DrawTmpToScreen(const Mesh* mesh, const ee::RenderParams& params);

}; // MeshRenderer

}

#endif // _EASYMESH_MESH_RENDERER_H_