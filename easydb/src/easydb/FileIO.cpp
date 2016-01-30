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
		d2d::Sprite* sprite = loadSprite(spriteValue);
		d2d::InsertSpriteSJ::Instance()->Insert(sprite);
 		spriteValue = value["sprite"][i++];
 	}
}

void FileIO::store(const char* filename)
{
	Json::Value value;

 	std::vector<d2d::Sprite*> sprites;
	Context::Instance()->stage->TraverseSprites(d2d::FetchAllVisitor<d2d::Sprite>(sprites));
 
 	d2d::Rect rect;
 	for (size_t i = 0, n = sprites.size(); i < n; ++i)
 	{
 		std::vector<d2d::Vector> vertices;
 		sprites[i]->GetBounding()->GetBoundPos(vertices);
 		for (size_t j = 0, m = vertices.size(); j < m; ++j)
 			rect.Combine(vertices[j]);
 	}
 	d2d::Vector offset(-rect.CenterX(), -rect.CenterY());
 
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["sprite"][i] = store(sprites[i], offset);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

d2d::Sprite* FileIO::loadSprite(const Json::Value& value)
{
	d2d::Sprite* sprite = NULL;
	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(
		value["filepath"].asString());
	sprite = d2d::SpriteFactory::Instance()->Create(symbol);
	symbol->Release();

	sprite->Load(value);

	return sprite;
}

d2d::Shape* FileIO::loadShape(const Json::Value& value)
{
	d2d::Shape* shape = NULL;
	return shape;
}

Json::Value FileIO::store(d2d::Sprite* sprite, const d2d::Vector& offset)
{
	Json::Value value;
	value["filepath"] = sprite->GetSymbol().GetFilepath();
	sprite->Store(value);
	return value;
}

Json::Value FileIO::store(d2d::Shape* shape, const d2d::Vector& offset)
{
	Json::Value value;

	return value;
}