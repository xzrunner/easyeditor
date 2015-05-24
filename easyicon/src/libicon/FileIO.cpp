#include "FileIO.h"
#include "Icon.h"
#include "IconFactory.h"

#include <json/json.h>

namespace eicon
{

void FileIO::StoreToFile(const char* filename, const Icon* icon)
{
	Json::Value value;

	std::string dir = d2d::FilenameTools::getFileDir(filename);
	value["image"] = d2d::FilenameTools::getRelativePath(dir,
		icon->GetImage()->filepath()).ToStdString();

	value["type"] = icon->GetIconDesc();

	icon->StoreToFile(value);
	
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

Icon* FileIO::LoadFromFile(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filename);
	wxString path = d2d::FilenameTools::getAbsolutePath(dir, value["image"].asString());
	d2d::Image* img = d2d::ImageMgr::Instance()->getItem(path);

	Icon* icon = IconFactory::CreateIconFromFile(value);
	icon->LoadFromFile(value);
	icon->SetImage(img);
	img->Release();

	return icon;
}

}