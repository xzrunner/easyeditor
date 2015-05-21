#include "FileIO.h"
#include "StagePanel.h"

namespace sg
{

void FileIO::load(const char* filename, StagePanel* stage,
				  d2d::LibraryPanel* library)
{
// 	d2d::SymbolMgr::Instance()->clear();
// 	d2d::BitmapMgr::Instance()->clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value imgValue = value["image"][i++];
	while (!imgValue.isNull()) {
		d2d::ISprite* sprite = load(imgValue, stage);
		stage->insertSprite(sprite);
		imgValue = value["image"][i++];
	}

//	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename, StagePanel* stage)
{
	Json::Value value;

	std::vector<d2d::ISprite*> sprites;
	stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["image"][i] = store(sprites[i], stage);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

d2d::ISprite* FileIO::load(const Json::Value& value, StagePanel* stage)
{
	std::string filepath = value["filepath"].asString();
	const int row = value["row"].asInt(),
		col = value["col"].asInt();

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	d2d::Vector pos;
	stage->TransGridPosToCoords(row, col, pos);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	symbol->Release();
	sprite->translate(pos);
	return sprite;
}

Json::Value FileIO::store(const d2d::ISprite* sprite, StagePanel* stage)
{
	Json::Value value;

	value["filepath"] = sprite->getSymbol().getFilepath().ToStdString();

	int row, col;
	stage->TransCoordsToGridPos(sprite->getPosition(), row, col);
	value["row"] = row;
	value["col"] = col;

	return value;
}

}