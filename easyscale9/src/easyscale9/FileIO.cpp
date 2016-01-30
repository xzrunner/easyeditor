#include "FileIO.h"
#include "StagePanel.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"

#include <easyscale9.h>

namespace escale9
{

static const int OFFSET_VERSION = 2;
static const int VERSION = OFFSET_VERSION;

void FileIO::load(const char* filename, d2d::LibraryPanel* library, 
				  d2d::MultiSpritesImpl* stage, ToolbarPanel* toolbar)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString dir = d2d::FileHelper::GetFileDir(filename);

	bool need_offset = true;
	if (!value["version"].isNull() && value["version"].asInt() >= OFFSET_VERSION) {
		need_offset = false;
	}

 	int i = 0;
 	Json::Value spriteValue = value["sprite"][i++];
 	while (!spriteValue.isNull()) {
		d2d::Sprite* sprite = load(spriteValue, dir);
		if (need_offset) {
			sprite->Translate(d2d::Vector(-150, -150));
		}
		d2d::InsertSpriteSJ::Instance()->Insert(sprite);
 		spriteValue = value["sprite"][i++];
 	}

	float width = value["width"].asInt(),
		  height = value["height"].asInt();
	toolbar->setSize(width, height);

	library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename, StagePanel* stage, 
				   ToolbarPanel* toolbar)
{
	Json::Value value;

	value["version"] = VERSION;

	value["type"] = stage->getPatchSymbol()->GetScale9Data().GetType();

	value["width"] = toolbar->getWidth();
	value["height"] = toolbar->getHeight();

 	std::vector<d2d::Sprite*> sprites;
	stage->TraverseSprites(d2d::FetchAllVisitor<d2d::Sprite>(sprites));

	wxString dir = d2d::FileHelper::GetFileDir(filename);
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		value["sprite"][i] = StoreNew(sprites[i], dir);
	}

// 	Symbol* symbol = stage->getPatchSymbol();
// 	const Scale9Data& s_data = symbol->GetScale9Data();
// 	int idx = 0;
// 	for (int i = 0; i < 3; ++i) {
// 		for (int j = 0; j < 3; ++j) {
// 			d2d::Sprite* sprite = s_data.GetSprite(i, j);
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

d2d::Sprite* FileIO::load(const Json::Value& value, const wxString& dir)
{
	std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, value);
	if (!d2d::FileHelper::IsFileExist(filepath)) {
		std::string filepath = value["filepath"].asString();
		throw d2d::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
			dir.ToStdString().c_str(), filepath.c_str());
	}

	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	d2d::SymbolSearcher::SetSymbolFilepaths(dir, symbol, value);
	d2d::Sprite* sprite = d2d::SpriteFactory::Instance()->Create(symbol);
	sprite->Load(value);
	symbol->Release();

	return sprite;
}

Json::Value FileIO::store(d2d::Sprite* sprite, const wxString& dir)
{
	Json::Value value;
	const d2d::Symbol& symbol = sprite->GetSymbol();

	// filepath
	value["filepath"] = d2d::FileHelper::GetRelativePath(dir,
		symbol.GetFilepath()).ToStdString();
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

Json::Value FileIO::StoreNew(d2d::Sprite* sprite, const wxString& dir)
{
	Json::Value value;
	const d2d::Symbol& symbol = sprite->GetSymbol();

	// filepath
	value["filepath"] = d2d::FileHelper::GetRelativePath(dir,
		symbol.GetFilepath()).ToStdString();
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