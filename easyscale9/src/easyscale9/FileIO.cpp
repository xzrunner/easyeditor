#include "FileIO.h"
#include "StagePanel.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"

#include <fstream>

#include <easyscale9.h>

#include <ee/FileHelper.h>
#include <ee/sprite_msg.h>
#include <ee/LibraryPanel.h>
#include <ee/SymbolMgr.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SymbolSearcher.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>

namespace escale9
{

static const int OFFSET_VERSION = 2;
static const int VERSION = OFFSET_VERSION;

void FileIO::Load(const char* filename, ee::LibraryPanel* library, 
				  ee::MultiSpritesImpl* stage, ToolbarPanel* toolbar)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filename);

	bool need_offset = true;
	if (!value["version"].isNull() && value["version"].asInt() >= OFFSET_VERSION) {
		need_offset = false;
	}

 	int i = 0;
 	Json::Value spriteValue = value["sprite"][i++];
 	while (!spriteValue.isNull()) {
		ee::Sprite* sprite = Load(spriteValue, dir);
		if (need_offset) {
			sprite->Translate(ee::Vector(-150, -150));
		}
		ee::InsertSpriteSJ::Instance()->Insert(sprite);
 		spriteValue = value["sprite"][i++];
 	}

	float width = value["width"].asInt(),
		  height = value["height"].asInt();
	toolbar->setSize(width, height);

	library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void FileIO::Store(const char* filename, StagePanel* stage, 
				   ToolbarPanel* toolbar)
{
	Json::Value value;

	value["version"] = VERSION;

	value["type"] = stage->getPatchSymbol()->GetScale9Data().GetType();

	value["width"] = toolbar->getWidth();
	value["height"] = toolbar->getHeight();

 	std::vector<ee::Sprite*> sprites;
	stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));

	std::string dir = ee::FileHelper::GetFileDir(filename);
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		value["sprite"][i] = StoreNew(sprites[i], dir);
	}

// 	Symbol* symbol = stage->getPatchSymbol();
// 	const Scale9Data& s_data = symbol->GetScale9Data();
// 	int idx = 0;
// 	for (int i = 0; i < 3; ++i) {
// 		for (int j = 0; j < 3; ++j) {
// 			ee::Sprite* sprite = s_data.GetSprite(i, j);
// 			if (sprite) {
// 				value["sprite new"][idx++] = StoreNew(sprite, dir);
// 			} else {
// //				value["sprite new"][idx++] = NULL;
// 				idx++;
// 			}
// 		}
// 	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::Sprite* FileIO::Load(const Json::Value& value, const std::string& dir)
{
	std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, value);
	if (!ee::FileHelper::IsFileExist(filepath)) {
		std::string filepath = value["filepath"].asString();
		throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
			dir.c_str(), filepath.c_str());
	}

	ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	ee::SymbolSearcher::SetSymbolFilepaths(dir, symbol, value);
	ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
	sprite->Load(value);
	symbol->Release();

	return sprite;
}

Json::Value FileIO::Store(ee::Sprite* sprite, const std::string& dir)
{
	Json::Value value;
	const ee::Symbol& symbol = sprite->GetSymbol();

	// filepath
	value["filepath"] = ee::FileHelper::GetRelativePath(dir,
		symbol.GetFilepath());
	// filepaths
	const std::set<std::string>& filepaths = symbol.GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	sprite->Store(value);

	return value;
}

Json::Value FileIO::StoreNew(ee::Sprite* sprite, const std::string& dir)
{
	Json::Value value;
	const ee::Symbol& symbol = sprite->GetSymbol();

	// filepath
	value["filepath"] = ee::FileHelper::GetRelativePath(dir,
		symbol.GetFilepath());
	// filepaths
	const std::set<std::string>& filepaths = symbol.GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	sprite->Store(value);

	return value;
}

} // escale9