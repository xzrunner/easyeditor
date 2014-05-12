#include "Frame.h"

#include "RectCutOP.h"
#include "RectCutCMPT.h"

namespace eimage
{

Frame::Frame(const wxString& title)
	: d2d::Frame(title, "")
{
}

void Frame::onOpen(wxCommandEvent& event)
{
	d2d::AbstractEditOP* editop = m_task->getEditPanel()->getEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onLoadEditOP(event);
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	d2d::AbstractEditOP* editop = m_task->getEditPanel()->getEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onSaveEditOP(event);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	d2d::AbstractEditOP* editop = m_task->getEditPanel()->getEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onSaveEditOP(event);
	}
}

}