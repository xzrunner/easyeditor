#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"
#include "ActorInfo.h"

using namespace raiden;

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

	std::string dir = ee::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		ee::Layer* layer = loadLayer(layerValue, dir);
		context->layers->addLayer(layer);
		layerValue = value["layer"][i++];
	}

	context->library->loadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	Json::Value value;

	std::string dir = ee::FilenameTools::getFileDir(filename);

	std::vector<ee::Layer*> layers;
	Context::Instance()->layers->traverseAllLayers(ee::FetchAllVisitor<ee::Layer>(layers));
	for (size_t i = 0, n = layers.size(); i < n; ++i)
		value["layer"][i] = store(layers[i], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::Layer* FileIO::loadLayer(const Json::Value& value,
							  const std::string& dir)
{
	ee::Layer* layer = new ee::Layer;

	layer->setName(value["name"].asString());

	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		ee::ISprite* sprite = loadSprite(spriteValue, dir);
		layer->insert(sprite);
		spriteValue = value["sprite"][i++];
	}

	return layer;
}

Json::Value FileIO::store(const ee::Layer* layer,
						  const std::string& dir)
{
	Json::Value value;

	value["name"] = layer->getName().ToStdString();

	std::vector<ee::ISprite*> sprites = layer->getSprites();
	std::sort(sprites.begin(), sprites.end(), ee::SpriteCmp());
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["sprite"][i] = store(sprites[i], dir);

	return value;
}

ee::ISprite* FileIO::loadSprite(const Json::Value& value,
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
	info->speed = value["speed"].asInt();
	info->id = value["id"].asInt();

	std::string pathfile = value["path"].asString();
	if (!pathfile.empty())
	{
		pathfile = ee::FilenameTools::getAbsolutePath(dir, pathfile);
		// todo release symbol
		info->symbol = ee::SymbolMgr::Instance()->fetchSymbol(pathfile);
		info->resetOffset();
	}

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
	value["speed"] = info->speed;
 	value["id"] = info->id;

	wxString filepath;
	if (info->symbol)
	{
		if (info->symbol->getFilepath().empty())
			filepath = info->symbol->getName() + "_" + ee::FileNameParser::getFileTag(ee::FileNameParser::e_shape) + ".json";
		else
			filepath = info->symbol->getFilepath();
		value["path"] = ee::FilenameTools::getRelativePath(dir, filepath).ToStdString();
	}
	else
		value["path"] = "";

	return value;
}