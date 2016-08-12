#ifndef _EASY3D_IMODEL_H_
#define _EASY3D_IMODEL_H_

#include "Mesh.h"

#include <vector>

namespace e3d 
{

class IModel : public ee::Object
{
public:
	virtual const std::vector<Mesh>& GetAllMeshes() const = 0;
	virtual ~IModel() {}
};

}

#endif // _EASY3D_IMODEL_H_