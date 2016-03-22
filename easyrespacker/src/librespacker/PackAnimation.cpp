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
#include <ee/FileType.h>
#include <ee/ImageSprite.h>
#include <ee/trans_color.h>

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

void PackAnimation::CreateFramePart(const ee::Sprite* spr, Frame& frame)
{	
	const IPackNode* node = PackNodeFactory::Instance()->Create(spr);

	PackAnimation::Part part;
	std::string name = "";
	if (Utility::IsNameValid(spr->name)) {
		name = spr->name;
	}

	bool force_mat = AddComponent(node, name, part.comp_idx);
	PackAnimation::LoadSprTrans(spr, part.t, force_mat);

	frame.parts.push_back(part);
}

void PackAnimation::CreateClipboxFramePart(const PackClipbox* cb, Frame& frame)
{
	PackAnimation::Part part;

	AddComponent(cb, "", part.comp_idx);

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

bool PackAnimation::AddComponent(const IPackNode* node, const std::string& name, int& comp_idx)
{
	bool is_anchor = false;
	if (const PackAnchor* anchor = dynamic_cast<const PackAnchor*>(node)) {
		is_anchor = true;
		for (int i = 0, n = components.size(); i < n; ++i) {
			if (dynamic_cast<const PackAnchor*>(components[i].node) && components[i].name == name) {
				comp_idx = i;
				return true;
			}
		}
	} else {
		for (int i = 0, n = components.size(); i < n; ++i) {
			if (components[i].node == node && components[i].name == name) {
				comp_idx = i;
				return true;
			}
		}

		for (int i = 0, n = components.size(); i < n; ++i) {
			if (components[i].node->GetFilepath() == node->GetFilepath() 
				&& components[i].name == name
				&& !name.empty()) 
			{
				ee::FileType::Type type = ee::FileType::GetType(node->GetFilepath());
				if (type == ee::FileType::e_image ||
					type == ee::FileType::e_complex ||
					type == ee::FileType::e_anim) {
					comp_idx = i;
					return true;
				}
			}
		}
	}

	Component comp;
	comp.node = node;
	comp.name = name;
	components.push_back(comp);
	comp_idx = components.size() - 1;

	if (is_anchor) {
		return true;
	} else if (ee::FileType::IsType(node->GetFilepath(), ee::FileType::e_image)) {
		return false;
	} else {
		return !name.empty();
	}
}

void PackAnimation::LoadSprTrans(const ee::Sprite* spr, SpriteTrans& trans, bool force_mat)
{
	LoadSprMat(spr, trans, force_mat);
	LoadSprColor(spr, trans);
	trans.blend = int(spr->GetBlendMode());
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

	ee::Vector center = spr->GetCenter();

	bool xmir, ymir;
	spr->GetMirror(xmir, ymir);

	float sx = spr->GetScale().x,
		sy = spr->GetScale().y;
	if (xmir) {
		sx = -sx;
	}
	if (ymir) {
		sy = -sy;
	}

	float c = cos(-spr->GetAngle()),
		s = sin(-spr->GetAngle());
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
	trans.color = ee::color2int(spr->color.multi, ee::PT_ARGB);
	trans.additive = ee::color2int(spr->color.add, ee::PT_ARGB);

	trans.rmap = ee::color2int(spr->color.r, ee::PT_RGBA);
	trans.gmap = ee::color2int(spr->color.g, ee::PT_RGBA);
	trans.bmap = ee::color2int(spr->color.b, ee::PT_RGBA);	
}

bool PackAnimation::IsMatrixIdentity(const int* mat)
{
	return mat[0] == 1024 && mat[3] == 1024 
		&& mat[1] == 0 && mat[2] == 0 && mat[4] == 0 && mat[5] == 0;
}

}