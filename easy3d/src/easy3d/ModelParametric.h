#ifndef _E3D_MODEL_PARAMETRIC_H_
#define _E3D_MODEL_PARAMETRIC_H_

#include "IModel.h"

namespace e3d
{

class ISurface;
class AABB;

class ModelParametric : public IModel
{
public:
	ModelParametric();
	ModelParametric(const ISurface* surface, AABB& aabb);

	virtual const std::vector<Mesh>& GetAllMeshes() const {
		return m_meshes;
	}

	void AddMesh(const Mesh& mesh) {
		m_meshes.push_back(mesh);
	}

private:
	std::vector<Mesh> m_meshes;

}; // ModelParametric

}

#endif // _E3D_MODEL_PARAMETRIC_H_