#include "StagePanel.h"
#include "StageCanvas.h"

namespace eicon
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
{
	SetCanvas(new StageCanvas(this));

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::~StagePanel()
{
}

void StagePanel::Clear()
{
}

void StagePanel::SetIcon(Icon* icon)
{
	m_symbol.SetIcon(icon);	
}

Icon* StagePanel::GetIcon()
{
	return m_symbol.GetIcon();
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
	: d2d::StageDropTarget(stage, stage->GetStageImpl(), NULL, library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos)
{
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		m_stage->SetImage(image->getImage());
		m_stage->SetCanvasDirty();
		return true;
	}

	return false;
}

}