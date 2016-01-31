#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"

using namespace shootbubble;

void FileIO::load(const char* filename)
{
	ee::SymbolMgr::Instance()->clear();
	ee::BitmapMgr::Instance()->clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Context* context = Context::Instance();

	int i = 0;
	Json::Value imgValue = value["image"][i++];
	while (!imgValue.isNull()) {
		ee::ISprite* sprite = load(imgValue);
		context->stage->insertSprite(sprite);
		imgValue = value["image"][i++];
	}

	context->library->loadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	Json::Value value;

	std::vector<ee::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["image"][i] = store(sprites[i]);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::ISprite* FileIO::load(const Json::Value& value)
{
	std::string filepath = value["filepath"].asString();
	const int row = value["row"].asInt(),
		col = value["col"].asInt();

	ee::ISymbol* symbol = ee::SymbolMgr::Instance()->fetchSymbol(filepath);
	ee::Vector pos;
	Context::Instance()->stage->transGridPosToCoords(row, col, pos);
	ee::ISprite* sprite = ee::SpriteFactory::Instance()->create(symbol);
	symbol->release();
	sprite->translate(pos);
	return sprite;
}

Json::Value FileIO::store(const ee::ISprite* sprite)
{
	Json::Value value;

	value["filepath"] = sprite->getSymbol().getFilepath().ToStdString();

	int row, col;
	Context::Instance()->stage->transCoordsToGridPos(sprite->getPosition(), row, col);
	value["row"] = row;
	value["col"] = col;

	return value;
}