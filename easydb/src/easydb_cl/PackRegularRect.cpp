#include "PackRegularRect.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <easytexpacker.h>

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
	return Command() + " [src dir] [dst dir] [name]";
}

void PackRegularRect::Run(int argc, char *argv[])
{
	// pack-rect e:\debug\cut e:\debug\pack 2013baji

	if (!check_number(this, argc, 5)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	Trigger(argv[2], argv[3], argv[4]);
}

void PackRegularRect::Trigger(const std::string& src_dir, const std::string& dst_dir,
							  const std::string& name)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);

	std::string filepath = dst_dir + "\\" + name;

  	// pack
  	etexpacker::RegularRectPack rrp(files);
  	rrp.Pack();
  	rrp.OutputToText(filepath);
  
  	// gen image
  	etexpacker::GenRegularRectImage::CreateMulti(filepath);
}

}