#include "FileStorer.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/Sprite.h>

#include <sprite2/S2_Sprite.h>

#include <wx/msgdlg.h>

#include <fstream>

namespace ecomplex
{

void FileStorer::Store(const char* filepath, const Symbol* sym)
{
	CheckDuplicateName(sym);

	Json::Value value;

//	centerSymbol(sym);

	value["name"] = sym->name;
	value["tag"] = sym->tag;

	const sm::rect& scissor = sym->GetScissor();
	value["xmin"] = scissor.xmin;
	value["xmax"] = scissor.xmax;
	value["ymin"] = scissor.ymin;
	value["ymax"] = scissor.ymax;

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
	CheckDuplicateName(sym);

	Json::Value value;

	value["name"] = sym->name;
	value["tag"] = sym->tag;

	const sm::rect& scissor = sym->GetScissor();
	value["xmin"] = scissor.xmin;
	value["xmax"] = scissor.xmax;
	value["ymin"] = scissor.ymin;
	value["ymax"] = scissor.ymax;

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

void FileStorer::CheckDuplicateName(const Symbol* sym)
{
	std::set<std::string> names_set;
	const std::vector<s2::Sprite*>& children = sym->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		ee::Sprite* spr = dynamic_cast<ee::Sprite*>(children[i]);
		std::string name = spr->GetName();
		if (name.empty() || name[0] == '_') {
			continue;
		}

		std::set<std::string>::iterator itr = names_set.find(name);
		if (itr == names_set.end()) {
			names_set.insert(name);
		} else {
			wxMessageBox(name, "ÖØÃû");
		}
	}
}

} // complex