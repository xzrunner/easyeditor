#include "s2loader/SpriteFactory.h"
#include "s2loader/SymbolFactory.h"

#include "s2loader/SpriteIO.h"
#include "s2loader/Scale9SprLoader.h"
#include "s2loader/IconSprLoader.h"
#include "s2loader/TextboxSprLoader.h"
#include "s2loader/ComplexSprLoader.h"
#include "s2loader/AnimSprLoader.h"
#include "s2loader/Anim2SprLoader.h"
#include "s2loader/P3dSprLoader.h"
#include "s2loader/P2dSprLoader.h"
#include "s2loader/MeshSprLoader.h"
#include "s2loader/AudioSprLoader.h"

#include <simp/simp_types.h>
#include <simp/NodeFactory.h>
#include <simp/NodeScale9Spr.h>
#include <simp/NodeIconSpr.h>
#include <simp/NodeTextureSpr.h>
#include <simp/NodeComplexSpr.h>
#include <simp/NodeAnimationSpr.h>
#include <simp/NodeParticle3dSpr.h>
#include <simp/NodeParticle2dSpr.h>
#include <simp/NodeShapeSpr.h>
#include <simp/NodeMeshSpr.h>
#include <simp/NodeMaskSpr.h>
#include <simp/NodeTrailSpr.h>
#include <simp/NodeAnim2Spr.h>
#include <simp/NodeAudioSpr.h>
#include <simp/AudioIDMgr.h>

#include <sprite2/Symbol.h>
#include <sprite2/AnchorSprite.h>
#include <sprite2/ImageSprite.h>
#include <sprite2/Scale9Sprite.h>
#include <sprite2/IconSprite.h>
#include <sprite2/TextureSprite.h>
#include <sprite2/TextboxSprite.h>
#include <sprite2/ComplexSprite.h>
#include <sprite2/AnimSprite.h>
#include <sprite2/Anim2Sprite.h>
#include <sprite2/Particle3dSprite.h>
#include <sprite2/Particle2dSprite.h>
#include <sprite2/ShapeSprite.h>
#include <sprite2/MeshSprite.h>
#include <sprite2/MaskSprite.h>
#include <sprite2/TrailSprite.h>
#include <sprite2/SkeletonSprite.h>
#include <sprite2/AudioSprite.h>
#include <sprite2/SymType.h>
#include <sprite2/ResetActorFlagVisitor.h>
#include <sprite2/CreateActorsVisitor.h>
#include <sprite2/SprVisitorParams.h>
#include <sprite2/UpdateParams.h>

#include <gum/FilepathHelper.h>
#include <gum/SymbolPool.h>

#include <fault.h>
#include <logger.h>

#include <assert.h>

