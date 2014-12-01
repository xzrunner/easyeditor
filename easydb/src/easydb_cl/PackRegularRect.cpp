#include "PackRegularRect.h"
#include "check_params.h"

#include <drag2d.h>
#include <easypacker.h>

namespace edb
{

std::string PackRegularRect::Command() const
{
	return "pack-rect";
}

std::string PackRegularRect::Description() const
{
	return "pack regular rects which edge are times of 4";
}

std::string PackRegularRect::Usage() const
{
	return Command() + " [src dir] [dst dir]";
}

void PackRegularRect::Run(int argc, char *argv[])
{
	// pack-rect e:\debug\cut e:\debug\pack

	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	Trigger(argv[2], argv[3]);
}

void PackRegularRect::Trigger(const std::string& src_dir, const std::string& dst_dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(src_dir, files);
	libpacker::RegularRectPack rrp(files);
<<<<<<< HEAD
=======
	rrp.Pack();
	rrp.PrintStatics();
>>>>>>> [ADDED] regular rect pack to easydb
}

}