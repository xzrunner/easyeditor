#ifndef _EASY3D_IMODEL_H_
#define _EASY3D_IMODEL_H_

#include "Mesh.h"

#include <CU_RefCountObj.h>

#include <vector>

namespace e3d 
{

class IModel : public cu::RefCountObj
{
public:
	virtual const std::vector<Mesh>& GetAllMeshes() const = 0;
	virtual ~IModel() {}
};

}

#endif // _EASY3D_IMODEL_H_