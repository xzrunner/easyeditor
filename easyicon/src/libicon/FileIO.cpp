#include "FileIO.h"
#include "Icon.h"
#include "IconFactory.h"

#include <ee/FileHelper.h>
#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/SymbolMgr.h>

#include <memmgr/LinearAllocator.h>
#include <sns/NodeFactory.h>
#include <sns/IconSym.h>
#include <sns/NodeSpr.h>

#include <json/json.h>

#include <fstream>

namespace eicon
{

void FileIO::StoreToFile(const char* filename, const Icon& icon)
{
	Json::Value value;

	std::string dir = ee::FileHelper::GetFileDir(filename);

	auto ee_img = std::dynamic_pointer_cast<ee::ImageSymbol>(icon.GetImage());
	value["image"] = ee::FileHelper::GetRelativePath(dir, ee_img->GetFilepath());

	value["type"] = icon.GetIconDesc();

	icon.StoreToFile(value);
	
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

std::unique_ptr<s2::Icon> FileIO::LoadFromFile(const char* filename)
{
	auto ext = ee::FileHelper::GetExtension(filename);
	if (ext == "bin") {
		return LoadFromBinFile(filename);
	} else if (ext == "json") {
		return LoadFromJsonFile(filename);
	} else {
		return nullptr;
	}
}

std::unique_ptr<s2::Icon> FileIO::LoadFromJsonFile(const char* filename)
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
	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(path);
	auto img_sym = std::dynamic_pointer_cast<ee::ImageSymbol>(sym);

	auto icon = IconFactory::Create(value);
	icon->SetImage(img_sym);
	dynamic_cast<Icon*>(icon.get())->LoadFromFile(value);

	return icon;
}

std::unique_ptr<s2::Icon> FileIO::LoadFromBinFile(const char* filename)
{
	mm::LinearAllocator alloc;

	auto sym = sns::NodeFactory::CreateSymFromBin(alloc, filename);
	auto sym_src = dynamic_cast<sns::IconSym*>(sym);

	std::string dir = ee::FileHelper::GetFileDir(filename);
	std::string bsae_path = ee::FileHelper::GetAbsolutePath(dir, sym_src->GetBaseSymPath());
	auto bsae_sym = ee::SymbolMgr::Instance()->FetchSymbol(bsae_path);
	auto img_bsae_sym = std::dynamic_pointer_cast<ee::ImageSymbol>(bsae_sym);

	auto icon = IconFactory::Create(sym_src->GetType());
	icon->SetImage(img_bsae_sym);
	dynamic_cast<Icon*>(icon.get())->LoadFromFile(*sym_src);

	return icon;
}

}