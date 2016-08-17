#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"

#include <ee/sprite_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolMgr.h>

#include <sprite2/BoundingBox.h>

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
		ee::Sprite* spr = LoadSprite(spriteValue);
		ee::InsertSpriteSJ::Instance()->Insert(spr);
 		spriteValue = value["sprite"][i++];
 	}
}

void FileIO::Store(const char* filename)
{
	Json::Value value;

 	std::vector<ee::Sprite*> sprs;
	Context::Instance()->stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
 
 	sm::rect rect;
 	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->GetBounding()->CombineTo(rect);
 	}
 	sm::vec2 offset = -rect.Center();
 
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
		value["sprite"][i] = Store(sprs[i], offset);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::Sprite* FileIO::LoadSprite(const Json::Value& value)
{
	ee::Sprite* spr = NULL;
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(
		value["filepath"].asString());
	spr = ee::SpriteFactory::Instance()->Create(sym);
	sym->RemoveReference();

	spr->Load(value);

	return spr;
}

ee::Shape* FileIO::LoadShape(const Json::Value& value)
{
	ee::Shape* shape = NULL;
	return shape;
}

Json::Value FileIO::Store(ee::Sprite* spr, const sm::vec2& offset)
{
	Json::Value value;
	value["filepath"] = dynamic_cast<const ee::Symbol*>(spr->GetSymbol())->GetFilepath();
	spr->Store(value);
	return value;
}

Json::Value FileIO::Store(ee::Shape* shape, const sm::vec2& offset)
{
	Json::Value value;

	return value;
}

}