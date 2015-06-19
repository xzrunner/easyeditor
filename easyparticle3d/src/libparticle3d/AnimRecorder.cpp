#include "AnimRecorder.h"

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

void AnimRecorder::AddItem(const std::string& filepath, float x, float y, 
					   float angle, float scale, const d2d::Colorf& col)
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
	item->col = col;
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
	libanim::Symbol* symbol = new libanim::Symbol;
	libanim::Symbol::Layer* layer = new libanim::Symbol::Layer;
	// symbol->name = ani->export_name;
	symbol->setFPS(30);
	// particle 60fps, while anim 30fps
	for (int i = 0, n = m_frames.size(); i*2 < n; ++i)
//	for (int i = 0, n = m_frames.size(); i < n; ++i)
	{
		libanim::Symbol::Frame* frame = new libanim::Symbol::Frame;
		frame->index = i + 1;
		frame->bClassicTween = false;
		Frame* record_frame = m_frames[i*2];
//		Frame* record_frame = m_frames[i];
		for (int j = 0, m = record_frame->items.size(); j < m; ++j)
		{
			Item* item = record_frame->items[j];
			d2d::ISprite* sprite = new d2d::NullSprite(
				new d2d::NullSymbol(item->filepath));

			sprite->setTransform(d2d::Vector(item->x, item->y), item->angle);
			sprite->setScale(item->scale, item->scale);
			sprite->multiCol = item->col;

			frame->sprites.push_back(sprite);
		}
		layer->frames.push_back(frame);
	}
	symbol->m_layers.push_back(layer);

	libanim::FileSaver::store(filepath.c_str(), *symbol);
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