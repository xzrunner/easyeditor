#include "FileIO.h"
#include "Icon.h"
#include "IconFactory.h"

#include <ee/FileHelper.h>
#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/SymbolMgr.h>

#include <json/json.h>

#include <fstream>

namespace eicon
{

void FileIO::StoreToFile(const char* filename, const Icon* icon)
{
	Json::Value value;

	std::string dir = ee::FileHelper::GetFileDir(filename);

	const ee::ImageSymbol* ee_img = dynamic_cast<const ee::ImageSymbol*>(icon->GetImage());
	value["image"] = ee::FileHelper::GetRelativePath(dir, ee_img->GetFilepath());

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
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(path);
	ee::ImageSymbol* img_sym = dynamic_cast<ee::ImageSymbol*>(sym);

	Icon* icon = IconFactory::CreateIconFromFile(value);
	icon->SetImage(img_sym);
	icon->LoadFromFile(value);
	img_sym->RemoveReference();

	return icon;
}

}