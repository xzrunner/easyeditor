#include "FileStorer.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/Sprite.h>

#include <sprite2/S2_Sprite.h>

#include <fstream>

namespace ecomplex
{

void FileStorer::Store(const char* filepath, const Symbol* sym)
{
	Json::Value value;

//	centerSymbol(sym);

	value["name"] = sym->name;
	value["tag"] = sym->tag;

	value["xmin"] = sym->m_clipbox.xmin;
	value["xmax"] = sym->m_clipbox.xmax;
	value["ymin"] = sym->m_clipbox.ymin;
	value["ymax"] = sym->m_clipbox.ymax;

	value["use_render_cache"] = sym->m_use_render_cache;

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	const std::vector<s2::Sprite*>& children = sym->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		value["sprite"][i] = Store(child, dir);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileStorer::StoreWithHistory(const char* filepath, const Symbol* sym)
{
	Json::Value value;

	value["name"] = sym->name;
	value["tag"] = sym->tag;

	value["xmin"] = sym->m_clipbox.xmin;
	value["xmax"] = sym->m_clipbox.xmax;
	value["ymin"] = sym->m_clipbox.ymin;
	value["ymax"] = sym->m_clipbox.ymax;

	value["use_render_cache"] = sym->m_use_render_cache;

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	const std::vector<s2::Sprite*>& children = sym->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		value["sprite"][i] = Store(child, dir);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));
	if (fout.fail()) {
		throw ee::Exception("Can't save file: %s !", filepath);
	}
	writer.write(fout, value);
	fout.close();

//	editpanel->saveHistoryList(filepath, sym->m_sprs);
}

void FileStorer::CenterSymbol(Symbol* sym)
{
	sm::vec2 offset = sym->GetBounding().Center();
	const std::vector<s2::Sprite*>& children = sym->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		child->Translate(-offset);
	}
	sym->GetBounding().Translate(-offset);
}

Json::Value FileStorer::Store(ee::Sprite* spr, const std::string& dir)
{
	Json::Value value;
	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());

	// filepath
	value["filepath"] = ee::FileHelper::GetRelativePath(dir, sym->GetFilepath());
	// filepaths
	const std::set<std::string>& filepaths = sym->GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	spr->Store(value, dir);

	return value;
}

} // complex