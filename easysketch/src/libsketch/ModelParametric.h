#pragma once

#include <drag2d.h>

namespace z3d 
{

class ISurface;

class ModelParametric : public IModel
{
public:
	ModelParametric(const ISurface* surface);

	virtual const std::vector<Mesh>& GetAllMeshes() const {
		return m_meshes;
	}

private:
	std::vector<Mesh> m_meshes;

}; // ModelParametric

}