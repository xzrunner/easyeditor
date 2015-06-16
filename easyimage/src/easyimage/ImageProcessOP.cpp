#include "ImageProcessOP.h"
#include "StagePanel.h"

#include <easyimage.h>

namespace eimage
{

ImageProcessOP::ImageProcessOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true, false, false)
	, m_stage(stage)
{
}

bool ImageProcessOP::OnActive()
{
	if (d2d::ZoomViewOP::OnActive()) {
		return true;
	}

	if (const d2d::ISprite* sprite = m_stage->getImage())
	{
		d2d::Rect r = sprite->getSymbol().getSize();

		d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
		LanczosResamplingShader* shader = new LanczosResamplingShader;
		shader->Load();
// 		shader->SetTexelSize(1/r.xLength(), 1/r.yLength());
		shader->SetTexelSize(1.0f/4096, 1.0f/4096);
		shader_mgr->SetSpriteShader(shader);

		m_stage->getCanvas()->resetViewport();
	}

	return false;
}

}