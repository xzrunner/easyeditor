#include "ImageProcessOP.h"
#include "StagePanel.h"

#include <easyimage.h>

#include <ee/Sprite.h>
#include <ee/Symbol.h>

namespace eimage
{

ImageProcessOP::ImageProcessOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true, false, false)
	, m_stage(stage)
{
}

bool ImageProcessOP::OnActive()
{
	if (ee::ZoomViewOP::OnActive()) {
		return true;
	}

	if (const ee::Sprite* sprite = m_stage->GetImage())
	{
		sm::rect r = sprite->GetSymbol().GetSize();

		ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
		LanczosResamplingShader* shader = new LanczosResamplingShader;
		shader->Load();
// 		shader->SetTexelSize(1/r.Width(), 1/r.Height());
		shader->SetTexelSize(1.0f/4096, 1.0f/4096);
//		shader_mgr->SetSpriteShader(shader);
	}

	return false;
}

}