#include "LoadFromBin.h"
#include "message/messages.h"
#include "view/ViewMgr.h"
#include "dataset/DataMgr.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "view/LibraryPanel.h"

#include <ee/SymbolMgr.h>
#include <ee/FileHelper.h>
#include <ee/SpriteFactory.h>

#include <memmgr/LinearAllocator.h>
#include <s2s/NodeFactory.h>
#include <s2s/AnimSym.h>
#include <s2s/NodeSpr.h>

#include <sprite2/ILerp.h>
#include <sprite2/LerpType.h>
#include <sprite2/LerpCircle.h>
#include <sprite2/LerpSpiral.h>
#include <sprite2/LerpWiggle.h>
#include <sprite2/LerpEase.h>

namespace eanim
{

void LoadFromBin::Load(const std::string& filepath)
{
	mm::LinearAllocator alloc;

	auto sym = s2s::NodeFactory::CreateSymFromBin(alloc, filepath);
	auto anim_sym = dynamic_cast<s2s::AnimSym*>(sym);

	SetFpsSJ::Instance()->Set(anim_sym->m_fps);

	DataMgr::Instance()->GetLayers().Clear();
	ViewMgr::Instance()->library->Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);

	auto dir = ee::FileHelper::GetFileDir(filepath);
	for (int layer_idx = 0; layer_idx < anim_sym->m_layers_n; ++layer_idx)
	{
		auto& src_layer = anim_sym->m_layers[layer_idx];
		
		auto dst_layer = new Layer;
		InsertLayerSJ::Instance()->Insert(dst_layer);
		SetSelectedSJ::Instance()->Set(0, 0);
		for (int frame_idx = 0; frame_idx < src_layer.frames_n; ++frame_idx)
		{
			auto& src_frame = src_layer.frames[frame_idx];
			auto dst_frame = new KeyFrame(src_frame.index);
			dst_frame->SetLayer(dst_layer);

			LoadFrame(src_frame, dst_frame, dir);

			dst_layer->InsertKeyFrame(dst_frame);
		}
	}
		
	ViewMgr::Instance()->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());

	SetSelectedSJ::Instance()->Set(0, 0);
}

void LoadFromBin::LoadFrame(const s2s::AnimSym::Frame& src, KeyFrame* dst, const std::string& dir)
{
	dst->SetClassicTween(src.tween);

	for (int i = 0; i < src.lerps_n; ++i) {
		LoadLerp(*src.lerps[i], dst);
	}

	for (int i = 0; i < src.actors_n; ++i) 
	{
		auto& child = *src.actors[i];
		auto child_path = ee::FileHelper::GetAbsolutePath(
			dir, child.GetCommon().GetFilepath());
		auto sym = ee::SymbolMgr::Instance()->FetchSymbol(child_path);
		auto spr = ee::SpriteFactory::Instance()->Create(sym);
		spr->Load(&child);
		spr->SetVisible(true);
		dst->Insert(spr, INT_MAX);
	}
}

void LoadFromBin::LoadLerp(const s2s::AnimSym::Lerp& src, KeyFrame* dst_frame)
{
	std::unique_ptr<s2::ILerp> dst = nullptr;
	switch (src.type)
	{
	case s2s::AnimSym::LERP_CIRCLE:
		{
			float scale = src.data[0] * 0.01f;
			dst = std::make_unique<s2::LerpCircle>(scale);
		}
		break;
	case s2s::AnimSym::LERP_SPIRAL:
		{
			float begin = src.data[0] * SM_DEG_TO_RAD,
				  end   = src.data[1] * SM_DEG_TO_RAD;
			float scale = src.data[2] * SM_DEG_TO_RAD;
			dst = std::make_unique<s2::LerpSpiral>(begin, end, scale);
		}
		break;
	case s2s::AnimSym::LERP_WIGGLE:
		{
			float freq = src.data[0],
				  amp  = src.data[1];
			dst = std::make_unique<s2::LerpWiggle>(freq, amp);
		}
		break;
	case s2s::AnimSym::LERP_EASE:
		{
			int type = src.data[0];
			dst = std::make_unique<s2::LerpEase>(type);
		}
		break;
	}
	
	if (dst) {
		auto data = static_cast<s2::AnimLerp::SprData>(src.spr_data);
		dst_frame->SetLerp(data, std::move(dst));
	}
}

}