#include "FileIO.h"
#include "StagePanel.h"
#include "SymbolExt.h"
#include "SpriteExt.h"

#include <ee/FileHelper.h>
#include <ee/sprite_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>
#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>

#include <fstream>

namespace sg
{

void FileIO::load(const char* filename, StagePanel* stage)
{
// 	ee::SymbolMgr::Instance()->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filename) + "\\";

	int i = 0;
	Json::Value imgValue = value["image"][i++];
	while (!imgValue.isNull()) {
		ee::Sprite* spr = load(imgValue, stage, dir);
		ee::InsertSpriteSJ::Instance()->Insert(spr);
		imgValue = value["image"][i++];
	}

//	library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void FileIO::store(const char* filename, StagePanel* stage)
{
	Json::Value value;

	std::string dir = ee::FileHelper::GetFileDir(filename) + "\\";

	std::vector<ee::Sprite*> sprs;
	stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		value["image"][i] = store(sprs[i], stage, dir);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::Sprite* FileIO::load(const Json::Value& value, StagePanel* stage, const std::string& dir)
{
	int row = value["row"].asInt(),
		col = value["col"].asInt();

	std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, value);
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	SetSymbolUserData(sym);

	sm::vec2 pos;
	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->SetTag(value["tag"].asString());
	sym->RemoveReference();

	//// old
	//stage->TransGridPosToCoords(row, col, pos);
 	// new
 	SymbolExt* symbol_info = static_cast<SymbolExt*>(spr->GetSymbol()->GetUserData());
 	int size = symbol_info->building->size;
 	if (size % 2) {
 		int offset = (size - 1) / 2;
 		row += offset;
 		col -= offset;
 	} else {
 		int offset = size / 2;
 		row += (offset - 1);
 		col -= offset;
 	}
 	stage->TransGridPosToCoordsNew(row, col, pos);

	if (!value["level"].isNull()) {
		SpriteExt* spr_info = new SpriteExt;
		spr_info->level = value["level"].asInt();
		spr->SetUserData(spr_info);
	}
	int level = value["row"].asInt();

 	//////////////////////////////////////////////////////////////////////////

	spr->Translate(pos);

	return spr;
}

Json::Value FileIO::store(const ee::Sprite* spr, StagePanel* stage, 
						  const std::string& dir)
{
	Json::Value value;

	value["filepath"] = ee::FileHelper::GetRelativePath(dir,
		spr->GetSymbol()->GetFilepath());

	int row, col;
	stage->TransCoordsToGridPosNew(spr->GetPosition(), row, col);

	SymbolExt* symbol_info = static_cast<SymbolExt*>(spr->GetSymbol()->GetUserData());
	int size = symbol_info->building->size;
	if (size % 2) {
		int offset = (size - 1) / 2;
		row -= offset;
		col += offset;
	} else {
		int offset = size / 2;
		row -= (offset - 1);
		col += offset;
	}

	value["row"] = row;
	value["col"] = col;
	value["tag"] = spr->GetTag();

	value["name"] = symbol_info->building->name;
	
	SpriteExt* spr_info = static_cast<SpriteExt*>(spr->GetUserData());	
	value["level"] = spr_info->level;

	return value;
}

void FileIO::SetSymbolUserData(ee::Symbol* sym)
{
	if (sym->GetUserData()) {
		return;
	}

	std::string filepath = sym->GetFilepath();
	if (!filepath.find("wall") != std::string::npos) {
		return;
	}

	int pos = filepath.find_last_of('_');
	std::string wall_type = filepath.substr(pos + 1, 1);
	std::string wall_path = filepath.substr(0, pos) + ".png";

	try {
		ee::Symbol* wall_symbol = ee::SymbolMgr::Instance()->FetchSymbol(wall_path);
		if (!wall_symbol || !wall_symbol->GetUserData()) {
			return;
		}

		SymbolExt* info = static_cast<SymbolExt*>(wall_symbol->GetUserData());

		SymbolExt* new_info = new SymbolExt;
		new_info->size = info->size;
		new_info->remain = info->remain;
		new_info->wall_type = (wall_type[0] - '0');
		new_info->level = info->level;
		new_info->building = info->building;

		sym->SetUserData(new_info);
	} catch (ee::Exception& e) {
		;
	}
}

}