#include "FileIO.h"
#include "Icon.h"
#include "IconFactory.h"

#include <json/json.h>

namespace eicon
{

void FileIO::StoreToFile(const char* filename, const Icon* icon)
{
	Json::Value value;

	std::string dir = ee::FileHelper::GetFileDir(filename);
	value["image"] = ee::FileHelper::GetRelativePath(dir,
		icon->GetImage()->GetFilepath()).ToStdString();

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

	std::string dir = ee::FileHelper::GetFileDir(filename);
	std::string path = ee::FileHelper::GetAbsolutePath(dir, value["image"].asString());
	ee::Image* img = ee::ImageMgr::Instance()->GetItem(path);

	Icon* icon = IconFactory::CreateIconFromFile(value);
	icon->SetImage(img);
	icon->LoadFromFile(value);
	img->Release();

	return icon;
}

}