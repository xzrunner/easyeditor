#ifndef _EASY3D_IMODEL_H_
#define _EASY3D_IMODEL_H_

#include "Mesh.h"

#include <drag2d.h>

namespace e3d 
{

class IModel : public d2d::Object
{
public:
	virtual const std::vector<Mesh>& GetAllMeshes() const = 0;
	virtual ~IModel() {}
};

}

#endif // _EASY3D_IMODEL_H_