#ifndef _E3D_MODEL_PARAMETRIC_H_
#define _E3D_MODEL_PARAMETRIC_H_

#include "IModel.h"

namespace e3d
{

class ISurface;
class Cube;

class ModelParametric : public IModel
{
public:
	ModelParametric(const ISurface* surface, Cube& aabb);

	virtual const std::vector<Mesh>& GetAllMeshes() const {
		return m_meshes;
	}

private:
	std::vector<Mesh> m_meshes;

}; // ModelParametric

}

#endif // _E3D_MODEL_PARAMETRIC_H_