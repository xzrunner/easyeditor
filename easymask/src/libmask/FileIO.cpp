#include "FileIO.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/panel_msg.h>
#include <ee/SymbolMgr.h>
#include <ee/Sprite.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolPath.h>

#include <fstream>

namespace emask
{

void FileIO::Store(const char* filepath, const Symbol& sym)
{
	Json::Value value;

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	value["name"] = sym.name;

	auto& base = sym.GetBase()->GetSymbol();
	auto& mask = sym.GetMask()->GetSymbol();
	if (base) {
		value["base"]["filepath"] = ee::SymbolPath::GetRelativePath( 
			*std::dynamic_pointer_cast<ee::Symbol>(base), dir);
	}
	if (mask) {
		value["mask"]["filepath"] = ee::SymbolPath::GetRelativePath(
			*std::dynamic_pointer_cast<ee::Symbol>(mask), dir);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const char* filepath, Symbol& sym)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	sym.name = value["name"].asString();

	if (!value["base"].isNull()) {
		std::string filepath = ee::FileHelper::GetAbsolutePath(dir, value["base"]["filepath"].asString());
		auto base_sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		auto base_spr = ee::SpriteFactory::Instance()->Create(base_sym);
		sym.SetBase(base_spr);
	}
	if (!value["mask"].isNull()) {
		std::string filepath = ee::FileHelper::GetAbsolutePath(dir, value["mask"]["filepath"].asString());
		auto mask_sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		auto mask_spr = ee::SpriteFactory::Instance()->Create(mask_sym);
		sym.SetMask(mask_spr);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}