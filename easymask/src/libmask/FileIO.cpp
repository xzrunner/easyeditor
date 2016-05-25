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

	const ee::Sprite *base = sym->GetSprite(true),
		             *mask = sym->GetSprite(false);
	if (base) {
		value["base"] = Store(dir, base);
	}
	if (mask) {
		value["mask"] = Store(dir, mask);
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
		ee::Sprite* spr = Load(dir, value["base"]);
		sym->SetSprite(spr, true);
	}
	if (!value["mask"].isNull()) {
		ee::Sprite* spr = Load(dir, value["mask"]);
		sym->SetSprite(spr, false);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

Json::Value FileIO::Store(const std::string& dir, const ee::Sprite* spr)
{
	Json::Value val;
	val["filepath"] = ee::FileHelper::GetRelativePath(dir, spr->GetSymbol().GetFilepath());
	spr->Store(val, dir);
	return val;
}

ee::Sprite* FileIO::Load(const std::string& dir, const Json::Value& val)
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(dir, val["filepath"].asString());
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->Load(val, dir);
	sym->Release();
	return spr;
}

}