#include "AnchorMgr.h"
#include "QueryWindowViewSizeSJ.h"

#include <ee/sprite_msg.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>
#include <ee/subject_id.h>
#include <ee/Math2D.h>
#include <ee/Symbol.h>
#include <ee/FileHelper.h>
#include <ee/Visitor.h>
#include <ee/ObjectVector.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>

#include <algorithm>

namespace eui
{
namespace window
{

AnchorMgr::AnchorMgr()
{
	int w, h;
	QueryWindowViewSizeSJ::Instance()->Query(w, h);
	OnViewChanged(w, h);
}

AnchorMgr::~AnchorMgr()
{
	Clear();
}

void AnchorMgr::LoadFromFile(const Json::Value& value,
							 const std::vector<ee::Sprite*>& sprites)
{
	if (value["anchor"].size() == 6) {
		for (int i = 0; i < 6; ++i) {
			int dst = i < 3 ? i : i + 3;
			LoadAnchorData(sprites, value["anchor"][i], m_anchors[dst]);
		}
	} else {
		for (int i = 0; i < ANCHOR_COUNT; ++i) {
			LoadAnchorData(sprites, value["anchor"][i], m_anchors[i]);
		}
	}
}

void AnchorMgr::StoreToFile(Json::Value& value, const std::string& dir) const
{
	for (int i = 0; i < ANCHOR_COUNT; ++i) {
		Json::Value a_val;
		for (int j = 0; j < m_anchors[i].sprites.size(); ++j) {
			ee::Sprite* spr = m_anchors[i].sprites[j];

			std::string filepath = ee::FileHelper::GetRelativePath(dir,
				spr->GetSymbol().GetFilepath());

			Json::Value spr_val_anchor;
			spr_val_anchor["name"] = spr->GetName();
			spr_val_anchor["filepath"] = filepath;
			a_val[j] = spr_val_anchor;
		}
		value["anchor"][i] = a_val;
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = m_sprites[i];
		std::string filepath = ee::FileHelper::GetRelativePath(dir,
			spr->GetSymbol().GetFilepath());

		Json::Value spr_val;
		spr_val["filepath"] = filepath;
		spr->Store(spr_val);
		value["sprite"][i] = spr_val;
	}
}

void AnchorMgr::OnViewChanged(int width, int height)
{
	float hw = width * 0.5f,
		  hh = height * 0.5f;

	ChangeAnchorPos(m_anchors[0], sm::vec2(-hw, hh));
	ChangeAnchorPos(m_anchors[1], sm::vec2(0.0f, hh));
	ChangeAnchorPos(m_anchors[2], sm::vec2(hw, hh));

	ChangeAnchorPos(m_anchors[3], sm::vec2(-hw, 0.0f));
	ChangeAnchorPos(m_anchors[4], sm::vec2(0.0f, 0.0f));
	ChangeAnchorPos(m_anchors[5], sm::vec2(hw, 0.0f));

	ChangeAnchorPos(m_anchors[6], sm::vec2(-hw, -hh));
	ChangeAnchorPos(m_anchors[7], sm::vec2(0.0f, -hh));
	ChangeAnchorPos(m_anchors[8], sm::vec2(hw, -hh));
}

void AnchorMgr::OnSprPosChanged(ee::Sprite* spr)
{
	Move(spr);
}

void AnchorMgr::DrawSprites(const s2::RenderParams& params) const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ee::SpriteRenderer::Draw(m_sprites[i], params);
	}
}

void AnchorMgr::DrawNodes(const s2::RenderParams& params) const
{
	s2::RVG::SetColor(ee::LIGHT_GREY);
	for (int i = 0; i < ANCHOR_COUNT; ++i) {
		const Anchor& anchor = m_anchors[i];
		s2::RVG::Circle(anchor.pos, RADIUS, true);
	}
}

void AnchorMgr::Traverse(ee::Visitor& visitor)
{
	ee::ObjectVector<ee::Sprite>::Traverse(m_sprites, visitor, ee::DT_ALL);
}

void AnchorMgr::Remove(ee::Sprite* spr)
{
	for (int i = 0; i < ANCHOR_COUNT; ++i) 
	{
		Anchor& anchor = m_anchors[i];
		if (anchor.sprites.empty()) {
			continue;
		}

		std::vector<ee::Sprite*>::iterator itr = anchor.sprites.begin();
		for ( ; itr != anchor.sprites.end(); ++itr) {
			if (*itr == spr) {
				spr->Release();
				anchor.sprites.erase(itr);
				break;
			}
		}
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (spr == m_sprites[i]) {
			spr->Release();
			m_sprites.erase(m_sprites.begin() + i);
			break;
		}
	}
}

void AnchorMgr::Insert(ee::Sprite* spr)
{
	Anchor* anchor = GetNearestAnchor(spr->GetPosition());

	spr->SetTransform(anchor->pos, 0);

	spr->Retain();
	anchor->sprites.push_back(spr);

	spr->Retain();
	m_sprites.push_back(spr);
}

void AnchorMgr::Clear()
{
	for (int i = 0; i < ANCHOR_COUNT; ++i) {
		Anchor& anchor = m_anchors[i];
		for_each(anchor.sprites.begin(), anchor.sprites.end(), 
			ee::ReleaseObjectFunctor<ee::Sprite>());
		anchor.sprites.clear();
	}

	for_each(m_sprites.begin(), m_sprites.end(), 
		ee::ReleaseObjectFunctor<ee::Sprite>());
	m_sprites.clear();
}

void AnchorMgr::ResetOrder(ee::Sprite* spr, bool up)
{
	ee::ObjectVector<ee::Sprite>::ResetOrder(m_sprites, spr, up);
}

void AnchorMgr::ResetOrderMost(ee::Sprite* spr, bool up)
{
	ee::ObjectVector<ee::Sprite>::ResetOrderMost(m_sprites, spr, up);
}

AnchorMgr::Anchor* AnchorMgr::GetNearestAnchor(const sm::vec2& pos)
{
	float nearest = FLT_MAX;
	int selected = -1;
	for (int i = 0; i < ANCHOR_COUNT; ++i)
	{
		Anchor& anchor = m_anchors[i];
		float dis = ee::Math2D::GetDistance(anchor.pos, pos);
		if (dis < nearest) 
		{
			nearest = dis;
			selected = i;
		}
	}
	return (selected == -1) ? NULL : &m_anchors[selected];
}

void AnchorMgr::Move(ee::Sprite* spr)
{
	Anchor* anchor = GetNearestAnchor(spr->GetPosition());
	spr->SetTransform(anchor->pos, 0);
}

void AnchorMgr::ChangeAnchorPos(Anchor& anchor, const sm::vec2& pos)
{
	anchor.pos = pos;
	for (int i = 0, n = anchor.sprites.size(); i < n; ++i) {
		anchor.sprites[i]->SetTransform(pos, 0);
	}
}

void AnchorMgr::LoadAnchorData(const std::vector<ee::Sprite*>& sprites,
							   const Json::Value& value, Anchor& anchor)
{
	int idx = 0;
	Json::Value val = value[idx++];
	while (!val.isNull()) {
		std::string name = val.asString();
		ee::Sprite* spr = NULL;
		for (int j = 0, m = sprites.size(); j < m; ++j) {
			if (sprites[j]->GetName() == name) {
				spr = sprites[j];
				break;
			}
		}
		if (spr) {
			anchor.sprites.push_back(spr);
			spr->SetTransform(anchor.pos, 0);
		}

		val = value[idx++];
	}
}

}
}