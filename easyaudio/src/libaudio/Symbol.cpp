#include "Symbol.h"
#include "Sprite.h"

#include <ee/SymbolFile.h>
#include <ee/std_functor.h>
#include <ee/Blackboard.h>

#include <easycomplex.h>

#include <uniaudio/AudioData.h>
#include <uniaudio/AudioContext.h>
#include <uniaudio/DecoderFactory.h>
#include <uniaudio/Decoder.h>
#include <uniaudio/openal/Source.h>
#include <sprite2/RVG.h>
#include <sprite2/RenderParams.h>
#include <sprite2/BoundingBox.h>
#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>
#include <gum/Audio.h>

namespace eaudio
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

s2::RenderReturn Symbol::DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr) const
{
	if (!spr) {
		return s2::RENDER_NO_DATA;
	}

	if (ee::Blackboard::Instance()->visible_audio) {
		S2_MAT mt = spr->GetLocalMat() * rp.mt;
		DrawBackground(dynamic_cast<const Sprite*>(spr), mt);	
	}
	
	return s2::RENDER_OK;
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(200, 200);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

	return LoadResourcesStream();
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

bool Symbol::LoadResourcesStatic()
{
	if (ee::SymbolFile::Instance()->Type(m_filepath) == s2::SYM_COMPLEX)
	{
		std::vector<std::string> children;
		ecomplex::FileLoader::LoadChildren(m_filepath, children);

		CU_VEC<ua::AudioData*> list;
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (ee::SymbolFile::Instance()->Type(children[i]) == s2::SYM_AUDIO) {
				list.push_back(new ua::AudioData(children[i].c_str()));
			}
		}

		if (list.size() == 0) {
			return false;
		} else if (list.size() == 1) {
			ua::AudioContext* ctx = gum::Audio::Instance()->GetContext();
			SetSource(ctx->CreateSource(list[0]));
		} else {
			ua::AudioData* data = new ua::AudioData(list);
			ua::AudioContext* ctx = gum::Audio::Instance()->GetContext();
			SetSource(ctx->CreateSource(data));
			delete data;
			for_each(list.begin(), list.end(), ee::DeletePointerFunctor<ua::AudioData>());
		}
	}
	else
	{
		ua::AudioData* data = new ua::AudioData(m_filepath.c_str());
		ua::AudioContext* ctx = gum::Audio::Instance()->GetContext();
		SetSource(ctx->CreateSource(data));
		delete data;
	}

	return true;
}

bool Symbol::LoadResourcesStream()
{
	auto decoder = ua::DecoderFactory::Create(m_filepath.c_str());
	if (!decoder) {
		return false;
	}

	ua::AudioContext* ctx = gum::Audio::Instance()->GetContext();
	SetSource(ctx->CreateSource(decoder));

	return true;
}

}