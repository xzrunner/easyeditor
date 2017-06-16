#include "PackParticle2D.h"
#include "check_params.h"

#include <easyparticle2d.h>

namespace edb
{

std::string PackParticle2D::Command() const
{
	return "pack-particle3d";
}

std::string PackParticle2D::Description() const
{
	return "pack particle3d data from dir to json file";
}

std::string PackParticle2D::Usage() const
{
	// pack-particle2d D:\projects\ejoy\coco-tools\sg_particle\data D:\projects\ejoy\coco-tools\sg_particle\data\zz.json

	return Command() + " [dir path] [output file]";
}

int PackParticle2D::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void PackParticle2D::Trigger(const std::string& dir, const std::string& out)
{
	eparticle2d::CocoPacker::pack(dir, out);
}

}