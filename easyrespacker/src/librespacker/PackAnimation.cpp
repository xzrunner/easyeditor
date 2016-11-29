#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "PackAnchor.h"
#include "PackClipbox.h"
#include "Utility.h"
#include "typedef.h"

#include "AnimToLuaString.h"
#include "AnimFromLua.h"

#include "AnimToBin.h"
#include "AnimFromBin.h"

#include <ee/Sprite.h>
#include <ee/SymbolFile.h>
#include <ee/ImageSprite.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>
#include <sprite2/RenderColor.h>
#include <sprite2/RenderShader.h>
#include <sprite2/RenderCamera.h>
#include <sprite2/RenderFilter.h>
#include <gum/trans_color.h>

namespace erespacker
{

PackAnimation::PackAnimation(int id)
	: IPackNode(id)
{
}

void PackAnimation::PackToLuaString(ebuilder::CodeGenerator& gen, 
									const ee::TexturePacker& tp,
									float scale) const
{
	AnimToLuaString::Pack(this, gen);
}

void PackAnimation::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	AnimFromLua::Unpack(L, this);
}

int PackAnimation::SizeOfPackToBin() const
{
	return AnimToBin::Size(this);
}

void PackAnimation::PackToBin(uint8_t** ptr, 
							  const ee::TexturePacker& tp,
							  float scale) const
{
	AnimToBin::Pack(this, ptr);
}

int PackAnimation::SizeOfUnpackFromBin() const
{
	return AnimFromBin::Size(this);
}

void PackAnimation::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	AnimFromBin::Unpack(ptr, this);
}

bool PackAnimation::CreateFramePart(const ee::Sprite* spr, Frame& frame)
{	
	const IPackNode* node = PackNodeFactory::Instance()->Create(spr);

	PackAnimation::Part part;
	std::string name = "";
	if (Utility::IsNameValid(spr->GetName())) {
		name = spr->GetName();
	}

	bool force_mat = false;
	bool new_comp = AddComponent(node, name, part.comp_idx, force_mat);
	PackAnimation::LoadSprTrans(spr, part.t, force_mat);

	frame.parts.push_back(part);

	return new_comp;
}

void PackAnimation::CreateClipboxFramePart(const PackClipbox* cb, Frame& frame)
{
	PackAnimation::Part part;

	bool force_mat;
	AddComponent(cb, "", part.comp_idx, force_mat);

	part.t.mat[0] = part.t.mat[3] = 1024;
	part.t.mat[1] = part.t.mat[2] = 0;
	part.t.mat[4] = static_cast<int>(cb->x * SCALE);
	part.t.mat[5] =-static_cast<int>(cb->y * SCALE);

	frame.parts.push_back(part);
}

void PackAnimation::Clear()
{
	export_name.clear();
	components.clear();
	actions.clear();
	frames.clear();
}

bool PackAnimation::AddComponent(const IPackNode* node, const std::string& name, int& comp_idx, bool& force_mat)
{
	bool new_comp = false;
	bool is_anchor = false;
	if (const PackAnchor* anchor = dynamic_cast<const PackAnchor*>(node)) {
		is_anchor = true;
		for (int i = 0, n = components.size(); i < n; ++i) {
			if (dynamic_cast<const PackAnchor*>(components[i].node) && components[i].name == name) {
				comp_idx = i;
				force_mat = true;
				return new_comp;
			}
		}
	} else {
		for (int i = 0, n = components.size(); i < n; ++i) {
			if (components[i].node == node && components[i].name == name) {
				comp_idx = i;
				force_mat = true;
				return new_comp;
			}
		}

		for (int i = 0, n = components.size(); i < n; ++i) {
			if (components[i].node->GetFilepath() == node->GetFilepath() 
				&& components[i].name == name
				&& !name.empty()) 
			{
				int type = ee::SymbolFile::Instance()->Type(node->GetFilepath());
				switch (type)
				{
				case s2::SYM_IMAGE:
					comp_idx = i;
					force_mat = false;
					return new_comp;
				case s2::SYM_COMPLEX: case s2::SYM_ANIMATION: case s2::SYM_TEXTBOX: case s2::SYM_MASK: case s2::SYM_PARTICLE3D:
					comp_idx = i;
					force_mat = true;
					return new_comp;
				}
			}
		}
	}

	Component comp;
	comp.node = node;
	comp.name = name;
	components.push_back(comp);
	comp_idx = components.size() - 1;

	new_comp = true;

	if (is_anchor) {
		force_mat = true;
	} else if (ee::SymbolFile::Instance()->Type(node->GetFilepath()) == s2::SYM_IMAGE) {
		force_mat = false;
	} else {
		force_mat = !name.empty();
	}

	return new_comp;
}

void PackAnimation::LoadSprTrans(const ee::Sprite* spr, SpriteTrans& trans, bool force_mat)
{
	LoadSprMat(spr, trans, force_mat);
	LoadSprColor(spr, trans);
	trans.blend = static_cast<int>(spr->GetShader().GetBlend());
	if (spr->GetShader().GetFilter()) {
		trans.filter = static_cast<int>(spr->GetShader().GetFilter()->GetMode());
	} else {
		trans.filter = s2::FM_NULL;
	}
	trans.camera = static_cast<int>(spr->GetCamera().mode);
}

void PackAnimation::LoadSprMat(const ee::Sprite* spr, SpriteTrans& trans, bool force)
{
	if (!force && dynamic_cast<const ee::ImageSprite*>(spr)) {
		return;
	}

	float mat[6];

	// | 1  ky    | | sx       | |  c  s    | | 1       |
	// | kx  1    | |    sy    | | -s  c    | |    1    |
	// |        1 | |        1 | |        1 | | x  y  1 |
	//     skew        scale        rotate        move

	mat[1] = mat[2] = mat[4] = mat[5] = 0;
	mat[0] = mat[3] = 1;

	sm::vec2 center = spr->GetCenter();

	float sx = spr->GetScale().x,
		  sy = spr->GetScale().y;
	float c  = cos(-spr->GetAngle()),
		  s  = sin(-spr->GetAngle());
	float kx = -spr->GetShear().x,
		  ky = -spr->GetShear().y;
	mat[0] = sx*c - ky*sy*s;
	mat[1] = sx*s + ky*sy*c;
	mat[2] = kx*sx*c - sy*s;
	mat[3] = kx*sx*s + sy*c;
	mat[4] = center.x/* * m_scale*/;
	mat[5] = center.y/* * m_scale*/;

	for (size_t i = 0; i < 4; ++i) {
		trans.mat[i] = static_cast<int>(floor(mat[i] * 1024 + 0.5f));
	}
	for (size_t i = 4; i < 6; ++i) {
		trans.mat[i] = static_cast<int>(floor(mat[i] * 16 + 0.5f));
	}
	// flip y
	trans.mat[5] = -trans.mat[5];
}

void PackAnimation::LoadSprColor(const ee::Sprite* spr, SpriteTrans& trans)
{
	const s2::RenderColor& rc = spr->GetColor();

	trans.color    = gum::color2int(rc.mul, gum::ARGB);
	trans.additive = gum::color2int(rc.add, gum::ARGB);

	trans.rmap = rc.rmap.ToRGBA();
	trans.gmap = rc.gmap.ToRGBA();
	trans.bmap = rc.bmap.ToRGBA();	
}

bool PackAnimation::IsMatrixIdentity(const int* mat)
{
	return mat[0] == 1024 && mat[3] == 1024 
		&& mat[1] == 0 && mat[2] == 0 && mat[4] == 0 && mat[5] == 0;
}

}