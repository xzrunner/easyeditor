#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"
#include "ActorInfo.h"

using namespace raiden;

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

	std::string dlg = d2d::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		d2d::Layer* layer = loadLayer(layerValue, dlg);
		context->layers->addLayer(layer);
		layerValue = value["layer"][i++];
	}

	context->library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	Json::Value value;

	std::string dlg = d2d::FilenameTools::getFileDir(filename);

	std::vector<d2d::Layer*> layers;
	Context::Instance()->layers->traverseAllLayers(d2d::FetchAllVisitor<d2d::Layer>(layers));
	for (size_t i = 0, n = layers.size(); i < n; ++i)
		value["layer"][i] = store(layers[i], dlg);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filename);
	writer.write(fout, value);
	fout.close();
}

d2d::Layer* FileIO::loadLayer(const Json::Value& value,
							  const std::string& dlg)
{
	d2d::Layer* layer = new d2d::Layer;

	layer->setName(value["name"].asString());

	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		d2d::ISprite* sprite = loadSprite(spriteValue, dlg);
		layer->insert(sprite);
		spriteValue = value["sprite"][i++];
	}

	return layer;
}

Json::Value FileIO::store(const d2d::Layer* layer,
						  const std::string& dlg)
{
	Json::Value value;

	value["name"] = layer->getName().ToStdString();

	std::vector<d2d::ISprite*> sprites = layer->getSprites();
	std::sort(sprites.begin(), sprites.end(), d2d::SpriteCmp());
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["sprite"][i] = store(sprites[i], dlg);

	return value;
}

d2d::ISprite* FileIO::loadSprite(const Json::Value& value,
								 const std::string& dlg)
{
	std::string filepath = value["filepath"].asString();
	filepath = d2d::FilenameTools::getAbsolutePath(dlg, filepath);

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	const float x = value["position"]["x"].asDouble(),
		y = value["position"]["y"].asDouble();
	sprite->setTransform(d2d::Vector(x, y), 0);

	ActorInfo* info = new ActorInfo;
	info->speed = value["speed"].asInt();
	info->id = value["id"].asInt();

	std::string pathfile = value["path"].asString();
	if (!pathfile.empty())
	{
		pathfile = d2d::FilenameTools::getAbsolutePath(dlg, pathfile);
		info->symbol = d2d::SymbolMgr::Instance()->getSymbol(pathfile);
		info->resetOffset();
	}

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
	value["speed"] = info->speed;
 	value["id"] = info->id;

	wxString filepath;
	if (info->symbol)
	{
		if (info->symbol->getFilepath().empty())
			filepath = info->symbol->getName() + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
		else
			filepath = info->symbol->getFilepath();
		value["path"] = d2d::FilenameTools::getRelativePath(dlg, filepath).ToStdString();
	}
	else
		value["path"] = "";

	return value;
}