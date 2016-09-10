#ifndef _GLUE_MESH_SYM_LOADER_H_
#define _GLUE_MESH_SYM_LOADER_H_

#include <CU_Uncopyable.h>

namespace s2 { class MeshSymbol; class MeshTransform; class Mesh; class Symbol; }

#include <json/json.h>

#include <string>

namespace glue
{

class MeshSymLoader : private cu::Uncopyable
{
public:
	MeshSymLoader(s2::MeshSymbol* sym);
	~MeshSymLoader();

	void LoadJson(const std::string& filepath);	

	static void LoadMeshTransform(s2::MeshTransform& trans, const Json::Value& value);

private:
	static s2::Mesh* CreateNetworkMesh(const Json::Value& val, const s2::Symbol* sym);

private:
	s2::MeshSymbol* m_sym;

}; // MeshSymLoader

}

#endif // _GLUE_MESH_SYM_LOADER_H_