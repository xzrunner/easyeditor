#include "StoreToJson.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/SymbolType.h>
#include <ee/SymbolPath.h>

#include <js/RapidJsonHelper.h>

#include <wx/msgdlg.h>

namespace ecomplex
{

void StoreToJson::StoreToFile(const std::string& filepath, const Symbol& sym, const std::string& _dir, bool store_history)
{
	rapidjson::Document doc;

	std::string dir;
	if (filepath != ee::SYM_GROUP_TAG) {
		dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	} else {
		dir = _dir;
	}

	StoreToMem(doc, sym, dir);

	js::RapidJsonHelper::WriteToFile(filepath.c_str(), doc);

	if (store_history) {
//		editpanel->saveHistoryList(filepath, sym.m_sprs);
	}
}

void StoreToJson::StoreToMem(rapidjson::Document& doc, const Symbol& sym, const std::string& dir)
{
	CheckName(sym);

//	centerSymbol(sym);

	auto& alloc = doc.GetAllocator();

	doc["name"].SetString(sym.name.c_str(), alloc);
	doc["tag"].SetString(sym.tag.c_str(), alloc);

	const sm::rect& scissor = sym.GetScissor();
	doc["xmin"] = scissor.xmin;
	doc["xmax"] = scissor.xmax;
	doc["ymin"] = scissor.ymin;
	doc["ymax"] = scissor.ymax;

	doc["use_render_cache"] = sym.m_use_render_cache;

	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		doc["sprite"].PushBack(StoreSpr(child, dir, alloc).Move(), alloc);
	}

//	StoreAction(sym, val);
}

void StoreToJson::CenterSymbol(Symbol& sym)
{
	sm::vec2 offset = sym.GetBounding().Center();
	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		child->Translate(-offset);
	}
	sym.GetBounding().Translate(-offset);
}

rapidjson::Value StoreToJson::StoreSpr(const ee::SprPtr& spr, const std::string& dir,
	                                   rapidjson::MemoryPoolAllocator<>& alloc)
{
	rapidjson::Value value;
	auto sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());

	// filepath
	value["filepath"].SetString(ee::SymbolPath::GetRelativePath(*sym, dir).c_str(), alloc);

	// filepaths
	auto& filepaths = sym->GetFilepaths();
	for (auto& filepath : filepaths) {
		value["filepaths"].PushBack(rapidjson::Value(filepath.c_str(), alloc).Move(), alloc);
	}

	// other
	spr->Store(value, dir);

	return value;
}

void StoreToJson::CheckName(const Symbol& sym)
{
	CheckDuplicateName(sym);
	CheckNameDiff(sym);
	CheckAnchorName(sym);
}

void StoreToJson::CheckDuplicateName(const Symbol& sym)
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

void StoreToJson::CheckNameDiff(const Symbol& sym)
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

void StoreToJson::CheckAnchorName(const Symbol& sym)
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

// void StoreToJson::StoreAction(const Symbol& sym, Json::Value& val)
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

}