#include "FileSaver.h"

namespace libcomplex
{

void FileSaver::store(const char* filepath, const d2d::ComplexSymbol* symbol)
{
	Json::Value value;

//	centerSymbol(symbol);

	value["name"] = symbol->name;

	wxString dir = d2d::FilenameTools::getFileDir(filepath) + "\\";
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		value["sprite"][i] = store(symbol->m_sprites[i], dir);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath);
	writer.write(fout, value);
	fout.close();
}

void FileSaver::storeWithHistory(const char* filepath, const d2d::ComplexSymbol* symbol,
								 d2d::EditPanel* editpanel)
{
	Json::Value value;

	value["name"] = symbol->name;

	wxString dir = d2d::FilenameTools::getFileDir(filepath) + "\\";
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		value["sprite"][i] = store(symbol->m_sprites[i], dir);

	editpanel->saveHistoryList(value, symbol->m_sprites);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath);
	writer.write(fout, value);
	fout.close();
}

void FileSaver::centerSymbol(d2d::ComplexSymbol* symbol)
{
	d2d::Vector offset;
	offset.x = symbol->m_rect.xCenter();
	offset.y = symbol->m_rect.yCenter();
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = symbol->m_sprites[i];
		sprite->translate(-offset);
	}
	symbol->m_rect.translate(-offset);
}

Json::Value FileSaver::store(d2d::ISprite* sprite, const wxString& dir)
{
	Json::Value value;

	//		value["id"] = *static_cast<int*>(sprite->getUserData());
	void* data = sprite->getUserData();

	value["filepath"] = d2d::FilenameTools::getRelativePath(dir,
		sprite->getSymbol().getFilepath()).ToStdString();

	value["name"] = sprite->name;

	value["multi color"] = sprite->multiColor;
	value["add color"] = sprite->addColor;

	value["position"]["x"] = sprite->getPosition().x;
	value["position"]["y"] = sprite->getPosition().y;

	value["angle"] = sprite->getAngle();

	value["x scale"] = sprite->getScaleX();
	value["y scale"] = sprite->getScaleY();

	value["x shear"] = sprite->getShearX();
	value["y shear"] = sprite->getShearY();

	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
	value["x mirror"] = xMirror;
	value["y mirror"] = yMirror;

	return value;
}
} // libcomplex