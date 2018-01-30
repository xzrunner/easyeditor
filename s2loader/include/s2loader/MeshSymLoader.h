#ifndef _S2LOADER_MESH_SYM_LOADER_H_
#define _S2LOADER_MESH_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <sprite2/typedef.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class MeshSymbol; class Mesh; class Symbol; }
namespace simp { class NodeMesh; class PointsMesh; class TrianglesMesh; class Skin2Mesh; }

namespace s2loader
{

class MeshSymLoader : private cu::Uncopyable
{
public:
	MeshSymLoader(s2::MeshSymbol& sym);

	void LoadJson(const CU_STR& filepath);	
	void LoadBin(const simp::NodeMesh* node);

private:
	static std::unique_ptr<s2::Mesh> LoadPointsMesh(const s2::SymConstPtr& base_sym, simp::PointsMesh* mesh);
	static std::unique_ptr<s2::Mesh> LoadTrianglesMesh(const s2::SymConstPtr& base_sym, simp::TrianglesMesh* mesh);
	static std::unique_ptr<s2::Mesh> LoadSkin2Mesh(const s2::SymConstPtr& base_sym, simp::Skin2Mesh* mesh);

	static std::unique_ptr<s2::Mesh> CreatePointsMesh(const Json::Value& val, const s2::SymConstPtr& base_sym);

private:
	s2::MeshSymbol& m_sym;

}; // MeshSymLoader

}

#endif // _S2LOADER_MESH_SYM_LOADER_H_
