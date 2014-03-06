#include "FileIO.h"
#include "StagePanel.h"
#include "Context.h"
#include "ResizeCMPT.h"

#include <easyscale9.h>

namespace escale9
{

void FileIO::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	std::string dlg = d2d::FilenameTools::getFileDir(filename);
	d2d::Context::Instance()->resPath = dlg + "\\";

	Context* context = Context::Instance();

 	int i = 0;
 	Json::Value spriteValue = value["sprite"][i++];
 	while (!spriteValue.isNull()) {
		d2d::ISprite* sprite = load(spriteValue, dlg);
		context->stage->insertSprite(sprite);
 		spriteValue = value["sprite"][i++];
 	}

	context->width = value["width"].asInt();
	context->height = value["height"].asInt();
	context->resizeCMPT->setValue(context->width, context->height);

	context->library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	Json::Value value;

	value["type"] = static_cast<Symbol*>
		(Context::Instance()->stage->getPatchSymbol())->type();

	value["width"] = Context::Instance()->width;
	value["height"] = Context::Instance()->height;

 	std::vector<d2d::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	std::string dlg = d2d::FilenameTools::getFileDir(filename);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["sprite"][i] = store(sprites[i], dlg);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filename);
	writer.write(fout, value);
	fout.close();
}

d2d::ISprite* FileIO::load(const Json::Value& value, const std::string& dlg)
{
	d2d::ISprite* sprite = NULL;
	std::string path = d2d::FilenameTools::getAbsolutePath(dlg, value["filepath"].asString());
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(path);
	sprite = d2d::SpriteFactory::Instance()->create(symbol);

	sprite->name = value["name"].asString();

	d2d::Vector pos;
	pos.x = value["position"]["x"].asDouble();
	pos.y = value["position"]["y"].asDouble();
	sprite->setTransform(pos, value["angle"].asDouble());

	sprite->setScale(value["scale"].asDouble());

	sprite->setMirror(value["x mirror"].asBool(), 
		value["y mirror"].asBool());

	return sprite;
}

Json::Value FileIO::store(d2d::ISprite* sprite, const std::string& dlg)
{
	Json::Value value;

	value["filepath"] = d2d::FilenameTools::getRelativePath(dlg,
		sprite->getSymbol().getFilepath()).ToStdString();

	value["name"] = sprite->name;

	value["position"]["x"] = sprite->getPosition().x;
	value["position"]["y"] = sprite->getPosition().y;

	value["angle"] = sprite->getAngle();

	value["scale"] = sprite->getScale().x;

	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
	value["x mirror"] = xMirror;
	value["y mirror"] = yMirror;

	return value;
}
} // escale9