namespace s2loader
{

CU_SINGLETON_DEFINITION(SpriteFactory);

SpriteFactory::SpriteFactory()
{
}

s2::SprPtr SpriteFactory::Create(const s2::SymPtr& sym, uint32_t id, bool create_actors) const
{
	if (!sym) {
		return nullptr;
	}

	s2::SprPtr spr = nullptr;
	switch (sym->Type())
	{
	case s2::SYM_IMAGE:
		spr = CU_MAKE_SHARED<s2::ImageSprite>(sym, id);
		break;
	case s2::SYM_SCALE9:
		spr = CU_MAKE_SHARED<s2::Scale9Sprite>(sym, id);
		break;
	case s2::SYM_ICON:
		spr = CU_MAKE_SHARED<s2::IconSprite>(sym, id);
		break;
	case s2::SYM_TEXTURE:
		spr = CU_MAKE_SHARED<s2::TextureSprite>(sym, id);
		break;
	case s2::SYM_TEXTBOX:
		spr = CU_MAKE_SHARED<s2::TextboxSprite>(sym, id);
		break;
	case s2::SYM_COMPLEX:
		spr = CU_MAKE_SHARED<s2::ComplexSprite>(sym, id);
		break;
	case s2::SYM_ANIMATION:
		spr = CU_MAKE_SHARED<s2::AnimSprite>(sym, id);
		break;
	case s2::SYM_ANIM2:
		spr = CU_MAKE_SHARED<s2::Anim2Sprite>(sym, id);
		break;
	case s2::SYM_PARTICLE3D:
		spr = CU_MAKE_SHARED<s2::Particle3dSprite>(sym, id);
		break;
	case s2::SYM_PARTICLE2D:
		spr = CU_MAKE_SHARED<s2::Particle2dSprite>(sym, id);
		break;
	case s2::SYM_SHAPE:
		spr = CU_MAKE_SHARED<s2::ShapeSprite>(sym, id);
		break;
	case s2::SYM_MESH:
		spr = CU_MAKE_SHARED<s2::MeshSprite>(sym, id);
		break;
	case s2::SYM_MASK:
		spr = CU_MAKE_SHARED<s2::MaskSprite>(sym, id);
		break;
	case s2::SYM_TRAIL:
		spr = CU_MAKE_SHARED<s2::TrailSprite>(sym, id);
		break;
	case s2::SYM_SKELETON:
		spr = CU_MAKE_SHARED<s2::SkeletonSprite>(sym, id);
		break;
	case s2::SYM_AUDIO:
		spr = CU_MAKE_SHARED<s2::AudioSprite>(sym, id);
		break;
	default:
		LOGW("Create spr fail: unknown sym type %d", sym->Type());
	}
	if (create_actors && spr) {
		CreateSprActors(spr);
	}
	//if (spr) {
	//	spr->OnMessage(s2::UpdateParams(), s2::MSG_START);
	//}
	return spr;
}

s2::SprPtr SpriteFactory::Create(const CU_STR& filepath) const
{
	auto sym = gum::SymbolPool::Instance()->Fetch(filepath);
	if (!sym) {
		return nullptr;
	} else {
		auto spr = Create(sym, -1, false);
		if (spr) {
			CreateSprActors(spr);
//			spr->OnMessage(s2::UpdateParams(), s2::MSG_START);
		}
		return spr;
	}
}

s2::SprPtr SpriteFactory::Create(const Json::Value& val, const CU_STR& dir) const
{
	s2::SprPtr spr = nullptr;

	CU_STR filepath = val["filepath"].asString().c_str();
	filepath = gum::FilepathHelper::Absolute(dir, filepath);
	spr = Create(filepath);
	if (!spr) {
		return nullptr;
	}

	SpriteIO io(true, true);
	io.Load(val, spr, dir);

	switch (spr->GetSymbol()->Type())
	{
	case s2::SYM_IMAGE:
		{
		}
		break;
	case s2::SYM_SCALE9:
		{
			Scale9SprLoader loader(*S2_VI_PTR_DOWN_CAST<s2::Scale9Sprite>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case s2::SYM_ICON:
		{
			IconSprLoader loader(*S2_VI_PTR_DOWN_CAST<s2::IconSprite>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case s2::SYM_TEXTURE:
		{
		}
		break;
	case s2::SYM_TEXTBOX:
		{
			TextboxSprLoader loader(*S2_VI_PTR_DOWN_CAST<s2::TextboxSprite>(spr));
			loader.LoadJson(val);
		}
		break;
	case s2::SYM_COMPLEX:
		{
			ComplexSprLoader loader(*S2_VI_PTR_DOWN_CAST<s2::ComplexSprite>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case s2::SYM_ANIMATION:
		{
			AnimSprLoader loader(*S2_VI_PTR_DOWN_CAST<s2::AnimSprite>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case s2::SYM_PARTICLE3D:
		{
			P3dSprLoader loader(*S2_VI_PTR_DOWN_CAST<s2::Particle3dSprite>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case s2::SYM_PARTICLE2D:
		{
			P2dSprLoader loader(*S2_VI_PTR_DOWN_CAST<s2::Particle2dSprite>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case s2::SYM_MESH:
		{
			MeshSprLoader loader(*S2_VI_PTR_DOWN_CAST<s2::MeshSprite>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case s2::SYM_MASK:
		{
		}
		break;
	case s2::SYM_TRAIL:
		{
		}
		break;
	case s2::SYM_SKELETON:
		{
		}
		break;
	default:
		LOGW("Create spr fail: unknown sym type %d", spr->GetSymbol()->Type());
	}

	if (spr) {
		CreateSprActors(spr);
//		spr->OnMessage(s2::UpdateParams(), s2::MSG_START);
	} else {
		LOGW("Create spr fail: filepath %s", filepath.c_str());
	}

	return spr;
}

s2::SprPtr SpriteFactory::Create(uint32_t id)
{
	if (id == 0xffffffff) {
		return std::make_shared<s2::AnchorSprite>(SymbolFactory::Instance()->Create(id));
	}

	s2::SprPtr spr = nullptr;
	int type = simp::TYPE_INVALID;
	const void* data = simp::NodeFactory::Instance()->Create(id, &type);
	if (!data && type == simp::TYPE_INVALID)
	{
		int pkg_id = simp::NodeID::GetPkgID(id);
		int node_id = simp::NodeID::GetNodeID(id);
		LOGW("Create spr fail: id %d, pkg %d, node %d\n", id, pkg_id, node_id);
		return nullptr;
	}
	switch (type)
	{
	case simp::TYPE_IMAGE:
		{
			auto sym = gum::SymbolPool::Instance()->Fetch(id);
			if (sym) {
				spr = CU_MAKE_SHARED<s2::ImageSprite>(sym, id);
			}
		}
		break;
	case simp::TYPE_SCALE9_SPR:
		{
			const simp::NodeScale9Spr* node = (const simp::NodeScale9Spr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym)
			{
				auto s9_spr = CU_MAKE_SHARED<s2::Scale9Sprite>(sym, id);
				if (s9_spr)
				{
					Scale9SprLoader loader(*s9_spr);
					loader.LoadBin(node);
					spr = s9_spr;
				}
			}
		}
		break;
	case simp::TYPE_ICON_SPR:
		{
			const simp::NodeIconSpr* node = (const simp::NodeIconSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym)
			{
				auto icon_spr = CU_MAKE_SHARED<s2::IconSprite>(sym, id);
				if (icon_spr)
				{
					IconSprLoader loader(*icon_spr);
					loader.LoadBin(node);
					spr = icon_spr;
				}
			}
		}
		break;
	case simp::TYPE_TEXTURE_SPR:
		{
			const simp::NodeTextureSpr* node = (const simp::NodeTextureSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym)
			{
				auto tex_spr = CU_MAKE_SHARED<s2::TextureSprite>(sym, id);
				spr = tex_spr;
			}
		}
		break;
	case simp::TYPE_LABEL:
		{
			const simp::NodeLabel* node = (const simp::NodeLabel*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(id);
			if (sym)
			{
				auto tb_spr = S2_VI_PTR_DOWN_CAST<s2::TextboxSprite>(SpriteFactory::Instance()->Create(sym, id, true));
				if (tb_spr)
				{
					TextboxSprLoader loader(*tb_spr);
					loader.LoadBin(node);
					spr = tb_spr;
				}
			}
		}
		break;
	case simp::TYPE_COMPLEX_SPR:
		{
			const simp::NodeComplexSpr* node = (const simp::NodeComplexSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym) 
			{
				auto comp_spr = S2_VI_PTR_DOWN_CAST<s2::ComplexSprite>(SpriteFactory::Instance()->Create(sym, id, true));
				if (comp_spr) {
					ComplexSprLoader loader(*comp_spr);
					loader.LoadBin(node);
					spr = comp_spr;
				}
			}
		}
		break;
	case simp::TYPE_ANIM_SPR:
		{
			const simp::NodeAnimationSpr* node = (const simp::NodeAnimationSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym)
			{
				auto anim_spr = S2_VI_PTR_DOWN_CAST<s2::AnimSprite>(SpriteFactory::Instance()->Create(sym, id, true));
				if (anim_spr)
				{
					AnimSprLoader loader(*anim_spr);
					loader.LoadBin(node);
					spr = anim_spr;
				}
			}
		}
		break;
	case simp::TYPE_ANIM2_SPR:
		{
			const simp::NodeAnim2Spr* node = (const simp::NodeAnim2Spr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym)
			{
				auto anim2_spr = S2_VI_PTR_DOWN_CAST<s2::Anim2Sprite>(SpriteFactory::Instance()->Create(sym, id, true));
				if (anim2_spr)
				{
					Anim2SprLoader loader(*anim2_spr);
					loader.LoadBin(node);
					spr = anim2_spr;
				}
			}
		}
		break;
	case simp::TYPE_P3D_SPR:
		{
			const simp::NodeParticle3dSpr* node = (const simp::NodeParticle3dSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym)
			{
				auto p3d_spr = S2_VI_PTR_DOWN_CAST<s2::Particle3dSprite>(SpriteFactory::Instance()->Create(sym, id, true));
				if (p3d_spr)
				{
					P3dSprLoader loader(*p3d_spr);
					loader.LoadBin(node);
					spr = p3d_spr;
				}
			}
		}
		break;
	case simp::TYPE_P2D_SPR:
		{
			const simp::NodeParticle2dSpr* node = (const simp::NodeParticle2dSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym)
			{
				auto p2d_spr = S2_VI_PTR_DOWN_CAST<s2::Particle2dSprite>(SpriteFactory::Instance()->Create(sym, id, true));
				if (p2d_spr)
				{
					P2dSprLoader loader(*p2d_spr);
					loader.LoadBin(node);
					spr = p2d_spr;
				}
			}
		}
		break;
	case simp::TYPE_SHAPE_SPR:
		{
			const simp::NodeShapeSpr* node = (const simp::NodeShapeSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym) {
				spr = SpriteFactory::Instance()->Create(sym, id, true);
			}
		}
		break;
	case simp::TYPE_MESH_SPR:
		{
			const simp::NodeMeshSpr* node = (const simp::NodeMeshSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->mesh_id);
			if (sym)
			{
				auto mesh_spr = S2_VI_PTR_DOWN_CAST<s2::MeshSprite>(SpriteFactory::Instance()->Create(sym, id, true));
				if (mesh_spr)
				{
					MeshSprLoader loader(*mesh_spr);
					loader.LoadBin(node);
					spr = mesh_spr;
				}
			}
		}
		break;
	case simp::TYPE_MASK_SPR:
		{
			const simp::NodeMaskSpr* node = (const simp::NodeMaskSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym) {
				spr = SpriteFactory::Instance()->Create(sym, id, true);
			}
		}
		break;
	case simp::TYPE_TRAIL_SPR:
		{
			const simp::NodeTrailSpr* node = (const simp::NodeTrailSpr*)data;
			auto sym = gum::SymbolPool::Instance()->Fetch(node->sym);
			if (sym) {
				spr = SpriteFactory::Instance()->Create(sym, id, true);
			}
		}
		break;
	case simp::TYPE_AUDIO_SPR:
		{
			const simp::NodeAudioSpr* node = (const simp::NodeAudioSpr*)data;
			int audio_id = simp::NodeID::GetNodeID(node->sym);
			auto filepath = simp::AudioIDMgr::Instance()->QueryAudioPath(audio_id);
			if (!filepath.empty())
			{
				auto audio_spr = S2_VI_PTR_DOWN_CAST<s2::AudioSprite>(SpriteFactory::Instance()->Create(filepath));
				AudioSprLoader loader(*audio_spr);
				loader.LoadBin(filepath, node);
				spr = audio_spr;
			}
		}
		break;
	case simp::TYPE_SCALE9: case simp::TYPE_ICON: case simp::TYPE_TEXTURE: case simp::TYPE_COMPLEX: case simp::TYPE_ANIMATION:
	case simp::TYPE_PARTICLE3D: case simp::TYPE_PARTICLE2D: case simp::TYPE_SHAPE: case simp::TYPE_MESH: case simp::TYPE_MASK:
	case simp::TYPE_TRAIL: case simp::TYPE_AUDIO:
		spr = CreateFromSym(id, false);
		break;
	default:
		LOGW("Create spr fail: unknown type %d", type);
	}

	if (spr) {
		CreateSprActors(spr);
//		spr->OnMessage(s2::UpdateParams(), s2::MSG_START);
	} else {
		LOGW("Create spr fail: id %u", id);
	}

	return spr;
}

s2::SprPtr SpriteFactory::CreateFromSym(uint32_t id, bool create_actors)
{
	auto sym = gum::SymbolPool::Instance()->Fetch(id);
	if (!sym) {
		LOGW("Create spr fail: err sym %u", id);
		return nullptr;
	} else {
		return Create(sym, -1, create_actors);
	}
}

void SpriteFactory::CreateSprActors(const s2::SprPtr& spr)
{
	s2::ResetActorFlagVisitor v0;
	s2::SprVisitorParams vp0;
	spr->Traverse(v0, vp0);

	s2::CreateActorsVisitor v1;
	s2::SprVisitorParams vp1;
	spr->Traverse(v1, vp1);
}

}