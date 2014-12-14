#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"

#include <easysketch.h>

namespace eshader
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_shader(NULL)
{
	d2d::GLCanvas* canvas2d = new StageCanvas(this);
//	d2d::GLCanvas* canvas3d = new libsketch::StageCanvas(this, this, library);

	m_canvas = canvas2d;

	d2d::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, NULL, NULL, cfg);
}

}