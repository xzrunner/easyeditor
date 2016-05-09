#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"

#include <ee/sprite_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Rect.h>
#include <ee/Sprite.h>
#include <ee/SpriteFactory.h>
#include <ee/BoundingBox.h>
#include <ee/SymbolMgr.h>

#include <fstream>

namespace edb
{

void FileIO::Load(const char* filename)
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
		ee::Sprite* sprite = LoadSprite(spriteValue);
		ee::InsertSpriteSJ::Instance()->Insert(sprite);
 		spriteValue = value["sprite"][i++];
 	}
}

void FileIO::Store(const char* filename)
{
	Json::Value value;

 	std::vector<ee::Sprite*> sprites;
	Context::Instance()->stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
 
 	ee::Rect rect;
 	for (size_t i = 0, n = sprites.size(); i < n; ++i)
 	{
 		std::vector<sm::vec2> vertices;
 		sprites[i]->GetBounding()->GetBoundPos(vertices);
 		for (size_t j = 0, m = vertices.size(); j < m; ++j)
 			rect.Combine(vertices[j]);
 	}
 	sm::vec2 offset(-rect.CenterX(), -rect.CenterY());
 
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["sprite"][i] = Store(sprites[i], offset);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::Sprite* FileIO::LoadSprite(const Json::Value& value)
{
	ee::Sprite* sprite = NULL;
	ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(
		value["filepath"].asString());
	sprite = ee::SpriteFactory::Instance()->Create(symbol);
	symbol->Release();

	sprite->Load(value);

	return sprite;
}

ee::Shape* FileIO::LoadShape(const Json::Value& value)
{
	ee::Shape* shape = NULL;
	return shape;
}

Json::Value FileIO::Store(ee::Sprite* sprite, const sm::vec2& offset)
{
	Json::Value value;
	value["filepath"] = sprite->GetSymbol().GetFilepath();
	sprite->Store(value);
	return value;
}

Json::Value FileIO::Store(ee::Shape* shape, const sm::vec2& offset)
{
	Json::Value value;

	return value;
}

}