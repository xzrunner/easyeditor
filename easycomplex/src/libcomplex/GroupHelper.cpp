#include "GroupHelper.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/Math2D.h>
#include <ee/SpriteFactory.h>
#include <ee/FetchAllVisitor.h>
#include <ee/StringHelper.h>
#include <ee/SymbolFile.h>
#include <ee/FilepathDialog.h>
#include <ee/sprite_msg.h>
#include <ee/SymbolType.h>
#include <ee/AtomicOP.h>
#include <ee/TranslateSpriteAOP.h>
#include <ee/CombineAOP.h>
#include <ee/DeleteSpriteAOP.h>
#include <ee/InsertSpriteAOP.h>
#include <ee/panel_msg.h>
#include <ee/FileHelper.h>

#include <sprite2/S2_Sprite.h>
#include <sprite2/SymType.h>

#include <wx/datetime.h>

#include <assert.h>

namespace ecomplex
{

Sprite* GroupHelper::Group(const std::vector<ee::Sprite*>& sprs)
{
	Symbol* sym = new Symbol();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sym->Add(sprs[i]);
	}
	sym->RefreshThumbnail(ee::SYM_GROUP_TAG);

	sm::vec2 c = sym->GetBounding().Center();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->Translate(-c);
	}

	Sprite* spr = new Sprite(sym);
	spr->Translate(c);
	ee::SpriteFactory::Instance()->Insert(spr);

	sym->RemoveReference();

	return spr;
}

void GroupHelper::BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprs)
{
	Symbol* comp = dynamic_cast<Symbol*>(group->GetSymbol());
	assert(comp);

	const sm::vec2& pos = group->GetPosition();
	const sm::vec2& scale = group->GetScale();
	float angle = group->GetAngle();
	const std::vector<s2::Sprite*>& children = comp->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		ee::Sprite* spr = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)children[i])->Clone());

		sm::vec2 _scale = spr->GetScale();
		_scale.x *= scale.x;
		_scale.y *= scale.y;

//		float _angle = angle + spr->GetAngle();
		float _angle = angle;

		sm::vec2 _pos = group->GetTransMatrix() * spr->GetPosition();

		const sm::vec2& gs = group->GetScale();
		if (gs.x < 0 && gs.x >= 0 ||
			gs.y >= 0 && gs.y < 0) {
		} else {
			_angle += spr->GetAngle();
		}

		spr->SetScale(_scale);
		spr->SetAngle(_angle);
		spr->SetPosition(_pos);

		sprs.push_back(spr);
	}
}

void GroupHelper::BuildComplex(const std::vector<ee::Sprite*>& sprs, const std::string& dir, wxWindow* wnd)
{
	if (sprs.empty()) {
		return;
	}

	Sprite* spr = Group(sprs);
	std::string filepath = dir;
	filepath += "\\_tmp_";
	filepath += ee::StringHelper::ToString(wxDateTime::Now().GetTicks());
	filepath += "_" + ee::SymbolFile::Instance()->Tag(s2::SYM_COMPLEX) + ".json";
	Symbol* sym = dynamic_cast<Symbol*>(spr->GetSymbol());
	sym->SetFilepath(filepath);

	ee::FilepathDialog dlg(wnd, sym->GetFilepath());
	if (dlg.ShowModal() == wxID_OK) {
		sym->SetFilepath(dlg.GetFilepath());
		dlg.SaveLastDir();
	}

	ee::InsertSpriteSJ::Instance()->Insert(spr);
}

void GroupHelper::BreakUpComplex(std::vector<ee::Sprite*>& sprs)
{
	if (sprs.empty()) {
		return;
	}

	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		ee::Sprite* spr = sprs[i];
		int type = ee::SymbolFile::Instance()->Type(dynamic_cast<ee::Symbol*>(spr->GetSymbol())->GetFilepath());
		if (type == s2::SYM_COMPLEX) {
			BreakUp(spr);
		}
	}
}

void GroupHelper::BuildGroup(const std::vector<ee::Sprite*>& sprs)
{
	if (sprs.empty()) {
		return;
	}

	ecomplex::Sprite* spr = ecomplex::GroupHelper::Group(sprs);
	ee::Symbol* sym = dynamic_cast<ee::Symbol*>(spr->GetSymbol());
	sym->SetFilepath(ee::SYM_GROUP_TAG);
	sym->name = "_group";
	sym->SetName("_group");
	spr->SetName("_group");

	ee::AtomicOP* move_op = new ee::TranslateSpriteAOP(sprs, -spr->GetPosition());

	std::vector<ee::Sprite*> removed;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		ee::Sprite* spr = sprs[i];
		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		removed.push_back(spr);
	}
	ee::AtomicOP* del_op = new ee::DeleteSpriteAOP(removed);

	ee::InsertSpriteSJ::Instance()->Insert(spr);
	ee::AtomicOP* add_op = new ee::InsertSpriteAOP(spr);
	spr->RemoveReference();

	ee::CombineAOP* combine = new ee::CombineAOP;
	combine->Insert(move_op);
	combine->Insert(del_op);
	combine->Insert(add_op);
	ee::EditAddRecordSJ::Instance()->Add(combine);
}

void GroupHelper::BreakUpGroup(std::vector<ee::Sprite*>& sprs)
{
	if (sprs.empty()) {
		return;
	}

	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		ee::Sprite* spr = sprs[i];
		const std::string& filepath = dynamic_cast<ee::Symbol*>(spr->GetSymbol())->GetFilepath();
		if (filepath == ee::SYM_GROUP_TAG) {
			BreakUp(spr);
		}
	}
}

void GroupHelper::BreakUp(ee::Sprite* spr)
{
	ee::SelectSpriteSJ::Instance()->Select(spr, true);

	std::vector<ee::Sprite*> children;
	BreakUp(spr, children);
	for (int j = 0, m = children.size(); j < m; ++j) {
		ee::Sprite* spr = children[j];
		ee::InsertSpriteSJ::Instance()->Insert(spr);
		spr->RemoveReference();
	}

	ee::RemoveSpriteSJ::Instance()->Remove(spr);	
}

}