#include "FileSaver.h"
#include "Symbol.h"

namespace complex
{

void FileSaver::store(const char* filepath, const Symbol* symbol)
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

void FileSaver::storeWithHistory(const char* filepath, const Symbol* symbol,
								 d2d::EditPanel* editpanel)
{
	Json::Value value;

	value["name"] = symbol->name;

	value["xmin"] = symbol->m_clipbox.xMin;
	value["xmax"] = symbol->m_clipbox.xMax;
	value["ymin"] = symbol->m_clipbox.yMin;
	value["ymax"] = symbol->m_clipbox.yMax;

	wxString dir = d2d::FilenameTools::getFileDir(filepath) + "\\";
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		value["sprite"][i] = store(symbol->m_sprites[i], dir);

	editpanel->saveHistoryList(filepath, symbol->m_sprites);
}

void FileSaver::centerSymbol(Symbol* symbol)
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
	value["filepath"] = d2d::FilenameTools::getRelativePath(dir,
		sprite->getSymbol().getFilepath()).ToStdString();
	sprite->store(value);
	return value;
}

} // complex