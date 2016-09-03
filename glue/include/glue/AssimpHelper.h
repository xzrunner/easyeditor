#ifndef _GLUE_ASSIMP_HELPER_H_
#define _GLUE_ASSIMP_HELPER_H_

struct m3_model;
struct m3_mesh;
struct m3_material;
struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

#include <map>

namespace glue
{

class Texture;

class AssimpHelper
{
public:
	static m3_model* Load(const char* filename);

private:
	static void LoadNode(const aiScene* scene, const aiNode* node, m3_model* model, const std::string& dir);

	static void LoadMesh(const aiMesh* ai_mesh, const aiMaterial* ai_material, m3_mesh* mesh, const std::string& dir);

	static void LoadMaterial(const aiMesh* ai_mesh, const aiMaterial* ai_material, m3_material* material, const std::string& dir);

private:
	static std::map<std::string, Texture*> m_tex_cache;

}; // AssimpHelper

}

#endif // _GLUE_ASSIMP_HELPER_H_