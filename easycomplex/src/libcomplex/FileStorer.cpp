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
	CheckNameDiff(sym);

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

	StoreAction(sym, value);

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
	CheckNameDiff(sym);

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

	StoreAction(sym, value);

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
			wxMessageBox(name, "����");
		}
	}
}

void FileStorer::CheckNameDiff(const Symbol* sym)
{
	std::set<std::string> curr_names;
	const std::vector<s2::Sprite*>& children = sym->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		const std::string& name = children[i]->GetName();
		if (!name.empty() && name[0] != '_') {
			curr_names.insert(children[i]->GetName());
		}
	}

	std::vector<std::string> leak_names;
	const std::vector<std::string>& origin_names = sym->GetOriginNames();
	for (int i = 0, n = origin_names.size(); i < n; ++i) {
		if (curr_names.find(origin_names[i]) == curr_names.end()) {
			leak_names.push_back(origin_names[i]);
		}
	}

	if (leak_names.empty()) {
		return;
	}

	std::string str;
	for (int i = 0, n = leak_names.size(); i < n; ++i) {
		str += leak_names[i] + ", ";
	}

	wxMessageBox(str, "ɾ��������");
}

void FileStorer::StoreAction(const Symbol* sym, Json::Value& val)
{
	const std::vector<s2::ComplexSymbol::Action>& actions = sym->GetActions();
	if (actions.empty()) {
		return;
	}

	Json::Value action_val;

	const std::vector<s2::Sprite*>& children = sym->GetChildren();
	for (int i = 0, n = actions.size(); i < n; ++i) {
		const s2::ComplexSymbol::Action& action = actions[i];
		action_val[i]["name"] = action.name;
		for (int j = 0, m = action.sprs.size(); j < m; ++j) {
			int idx = -1;
			for (int k = 0; k < children.size(); ++k) {
				if (children[k] == action.sprs[j]) {
					idx = k;
					break;
				}
			}
			if (idx != -1) {
				action_val[i]["sprite"][j] = idx;
			}
		}
	}

	val["action"] = action_val;
}


} // complex