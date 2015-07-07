#include "StagePanel.h"
#include "StageCanvas.h"

namespace eicon
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
{
	m_canvas = new StageCanvas(this);

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::~StagePanel()
{
}

void StagePanel::clear()
{
}

void StagePanel::SetIcon(Icon* icon)
{
	m_symbol.SetIcon(icon);	
}

void StagePanel::SetImage(d2d::Image* img)
{
	m_symbol.SetImage(img);
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::StageDropTarget(stage, NULL, library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos)
{
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		m_stage->SetImage(image->getImage());
		m_stage->getCanvas()->ResetViewport();
		return true;
	}

	return false;
}

}