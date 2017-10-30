#include "FileStorer.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/Sprite.h>
#include <ee/SymbolType.h>
#include <ee/SymbolPath.h>

#include <sprite2/Sprite.h>

#include <wx/msgdlg.h>

#include <fstream>

namespace ecomplex
{

void FileStorer::Store(const std::string& filepath, const Symbol& sym, const std::string& _dir)
{
	CheckName(sym);

	Json::Value value;

//	centerSymbol(sym);

	value["name"] = sym.name.c_str();
	value["tag"] = sym.tag.c_str();

	const sm::rect& scissor = sym.GetScissor();
	value["xmin"] = scissor.xmin;
	value["xmax"] = scissor.xmax;
	value["ymin"] = scissor.ymin;
	value["ymax"] = scissor.ymax;

	value["use_render_cache"] = sym.m_use_render_cache;

	std::string dir;
	if (filepath != ee::SYM_GROUP_TAG) {
		dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	} else {
		dir = _dir;
	}
	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		value["sprite"][i] = Store(child, dir);
	}

//	StoreAction(sym, value);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileStorer::StoreWithHistory(const std::string& filepath, const Symbol& sym, const std::string& _dir)
{
	CheckName(sym);

	Json::Value value;

	value["name"] = sym.name.c_str();
	value["tag"] = sym.tag.c_str();

	const sm::rect& scissor = sym.GetScissor();
	value["xmin"] = scissor.xmin;
	value["xmax"] = scissor.xmax;
	value["ymin"] = scissor.ymin;
	value["ymax"] = scissor.ymax;

	value["use_render_cache"] = sym.m_use_render_cache;

	std::string dir;
	if (filepath != ee::SYM_GROUP_TAG) {
		dir	= ee::FileHelper::GetFileDir(filepath) + "\\";
	} else {
		dir = _dir;
	}

	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		value["sprite"][i] = Store(child, dir);
	}

//	StoreAction(sym, value);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	if (fout.fail()) {
		throw ee::Exception("Can't save file: %s !", filepath);
	}
	writer.write(fout, value);
	fout.close();

//	editpanel->saveHistoryList(filepath, sym.m_sprs);
}

void FileStorer::CenterSymbol(Symbol& sym)
{
	sm::vec2 offset = sym.GetBounding().Center();
	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		child->Translate(-offset);
	}
	sym.GetBounding().Translate(-offset);
}

Json::Value FileStorer::Store(const ee::SprPtr& spr, const std::string& dir)
{
	Json::Value value;
	auto sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());

	// filepath
	value["filepath"] = ee::SymbolPath::GetRelativePath(*sym, dir).c_str();
	// filepaths
	const std::set<std::string>& filepaths = sym->GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = itr->c_str();
	}
	// other
	spr->Store(value, dir);

	return value;
}

void FileStorer::CheckName(const Symbol& sym)
{
	CheckDuplicateName(sym);
	CheckNameDiff(sym);
	CheckAnchorName(sym);
}

void FileStorer::CheckDuplicateName(const Symbol& sym)
{
	std::set<std::string> names_set;
	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		auto spr = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		CU_STR name;
		s2::SprNameMap::Instance()->IDToStr(spr->GetName(), name);
		if (name.empty() || name[0] == '_') {
			continue;
		}

		auto itr = names_set.find(name.c_str());
		if (itr == names_set.end()) {
			names_set.insert(name.c_str());
		} else {
			std::string msg = sym.GetFilepath() + ": " + name.c_str();
			wxMessageBox(msg.c_str(), "重名");
		}
	}
}

void FileStorer::CheckNameDiff(const Symbol& sym)
{
	std::set<std::string> curr_names;
	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		CU_STR name;
		s2::SprNameMap::Instance()->IDToStr(children[i]->GetName(), name);
		if (!name.empty() && name[0] != '_') {
			curr_names.insert(name.c_str());
		}
	}

	std::vector<std::string> leak_names;
	const std::vector<std::string>& origin_names = sym.GetOriginNames();
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

	std::string msg = sym.GetFilepath() + ": " + str;
	wxMessageBox(msg.c_str(), "删除的名字");
}

void FileStorer::CheckAnchorName(const Symbol& sym)
{
	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		auto& spr = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		CU_STR name;
		s2::SprNameMap::Instance()->IDToStr(spr->GetName(), name);
		if (spr->IsAnchor() && name.compare(0, strlen("_sprite"), "_sprite") == 0) 
		{
			auto ee_sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
			std::string msg = sym.GetFilepath() + ": " + ee_sym->GetName();
			wxMessageBox(msg.c_str(), "anchor没有名字");
		}
	}
}

// void FileStorer::StoreAction(const Symbol& sym, Json::Value& val)
// {
// 	const std::vector<s2::ComplexSymbol::Action>& actions = sym.GetActions();
// 	if (actions.empty()) {
// 		return;
// 	}
// 
// 	Json::Value action_val;
// 
// 	auto& children = sym.GetChildren();
// 	for (int i = 0, n = actions.size(); i < n; ++i) {
// 		const s2::ComplexSymbol::Action& action = actions[i];
// 		action_val[i]["name"] = action.name;
// 		for (int j = 0, m = action.sprs.size(); j < m; ++j) {
// 			int idx = -1;
// 			for (int k = 0; k < children.size(); ++k) {
// 				if (children[k] == action.sprs[j]) {
// 					idx = k;
// 					break;
// 				}
// 			}
// 			if (idx != -1) {
// 				action_val[i]["sprite"][j] = idx;
// 			}
// 		}
// 	}
// 
// 	val["action"] = action_val;
// }


} // complex