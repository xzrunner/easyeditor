#include "AnimRecorder.h"

#include <ee/NullSprite.h>
#include <ee/NullSymbol.h>

#include <easyanim.h>

namespace eparticle3d
{

static const int MAX_FRAME = 1024;

AnimRecorder::AnimRecorder(int max_per_frame)
	: m_curr_frame(NULL)
	, m_frame_pool(MAX_FRAME)
	, m_item_pool(MAX_FRAME * max_per_frame)
{
}

AnimRecorder::~AnimRecorder()
{
	Clear();
}

void AnimRecorder::AddItem(const std::string& filepath, float x, float y, float angle, 
						   float scale, const ee::Colorf& mul_col, const ee::Colorf& add_col)
{
	while (!m_curr_frame) {
		m_curr_frame = m_frame_pool.GetPointer();
//		assert(m_curr_frame);
		if (!m_curr_frame) {
			Clear();
		} else {
			m_curr_frame->items.clear();
		}
	}

	Item* item = m_item_pool.GetPointer();
	assert(item);
	item->filepath = filepath;
	item->x = x;
	item->y = y;
	item->angle = angle;
	item->scale = scale;
	item->mul_col = mul_col;
	item->add_col = add_col;
	m_curr_frame->items.push_back(item);
}

void AnimRecorder::FinishFrame()
{
	if (m_curr_frame && !m_curr_frame->items.empty()) {
		m_frames.push_back(m_curr_frame);
		m_curr_frame = NULL;
	}
}

void AnimRecorder::Clear()
{
	m_frames.clear();
	m_curr_frame = NULL;

	m_frame_pool.Reset();
	m_item_pool.Reset();
}

void AnimRecorder::StoreToFile(const std::string& filepath) const
{
	eanim::Symbol* symbol = new eanim::Symbol;
	eanim::Symbol::Layer* layer = new eanim::Symbol::Layer;
	// symbol->name = ani->export_name;
	symbol->setFPS(30);
	// particle 60fps, while anim 30fps
	for (int i = 0, n = m_frames.size(); i*2 < n; ++i)
//	for (int i = 0, n = m_frames.size(); i < n; ++i)
	{
		eanim::Symbol::Frame* frame = new eanim::Symbol::Frame;
		frame->index = i + 1;
		frame->bClassicTween = false;
		Frame* record_frame = m_frames[i*2];
//		Frame* record_frame = m_frames[i];
		for (int j = 0, m = record_frame->items.size(); j < m; ++j)
		{
			Item* item = record_frame->items[j];
			ee::Sprite* spr = new ee::NullSprite(
				new ee::NullSymbol(item->filepath));

			spr->SetTransform(ee::Vector(item->x, item->y), item->angle);
			spr->SetScale(ee::Vector(item->scale, item->scale));
			spr->rp->color.multi = item->mul_col;

			frame->sprites.push_back(spr);
		}
		layer->frames.push_back(frame);
	}
	symbol->m_layers.push_back(layer);

	eanim::FileSaver::Store(filepath.c_str(), *symbol);
	delete symbol;
}

//////////////////////////////////////////////////////////////////////////
// AnimRecorder::Frame
//////////////////////////////////////////////////////////////////////////

AnimRecorder::Frame::~Frame()
{
//	Clear();

	items.clear();
}

// void AnimRecorder::Frame::Clear()
// {
// 	items.clear();
// }

}