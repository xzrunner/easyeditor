#include "AnchorMgr.h"
#include "SettingCfg.h"

namespace eui
{
namespace overall
{

AnchorMgr::AnchorMgr()
{
	SettingCfg* cfg = SettingCfg::Instance();
	OnViewChanged(cfg->m_view_width, cfg->m_view_height);
}

AnchorMgr::~AnchorMgr()
{
	for (int i = 0; i < 6; ++i) {
		Anchor& anchor = m_anchors[i];
		for (int i = 0, n = anchor.sprites.size(); i < n; ++i) {
			anchor.sprites[i]->Release();
		}
	}
}

void AnchorMgr::Insert(d2d::ISprite* spr)
{
	for (int i = 0; i < 6; ++i)
	{
		Anchor& anchor = m_anchors[i];
		if (d2d::Math::getDistance(anchor.pos, spr->GetPosition()) < RADIUS) {
			anchor.sprites.push_back(spr);
			spr->SetTransform(anchor.pos, 0);
			break;
		}
	}
}

void AnchorMgr::Remove(d2d::ISprite* spr)
{
	for (int i = 0; i < 6; ++i) 
	{
		Anchor& anchor = m_anchors[i];
		if (anchor.sprites.empty()) {
			continue;
		}

		std::vector<d2d::ISprite*>::iterator itr = anchor.sprites.begin();
		for ( ; itr != anchor.sprites.end(); ++itr) {
			if (*itr == spr) {
				spr->Release();
				anchor.sprites.erase(itr);
				break;
			}
		}
	}
}

void AnchorMgr::OnViewChanged(int width, int height)
{
	float hw = width * 0.5f,
		hh = height * 0.5f;
	ChangeAnchorPos(m_anchors[0], d2d::Vector(-hw, hh));
	ChangeAnchorPos(m_anchors[1], d2d::Vector(0.0f, hh));
	ChangeAnchorPos(m_anchors[2], d2d::Vector(hw, hh));
	ChangeAnchorPos(m_anchors[3], d2d::Vector(-hw, -hh));
	ChangeAnchorPos(m_anchors[4], d2d::Vector(0.0f, -hh));
	ChangeAnchorPos(m_anchors[5], d2d::Vector(hw, -hh));
}

void AnchorMgr::Draw() const
{
	for (int i = 0; i < 6; ++i) {
		const Anchor& anchor = m_anchors[i];
		d2d::PrimitiveDraw::drawCircle(anchor.pos, RADIUS, true, 2, d2d::LIGHT_GREY);
	}
}

void AnchorMgr::LoadFromFile(const Json::Value& value,
							 const std::vector<d2d::ISprite*>& sprites)
{
	for (int i = 0; i < 6; ++i) {
		int idx = 0;
		Json::Value val = value["anchor"][i][idx++];
		while (!val.isNull()) {
			std::string name = val.asString();
			d2d::ISprite* spr = NULL;
			for (int j = 0, m = sprites.size(); j < m; ++j) {
				if (sprites[j]->name == name) {
					spr = sprites[j];
					break;
				}
			}
			if (spr) {
				m_anchors[i].sprites.push_back(spr);
				spr->SetTransform(m_anchors[i].pos, 0);
			}
			val = value["anchor"][i][idx++];
		}
	}
}

void AnchorMgr::StoreToFile(Json::Value& value) const
{
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < m_anchors[i].sprites.size(); ++i) {
			value["anchor"][i][j] = m_anchors[i].sprites[j]->name;
		}
	}
}

void AnchorMgr::ChangeAnchorPos(Anchor& anchor, const d2d::Vector& pos)
{
	anchor.pos = pos;
	for (int i = 0, n = anchor.sprites.size(); i < n; ++i) {
		anchor.sprites[i]->SetTransform(pos, 0);
	}
}

}
}