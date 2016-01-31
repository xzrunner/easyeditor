#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"
#include "ActorInfo.h"
#include "ToolBarPanel.h"

using namespace formation;

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

	context->width = value["width"].asInt();
	context->height = value["height"].asInt();
	context->time = value["time"].asInt();
	context->toolbar->initFromContext();

	std::string dir = ee::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value actorValue = value["actor"][i++];
	while (!actorValue.isNull()) {
		ee::ISprite* sprite = load(actorValue, dir);
		context->stage->insertSprite(sprite);
		actorValue = value["actor"][i++];
	}

	context->library->loadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	Json::Value value;

	Context* context = Context::Instance();

	value["width"] = context->width;
	value["height"] = context->height;
	value["time"] = context->time;

	std::string dir = ee::FilenameTools::getFileDir(filename);

	std::vector<ee::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["actor"][i] = store(sprites[i], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::ISprite* FileIO::load(const Json::Value& value,
						   const std::string& dir)
{
	std::string filepath = value["filepath"].asString();
	filepath = ee::FilenameTools::getAbsolutePath(dir, filepath);

	ee::ISymbol* symbol = ee::SymbolMgr::Instance()->fetchSymbol(filepath);
	ee::ISprite* sprite = ee::SpriteFactory::Instance()->create(symbol);
	symbol->release();
	const float x = value["position"]["x"].asDouble(),
		y = value["position"]["y"].asDouble();
	sprite->setTransform(ee::Vector(x, y), 0);

	ActorInfo* info = new ActorInfo;
 	info->id = value["id"].asInt();
	sprite->setUserData(info);

	return sprite;
}

Json::Value FileIO::store(const ee::ISprite* sprite,
						  const std::string& dir)
{
	Json::Value value;
	
	value["filepath"] = ee::FilenameTools::getRelativePath(dir, 
		sprite->getSymbol().getFilepath()).ToStdString();

	value["position"]["x"] = sprite->getPosition().x;
	value["position"]["y"] = sprite->getPosition().y;

	ActorInfo* info = static_cast<ActorInfo*>(sprite->getUserData());
	value["id"] = info->id;

	return value;
}