#include "PackParticle3D.h"
#include "check_params.h"

#include <easyparticle3d.h>

namespace edb
{

std::string PackParticle3D::Command() const
{
	return "pack-particle3d";
}

std::string PackParticle3D::Description() const
{
	return "pack particle3d data from dir to json file";
}

std::string PackParticle3D::Usage() const
{
	// pack-particle3d D:\projects\ejoy\coco-tools\sg_particle\data D:\projects\ejoy\coco-tools\sg_particle\data\zz.json

	return Command() + " [dir path] [output file]";
}

void PackParticle3D::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2], argv[3]);
}

void PackParticle3D::Trigger(const std::string& dir, const std::string& out)
{
	eparticle3d::CocoPacker::PackDir(dir, out);
}

}