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

#include <sprite2/Sprite.h>
#include <sprite2/SymType.h>

#include <wx/datetime.h>

#include <assert.h>

namespace ecomplex
{

std::shared_ptr<Sprite> GroupHelper::Group(const std::vector<ee::SprPtr>& sprs)
{
	auto sym = std::make_shared<Symbol>();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sym->Add(sprs[i]);
	}
	sym->RefreshThumbnail(ee::SYM_GROUP_TAG);

	sm::vec2 c = sym->GetBounding().Center();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->Translate(-c);
	}

	auto spr = std::make_shared<Sprite>(sym);
	spr->Translate(c);

	return spr;
}

void GroupHelper::BreakUp(const ee::SprPtr& group, std::vector<ee::SprPtr>& sprs)
{
	auto comp_sym = std::dynamic_pointer_cast<Symbol>(group->GetSymbol());
	assert(comp_sym);

	const sm::vec2& pos = group->GetPosition();
	const sm::vec2& scale = group->GetScale();
	float angle = group->GetAngle();
	auto& children = comp_sym->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		ee::SprPtr spr = std::dynamic_pointer_cast<ee::Sprite>(children[i]->Clone());

		sm::vec2 _scale = spr->GetScale();
		_scale.x *= scale.x;
		_scale.y *= scale.y;

//		float _angle = angle + spr->GetAngle();
		float _angle = angle;

		sm::vec2 _pos = group->GetLocalMat() * spr->GetPosition();

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

void GroupHelper::BuildComplex(const std::vector<ee::SprPtr>& sprs, const std::string& dir, wxWindow* wnd)
{
	if (sprs.empty()) {
		return;
	}

	auto spr = Group(sprs);
	std::string filepath = dir.c_str();
	filepath += "\\_tmp_";
	filepath += ee::StringHelper::ToString(wxDateTime::Now().GetTicks());
	filepath += "_" + ee::SymbolFile::Instance()->Tag(s2::SYM_COMPLEX) + ".json";
	auto sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
	sym->SetFilepath(filepath);

	ee::FilepathDialog dlg(wnd, sym->GetFilepath());
	if (dlg.ShowModal() == wxID_OK) {
		sym->SetFilepath(dlg.GetFilepath());
		dlg.SaveLastDir();
	}

	ee::InsertSpriteSJ::Instance()->Insert(spr);
}

void GroupHelper::BreakUpComplex(std::vector<ee::SprPtr>& sprs)
{
	if (sprs.empty()) {
		return;
	}

	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		ee::SprPtr spr = sprs[i];
		int type = ee::SymbolFile::Instance()->Type(std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath());
		if (type == s2::SYM_COMPLEX) {
			BreakUp(spr);
		}
	}
}

void GroupHelper::BuildGroup(const std::vector<ee::SprPtr>& sprs)
{
	if (sprs.empty()) {
		return;
	}

	auto spr = ecomplex::GroupHelper::Group(sprs);
	auto sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
	sym->SetFilepath(ee::SYM_GROUP_TAG);
	sym->name = "_group";
	sym->SetName("_group");
	spr->SetName("_group");

	auto move_op = std::make_shared<ee::TranslateSpriteAOP>(sprs, -spr->GetPosition());

	std::vector<ee::SprPtr> removed;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		ee::SprPtr spr = sprs[i];
		ee::RemoveSpriteSJ::Instance()->Remove(spr);
		removed.push_back(spr);
	}
	auto del_op = std::make_shared<ee::DeleteSpriteAOP>(removed);

	ee::InsertSpriteSJ::Instance()->Insert(spr);
	auto add_op = std::make_shared<ee::InsertSpriteAOP>(spr);

	auto combine = std::make_shared<ee::CombineAOP>();
	combine->Insert(move_op);
	combine->Insert(del_op);
	combine->Insert(add_op);
	ee::EditAddRecordSJ::Instance()->Add(combine);
}

void GroupHelper::BreakUpGroup(std::vector<ee::SprPtr>& sprs)
{
	if (sprs.empty()) {
		return;
	}

	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		ee::SprPtr spr = sprs[i];
		const std::string& filepath = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath();
		if (filepath == ee::SYM_GROUP_TAG) {
			BreakUp(spr);
		}
	}
}

void GroupHelper::BreakUp(const ee::SprPtr& spr)
{
	ee::SelectSpriteSJ::Instance()->Select(spr, true);

	std::vector<ee::SprPtr> children;
	BreakUp(spr, children);
	for (int j = 0, m = children.size(); j < m; ++j) {
		ee::SprPtr spr = children[j];
		ee::InsertSpriteSJ::Instance()->Insert(spr);
	}

	ee::RemoveSpriteSJ::Instance()->Remove(spr);	
}

}