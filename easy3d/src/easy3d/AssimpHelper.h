#ifndef _E3D_ASSIMP_HELPER_H_
#define _E3D_ASSIMP_HELPER_H_

struct aiScene;
struct aiNode;
struct aiMesh;

namespace e3d
{

class ModelParametric;
class AABB;

class AssimpHelper
{
public:

	void LoadFile(const char* filename, ModelParametric& model, e3d::AABB& aabb);

private:
	void LoadNode(const aiScene* scene, const aiNode* node, ModelParametric& model,
		e3d::AABB& aabb);

	void LoadMesh(const aiMesh* mesh, ModelParametric& model, e3d::AABB& aabb);

}; // AssimpHelper

}

#endif // _E3D_ASSIMP_HELPER_H_