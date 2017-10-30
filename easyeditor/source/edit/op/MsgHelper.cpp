#include "MsgHelper.h"
#include "EditedFileStack.h"
#include "PackIDMgr.h"
#include "Symbol.h"
#include "SymbolMgr.h"

#include <sprite2/ComplexSymbol.h>

namespace ee
{

void MsgHelper::FillingSprites(const std::vector<SprPtr>& children, Json::Value& val)
{
	auto parent = EditedFileStack::Instance()->Top();
	if (parent.empty()) {
		return;
	}

	PackIDMgr* id_mgr = PackIDMgr::Instance();

	val["parent"] = id_mgr->QueryNode(parent);

	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		auto child = children[i];
		auto c_sym = S2_VI_PTR_DOWN_CAST<const Symbol>(child->GetSymbol());

		Json::Value cval;

		cval["id"] = id_mgr->QueryNode(c_sym->GetFilepath());
		cval["name"] = child->GetName();

		int idx = -1;
		auto sym = SymbolMgr::Instance()->FetchSymbol(parent);
		if (sym && sym->Type() == s2::SYM_COMPLEX)
		{
			auto comp_sym = S2_VI_PTR_DOWN_CAST<s2::ComplexSymbol>(sym);
			auto& children = comp_sym->GetAllChildren();
			for (int i = 0, n = children.size(); i < n; ++i) {
				if (child == children[i]) {
					idx = i;
					break;
				}
			}
		}
		cval["idx"] = idx;

		val["children"][i] = cval;
	}
}

void MsgHelper::FillingSprites(const Sprite& child, Json::Value& val)
{
	auto parent = EditedFileStack::Instance()->Top();
	if (parent.empty()) {
		return;
	}

	PackIDMgr* id_mgr = PackIDMgr::Instance();

	val["parent"] = id_mgr->QueryNode(parent);

	auto c_sym = S2_VI_PTR_DOWN_CAST<const Symbol>(child.GetSymbol());

	val["child_id"] = id_mgr->QueryNode(c_sym->GetFilepath());
	val["child_name"] = child.GetName();

	int idx = -1;
	auto sym = SymbolMgr::Instance()->FetchSymbol(parent);
	if (sym && sym->Type() == s2::SYM_COMPLEX)
	{
		auto comp_sym = S2_VI_PTR_DOWN_CAST<s2::ComplexSymbol>(sym);
		auto& children = comp_sym->GetAllChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (&child == children[i].get()) {
				idx = i;
				break;
			}
		}
	}
	val["child_idx"] = idx;
}

}