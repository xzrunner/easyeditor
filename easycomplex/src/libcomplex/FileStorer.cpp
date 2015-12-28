#include "FileStorer.h"
#include "Symbol.h"

namespace ecomplex
{

void FileStorer::Store(const char* filepath, const Symbol* symbol)
{
	Json::Value value;

//	centerSymbol(symbol);

	value["name"] = symbol->name;
	value["tag"] = symbol->tag;

	value["xmin"] = symbol->m_clipbox.xMin;
	value["xmax"] = symbol->m_clipbox.xMax;
	value["ymin"] = symbol->m_clipbox.yMin;
	value["ymax"] = symbol->m_clipbox.yMax;

	value["use_render_cache"] = symbol->m_use_render_cache;

	wxString dir = d2d::FilenameTools::getFileDir(filepath) + "\\";
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		value["sprite"][i] = Store(symbol->m_sprites[i], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileStorer::StoreWithHistory(const char* filepath, const Symbol* symbol)
{
	Json::Value value;

	value["name"] = symbol->name;
	value["tag"] = symbol->tag;

	value["xmin"] = symbol->m_clipbox.xMin;
	value["xmax"] = symbol->m_clipbox.xMax;
	value["ymin"] = symbol->m_clipbox.yMin;
	value["ymax"] = symbol->m_clipbox.yMax;

	value["use_render_cache"] = symbol->m_use_render_cache;

	wxString dir = d2d::FilenameTools::getFileDir(filepath) + "\\";
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		value["sprite"][i] = Store(symbol->m_sprites[i], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));
	if (fout.fail()) {
		throw d2d::Exception("Can't save file: %s !", filepath);
	}
	writer.write(fout, value);
	fout.close();

//	editpanel->saveHistoryList(filepath, symbol->m_sprites);
}

void FileStorer::CenterSymbol(Symbol* symbol)
{
	d2d::Vector offset;
	offset.x = symbol->m_rect.xCenter();
	offset.y = symbol->m_rect.yCenter();
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = symbol->m_sprites[i];
		sprite->Translate(-offset);
	}
	symbol->m_rect.translate(-offset);
}

Json::Value FileStorer::Store(d2d::ISprite* sprite, const wxString& dir)
{
	Json::Value value;
	const d2d::ISymbol& symbol = sprite->GetSymbol();

	// filepath
	value["filepath"] = d2d::FilenameTools::getRelativePath(dir,
		symbol.GetFilepath()).ToStdString();
	// filepaths
	const std::set<std::string>& filepaths = symbol.GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	sprite->Store(value);

	return value;
}

} // complex