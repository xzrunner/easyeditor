#include "FileIO.h"
#include "StagePanel.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"

#include <easyscale9.h>

namespace escale9
{

void FileIO::load(const char* filename, d2d::LibraryPanel* library, 
				  d2d::MultiSpritesImpl* stage, ToolbarPanel* toolbar)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString dir = d2d::FilenameTools::getFileDir(filename);

 	int i = 0;
 	Json::Value spriteValue = value["sprite"][i++];
 	while (!spriteValue.isNull()) {
		d2d::ISprite* sprite = load(spriteValue, dir);
		stage->insertSprite(sprite);
 		spriteValue = value["sprite"][i++];
 	}

	float width = value["width"].asInt(),
		  height = value["height"].asInt();
	toolbar->setSize(width, height);

	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename, StagePanel* stage, 
				   ToolbarPanel* toolbar)
{
	Json::Value value;

	value["type"] = stage->getPatchSymbol()->GetScale9Data().GetType();

	value["width"] = toolbar->getWidth();
	value["height"] = toolbar->getHeight();

 	std::vector<d2d::ISprite*> sprites;
	stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	wxString dir = d2d::FilenameTools::getFileDir(filename);
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		value["sprite"][i] = store(sprites[i], dir);
	}

	Symbol* symbol = stage->getPatchSymbol();
	const Scale9Data& s_data = symbol->GetScale9Data();
	int idx = 0;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			d2d::ISprite* sprite = s_data.m_sprites[i][j];
			if (sprite) {
				value["sprite new"][idx++] = StoreNew(sprite, dir);
			} else {
//				value["sprite new"][idx++] = NULL;
				idx++;
			}
		}
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

d2d::ISprite* FileIO::load(const Json::Value& value, const wxString& dir)
{
	d2d::ISprite* sprite = NULL;
	std::string path = d2d::FilenameTools::getAbsolutePath(dir, value["filepath"].asString());
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
	sprite = d2d::SpriteFactory::Instance()->create(symbol);
	symbol->Release();

	sprite->name = value["name"].asString();

	d2d::Vector pos;
	pos.x = value["position"]["x"].asDouble();
	pos.y = value["position"]["y"].asDouble();
	sprite->setTransform(pos, value["angle"].asDouble());

	float scale = value["scale"].asDouble();
	sprite->setScale(scale, scale);

	sprite->setMirror(value["x mirror"].asBool(), 
		value["y mirror"].asBool());

	return sprite;
}

Json::Value FileIO::store(d2d::ISprite* sprite, const wxString& dir)
{
	Json::Value value;

	value["filepath"] = d2d::FilenameTools::getRelativePath(dir,
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

Json::Value FileIO::StoreNew(d2d::ISprite* sprite, const wxString& dir)
{
	Json::Value value;
	value["filepath"] = d2d::FilenameTools::getRelativePath(dir,
		sprite->getSymbol().getFilepath()).ToStdString();
	sprite->store(value);
	return value;
}

} // escale9