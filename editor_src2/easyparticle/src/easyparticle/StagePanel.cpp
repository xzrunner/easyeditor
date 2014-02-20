
#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"
#include "Context.h"

using namespace eparticle;

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, NULL)
	, m_particle(NULL)
{
	m_canvas = new StageCanvas(this);
	m_editOP = new EditOP(this);
}

StagePanel::~StagePanel()
{
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();
	d2d::SpritesPanelImpl::clearSprites();
}
