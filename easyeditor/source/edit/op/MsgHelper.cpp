#include "MsgHelper.h"
#include "EditedFileStack.h"
#include "PackIDMgr.h"
#include "Symbol.h"
#include "SymbolMgr.h"
#include "Sprite.h"

#include <sprite2/ComplexSymbol.h>

namespace ee
{

void MsgHelper::FillingSprites(const std::vector<Sprite*>& children, Json::Value& val)
{
	std::string parent = EditedFileStack::Instance()->Top();
	if (parent.empty()) {
		return;
	}

	PackIDMgr* id_mgr = PackIDMgr::Instance();

	val["parent"] = id_mgr->QueryNode(parent);

	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		Sprite* child = children[i];
		const Symbol* c_sym = VI_DOWNCASTING<const Symbol*>(child->GetSymbol());

		Json::Value cval;

		cval["id"] = id_mgr->QueryNode(c_sym->GetFilepath());
		cval["name"] = child->GetName();

		int idx = -1;
		s2::Symbol* sym = SymbolMgr::Instance()->FetchSymbol(parent);
		if (sym && sym->Type() == s2::SYM_COMPLEX)
		{
			s2::ComplexSymbol* comp_sym = VI_DOWNCASTING<s2::ComplexSymbol*>(sym);
			const std::vector<s2::Sprite*>& children = comp_sym->GetAllChildren();
			for (int i = 0, n = children.size(); i < n; ++i) {
				if (child == VI_DOWNCASTING<Sprite*>(children[i])) {
					idx = i;
					break;
				}
			}
		}
		cval["idx"] = idx;

		val["children"][i] = cval;
	}
}

void MsgHelper::FillingSprites(const Sprite* child, Json::Value& val)
{
	std::string parent = EditedFileStack::Instance()->Top();
	if (parent.empty()) {
		return;
	}

	PackIDMgr* id_mgr = PackIDMgr::Instance();

	val["parent"] = id_mgr->QueryNode(parent);

	const Symbol* c_sym = VI_DOWNCASTING<const Symbol*>(child->GetSymbol());

	val["child_id"] = id_mgr->QueryNode(c_sym->GetFilepath());
	val["child_name"] = child->GetName();

	int idx = -1;
	s2::Symbol* sym = SymbolMgr::Instance()->FetchSymbol(parent);
	if (sym && sym->Type() == s2::SYM_COMPLEX)
	{
		s2::ComplexSymbol* comp_sym = VI_DOWNCASTING<s2::ComplexSymbol*>(sym);
		const std::vector<s2::Sprite*>& children = comp_sym->GetAllChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (child == VI_DOWNCASTING<Sprite*>(children[i])) {
				idx = i;
				break;
			}
		}
	}
	val["child_idx"] = idx;
}

}