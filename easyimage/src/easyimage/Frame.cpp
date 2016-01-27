#include "Frame.h"

#include "RectCutOP.h"
#include "RectCutCMPT.h"

namespace eimage
{

Frame::Frame(const wxString& title)
	: d2d::Frame(title, "")
{
}

void Frame::OnNew(wxCommandEvent& event)
{
	d2d::AbstractEditOP* editop = const_cast<d2d::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	editop->Clear();
}

void Frame::OnOpen(wxCommandEvent& event)
{
	d2d::AbstractEditOP* editop = const_cast<d2d::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onLoadEditOP(event);
	}
}

void Frame::OnSave(wxCommandEvent& event)
{
	d2d::AbstractEditOP* editop = const_cast<d2d::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onSaveEditOP(event);
	}
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	d2d::AbstractEditOP* editop = const_cast<d2d::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onSaveEditOP(event);
	}
}

}