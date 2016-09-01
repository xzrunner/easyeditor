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

	const s2::Symbol *base = sym->GetBase()->GetSymbol(),
		             *mask = sym->GetMask()->GetSymbol();
	if (base) {
		value["base"]["filepath"] = ee::FileHelper::GetRelativePath(dir, 
			dynamic_cast<const ee::Symbol*>(base)->GetFilepath());
	}
	if (mask) {
		value["mask"]["filepath"] = ee::FileHelper::GetRelativePath(dir, 
			dynamic_cast<const ee::Symbol*>(mask)->GetFilepath());
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
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
		static_cast<Symbol*>(sym)->SetBase(spr);
		spr->RemoveReference();
		sym->RemoveReference();
	}
	if (!value["mask"].isNull()) {
		std::string filepath = ee::FileHelper::GetAbsolutePath(dir, value["mask"]["filepath"].asString());
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
		static_cast<Symbol*>(sym)->SetMask(spr);
		spr->RemoveReference();
		sym->RemoveReference();
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}