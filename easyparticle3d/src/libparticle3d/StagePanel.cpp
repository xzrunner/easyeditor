#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"
#include "LibraryPanel.h"
#include "ToolBarPanel.h"

namespace eparticle3d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_ps(NULL)
{
	xRot = yRot = 0;

	m_canvas = new StageCanvas(this);
	m_editOP = new EditOP(this);
}

StagePanel::~StagePanel()
{
}

}