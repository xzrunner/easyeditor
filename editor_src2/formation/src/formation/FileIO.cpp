#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"
#include "ActorInfo.h"
#include "ToolBarPanel.h"

using namespace formation;

void FileIO::load(const char* filename)
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();

	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	Context* context = Context::Instance();

	context->width = value["width"].asInt();
	context->height = value["height"].asInt();
	context->time = value["time"].asInt();
	context->toolbar->initFromContext();

	std::string dlg = d2d::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value actorValue = value["actor"][i++];
	while (!actorValue.isNull()) {
		d2d::ISprite* sprite = load(actorValue, dlg);
		context->stage->insertSprite(sprite);
		actorValue = value["actor"][i++];
	}

	context->library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	Json::Value value;

	Context* context = Context::Instance();

	value["width"] = context->width;
	value["height"] = context->height;
	value["time"] = context->time;

	std::string dlg = d2d::FilenameTools::getFileDir(filename);

	std::vector<d2d::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["actor"][i] = store(sprites[i], dlg);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filename);
	writer.write(fout, value);
	fout.close();
}

d2d::ISprite* FileIO::load(const Json::Value& value,
						   const std::string& dlg)
{
	std::string filepath = value["filepath"].asString();
	filepath = d2d::FilenameTools::getAbsolutePath(dlg, filepath);

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	const float x = value["position"]["x"].asDouble(),
		y = value["position"]["y"].asDouble();
	sprite->setTransform(d2d::Vector(x, y), 0);

	if (x > 10000 || y > 10000)
		int zz = 0;

	ActorInfo* info = new ActorInfo;
 	info->id = value["id"].asInt();
	sprite->setUserData(info);

	return sprite;
}

Json::Value FileIO::store(const d2d::ISprite* sprite,
						  const std::string& dlg)
{
	Json::Value value;
	
	value["filepath"] = d2d::FilenameTools::getRelativePath(dlg, 
		sprite->getSymbol().getFilepath()).ToStdString();

	value["position"]["x"] = sprite->getPosition().x;
	value["position"]["y"] = sprite->getPosition().y;

	ActorInfo* info = static_cast<ActorInfo*>(sprite->getUserData());
	value["id"] = info->id;

	return value;
}