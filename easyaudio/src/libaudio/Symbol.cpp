#include "Symbol.h"
#include "Sprite.h"

#include <uniaudio/AudioData.h>
#include <uniaudio/openal/Source.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/RenderParams.h>
#include <sprite2/BoundingBox.h>
#include <gum/FilepathHelper.h>

namespace eaudio
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

s2::RenderReturn Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	if (!spr) {
		return s2::RENDER_NO_DATA;
	}

	S2_MAT mt = spr->GetLocalMat() * params.mt;
	DrawBackground(dynamic_cast<const Sprite*>(spr), mt);

	return s2::RENDER_OK;
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(200, 200);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	ua::AudioData* data = new ua::AudioData(m_filepath);
	SetSource(new ua::openal::Source(data));
	delete data;

	return true;
}

void Symbol::DrawBackground(const Sprite* spr, const S2_MAT& mt) const
{
	if (!spr) {
		return;
	}

	s2::RVG::SetColor(s2::Color(179, 179, 0, 179));

	sm::rect r = spr->GetBounding()->GetSize();
	s2::RVG::Rect(r, true);

// 	sm::vec2 min(r.xmin, r.ymin), max(r.xmax, r.ymax);
// 	min = mt * min;
// 	max = mt * max;
// 
// 	s2::RVG::Rect(min, max, true);
}

}