#include "PackParticle3D.h"

#include <easyparticle3d.h>

namespace edb
{

void PackParticle3D::Pack(const std::string& dir, const std::string& out)
{
	eparticle3d::CocoPacker::pack(dir, out);
}

}