#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"
#include "LibraryPanel.h"
#include "ToolBarPanel.h"
#include "ParticleSystem.h"

namespace eparticle2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_ps(NULL)
{
	m_canvas = new StageCanvas(this);
	m_editOP = new EditOP(this);

	m_ps = new ParticleSystem(1000);
}

StagePanel::~StagePanel()
{
}

}