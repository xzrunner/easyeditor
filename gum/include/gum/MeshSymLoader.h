#ifndef _GUM_MESH_SYM_LOADER_H_
#define _GUM_MESH_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class MeshSymbol; class MeshTransform; class Mesh; class Symbol; }
namespace simp { class NodeMesh; }

namespace gum
{

// todo: only support network mesh
class MeshSymLoader : private cu::Uncopyable
{
public:
	MeshSymLoader(s2::MeshSymbol* sym);
	~MeshSymLoader();

	void LoadJson(const std::string& filepath);	
	void LoadBin(const simp::NodeMesh* node);

	static void LoadMeshTransform(s2::MeshTransform& trans, const Json::Value& value);

private:
	static s2::Mesh* CreateNetworkMesh(const Json::Value& val, const s2::Symbol* sym);

private:
	s2::MeshSymbol* m_sym;

}; // MeshSymLoader

}

#endif // _GUM_MESH_SYM_LOADER_H_
