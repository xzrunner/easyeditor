#include "StagePanel2D.h"
#include "StageCanvas2D.h"
#include "SelectSpritesOP.h"

#include <easysketch.h>

namespace eshader
{

StagePanel2D::StagePanel2D(wxWindow* parent, wxTopLevelWindow* frame, 
						   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, m_shader(NULL)
{
	SetCanvas(new StageCanvas2D(this));

	ee::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	SetEditOP(new ee::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, NULL, NULL, cfg));
}

}