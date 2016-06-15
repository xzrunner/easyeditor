#include "FileIO.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/panel_msg.h>
#include <ee/SymbolMgr.h>
#include <ee/Sprite.h>
#include <ee/SpriteFactory.h>

#include <fstream>

namespace emask
{

void FileIO::Store(const char* filepath, const Symbol* sym)
{
	Json::Value value;

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	value["name"] = sym->name;

	const ee::Symbol *base = sym->GetSymbol(true),
		             *mask = sym->GetSymbol(false);
	if (base) {
		value["base"]["filepath"] = ee::FileHelper::GetRelativePath(dir, base->GetFilepath());
	}
	if (mask) {
		value["mask"]["filepath"] = ee::FileHelper::GetRelativePath(dir, mask->GetFilepath());
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const char* filepath, Symbol* sym)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	sym->name = value["name"].asString();

	if (!value["base"].isNull()) {
		std::string filepath = ee::FileHelper::GetAbsolutePath(dir, value["base"]["filepath"].asString());
		ee::Symbol* base = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		static_cast<Symbol*>(sym)->SetSymbol(base, true);
		base->Release();
	}
	if (!value["mask"].isNull()) {
		std::string filepath = ee::FileHelper::GetAbsolutePath(dir, value["mask"]["filepath"].asString());
		ee::Symbol* mask = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		static_cast<Symbol*>(sym)->SetSymbol(mask, false);
		mask->Release();
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}