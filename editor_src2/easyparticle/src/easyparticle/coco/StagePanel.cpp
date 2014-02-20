
#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"

#include "../Context.h"
#include "../LibraryPanel.h"
#include "ToolBarPanel.h"

namespace eparticle
{
namespace coco
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, Context::Instance()->library)
	, m_particle(NULL)
{
	xRot = yRot = 0;

	m_canvas = new StageCanvas(this);
	m_editOP = new EditOP(this);
}

StagePanel::~StagePanel()
{
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();
	Context::Instance()->toolbar->initParticle();
}

}
}