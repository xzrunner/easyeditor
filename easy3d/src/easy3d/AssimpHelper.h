#ifndef _EASY3D_ASSIMP_HELPER_H_
#define _EASY3D_ASSIMP_HELPER_H_

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

namespace e3d
{

class ModelParametric;
class AABB;
struct Material;

class AssimpHelper
{
public:

	void LoadFile(const char* filename, ModelParametric& model, e3d::AABB& aabb);

private:
	void LoadNode(const aiScene* scene, const aiNode* node, ModelParametric& model,
		e3d::AABB& aabb);

	void LoadMesh(const aiMesh* mesh, const aiMaterial* material, ModelParametric& model, e3d::AABB& aabb);
	void LoadMaterial(const aiMesh* ai_mesh, const aiMaterial* ai_material, Material& material);

}; // AssimpHelper

}

#endif // _EASY3D_ASSIMP_HELPER_H_