#include "Recorder.h"

#include <easyanim.h>

namespace eparticle3d
{

static const int MAX_FRAME = 1024;

Recorder::Recorder(int max_per_frame)
	: m_curr_frame(NULL)
	, m_frame_pool(MAX_FRAME)
	, m_item_pool(MAX_FRAME * max_per_frame)
{
}

Recorder::~Recorder()
{
	Clear();
}

void Recorder::AddItem(const std::string& filepath, float x, float y, 
					   float angle, float scale, const d2d::Colorf& col)
{
	while (!m_curr_frame) {
		m_curr_frame = m_frame_pool.GetPointer();
		if (!m_curr_frame) {
			Clear();
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

void Recorder::FinishFrame()
{
	if (m_curr_frame && !m_curr_frame->items.empty()) {
		m_frames.push_back(m_curr_frame);
		m_curr_frame = NULL;
	}
}

void Recorder::Clear()
{
	m_frames.clear();
	m_curr_frame = NULL;

	m_frame_pool.Reset();
	m_item_pool.Reset();
}

void Recorder::StoreToAnimFile(const std::string& filepath) const
{
	anim::Symbol* symbol = new anim::Symbol;
	anim::Symbol::Layer* layer = new anim::Symbol::Layer;
	// symbol->name = ani->export_name;
	symbol->m_fps = 30;
	for (int i = 0, n = m_frames.size(); i < n; ++i)
	{
		anim::Symbol::Frame* frame = new anim::Symbol::Frame;
		frame->index = i + 1;
		frame->bClassicTween = false;
		for (int j = 0, m = m_frames[i]->items.size(); j < m; ++j)
		{
			Item* item = m_frames[i]->items[j];
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

	anim::FileSaver::store(filepath.c_str(), *symbol);
	delete symbol;
}

//////////////////////////////////////////////////////////////////////////
// Recorder::Frame
//////////////////////////////////////////////////////////////////////////

Recorder::Frame::~Frame()
{
//	Clear();

	items.clear();
}

// void Recorder::Frame::Clear()
// {
// 	items.clear();
// }

}