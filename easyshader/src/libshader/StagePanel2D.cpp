#include "StagePanel2D.h"
#include "StageCanvas2D.h"
#include "SelectSpritesOP.h"

#include <easysketch.h>

namespace eshader
{

StagePanel2D::StagePanel2D(wxWindow* parent, wxTopLevelWindow* frame, 
						   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_shader(NULL)
{
	m_canvas = new StageCanvas2D(this);

	d2d::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	m_edit_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, NULL, NULL, NULL, cfg);
}

}