#include "Frame.h"

#include "RectCutOP.h"
#include "RectCutCMPT.h"

namespace eimage
{

Frame::Frame(const wxString& title)
	: ee::Frame(title, "")
{
}

void Frame::OnNew(wxCommandEvent& event)
{
	ee::EditOP* editop = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	editop->Clear();
}

void Frame::OnOpen(wxCommandEvent& event)
{
	ee::EditOP* editop = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onLoadEditOP(event);
	}
}

void Frame::OnSave(wxCommandEvent& event)
{
	ee::EditOP* editop = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onSaveEditOP(event);
	}
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	ee::EditOP* editop = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (RectCutOP* op = dynamic_cast<RectCutOP*>(editop))
	{
		op->getEditCMPT()->onSaveEditOP(event);
	}
}

}