#include "StagePanel.h"
#include "StageCanvas.h"
#include "Sprite.h"

namespace emesh
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
{
	m_sprite = new Sprite();

	init(library);
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library, Sprite* sprite)
	: d2d::EditPanel(parent, frame)
{
	sprite->retain();
	m_sprite = sprite;

	init(library);
}

StagePanel::~StagePanel()
{
	m_sprite->release();	
}

void StagePanel::clear()
{
	m_sprite->release();
	m_sprite = new Sprite;
}

Mesh* StagePanel::getMesh()
{
	return m_sprite->getMesh();
}

void StagePanel::init(d2d::LibraryPanel* library)
{
	m_editOP = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new DragSymbolTarget(this, library));
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

bool StagePanel::DragSymbolTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	d2d::ISymbol* symbol = m_library->getSymbol(index);
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		Sprite* sprite = new Sprite(new Symbol(image->getImage()));
		m_stage->m_sprite->release();
		m_stage->m_sprite = sprite;
	}

	return true;
}

}