#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"

using namespace edb;

void FileIO::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

 	int i = 0;
 	Json::Value spriteValue = value["sprite"][i++];
 	while (!spriteValue.isNull()) {
		d2d::ISprite* sprite = loadSprite(spriteValue);
		Context::Instance()->stage->insertSprite(sprite);
 		spriteValue = value["sprite"][i++];
 	}
}

void FileIO::store(const char* filename)
{
	Json::Value value;

 	std::vector<d2d::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
 
 	d2d::Rect rect;
 	for (size_t i = 0, n = sprites.size(); i < n; ++i)
 	{
 		std::vector<d2d::Vector> vertices;
 		sprites[i]->getBounding()->getBoundPos(vertices);
 		for (size_t j = 0, m = vertices.size(); j < m; ++j)
 			rect.combine(vertices[j]);
 	}
 	d2d::Vector offset(-rect.xCenter(), -rect.yCenter());
 
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["sprite"][i] = store(sprites[i], offset);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

d2d::ISprite* FileIO::loadSprite(const Json::Value& value)
{
	d2d::ISprite* sprite = NULL;
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(
		value["filepath"].asString());
	sprite = d2d::SpriteFactory::Instance()->create(symbol);
	symbol->Release();

	sprite->load(value);

	return sprite;
}

d2d::IShape* FileIO::loadShape(const Json::Value& value)
{
	d2d::IShape* shape = NULL;
	return shape;
}

Json::Value FileIO::store(d2d::ISprite* sprite, const d2d::Vector& offset)
{
	Json::Value value;
	value["filepath"] = sprite->getSymbol().GetFilepath().ToStdString();
	sprite->store(value);
	return value;
}

Json::Value FileIO::store(d2d::IShape* shape, const d2d::Vector& offset)
{
	Json::Value value;

	return value;
}