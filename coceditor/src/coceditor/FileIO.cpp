#include "FileIO.h"
#include "StagePanel.h"
#include "LibraryPanel.h"
#include "Context.h"

namespace coceditor
{

void FileIO::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	std::string dlg = ee::FilenameTools::getFileDir(filename);
	ee::Context::Instance()->resPath = dlg + "\\";

 	int i = 0;
 	Json::Value spriteValue = value["sprite"][i++];
 	while (!spriteValue.isNull()) {
		ee::ISprite* sprite = load(spriteValue, dlg);
		Context::Instance()->stage->insertSprite(sprite);
 		spriteValue = value["sprite"][i++];
 	}

	Context::Instance()->library->loadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	Json::Value value;

 	std::vector<ee::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites));
 
	std::string dlg = ee::FilenameTools::getFileDir(filename);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["sprite"][i] = store(sprites[i], dlg);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();
}

ee::ISprite* FileIO::load(const Json::Value& value, const std::string& dlg)
{
	ee::ISprite* sprite = NULL;
	std::string path = ee::FilenameTools::getAbsolutePath(dlg, value["filepath"].asString());
	ee::ISymbol* symbol = ee::SymbolMgr::Instance()->getSymbol(path);
	sprite = ee::SpriteFactory::Instance()->create(symbol);

	sprite->setUserData(new int(value["id"].asDouble()));

	ee::Vector pos;
	pos.x = value["position"]["x"].asDouble();
	pos.y = value["position"]["y"].asDouble();
	sprite->setTransform(pos, value["angle"].asDouble());

	sprite->setScale(value["scale"].asDouble());

	sprite->setMirror(value["x mirror"].asBool(), 
		value["y mirror"].asBool());

	return sprite;
}

Json::Value FileIO::store(ee::ISprite* sprite, const std::string& dlg)
{
	Json::Value value;

	value["id"] = *static_cast<int*>(sprite->getUserData());

	value["filepath"] = ee::FilenameTools::getRelativePath(dlg,
		sprite->getSymbol().getFilepath()).ToStdString();

	value["position"]["x"] = sprite->getPosition().x;
	value["position"]["y"] = sprite->getPosition().y;

	value["angle"] = sprite->getAngle();

	value["scale"] = sprite->getScaleX();

	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
	value["x mirror"] = xMirror;
	value["y mirror"] = yMirror;

	return value;
}
} // coceditor