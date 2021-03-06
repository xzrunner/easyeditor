#include "EditPanel.h"
#include "EditPanelImpl.h"

namespace ee
{

BEGIN_EVENT_TABLE(EditPanel, wxPanel)
 	EVT_SIZE(EditPanel::OnSize)
END_EVENT_TABLE()

EditPanel::EditPanel(wxWindow* parent, wxTopLevelWindow* frame)
	: wxPanel(parent)
{
	m_impl = new EditPanelImpl(frame, this);

	SetFocus();		// For OnMouseWheelRotation
}

EditPanel::~EditPanel()
{
	m_impl->RemoveReference();
}

sm::vec2 EditPanel::TransPosScrToProj(int x, int y) const
{
	return m_impl->TransPosScrToProj(x, y);
}

sm::vec2 EditPanel::TransPosProjToScr(const sm::vec2& proj) const
{
	return m_impl->TransPosProjToScr(proj);
}

void EditPanel::DrawEditOP() const
{
	m_impl->DrawEditOP();
}

const std::shared_ptr<ee::EditOP>& EditPanel::GetEditOP() const
{ 
	return m_impl->GetEditOP();
}

std::shared_ptr<ee::EditOP>& EditPanel::GetEditOP()
{
	return m_impl->GetEditOP();
}

void EditPanel::SetEditOP(const std::shared_ptr<EditOP>& op)
{
	m_impl->SetEditOP(op);
}

const std::shared_ptr<StageCanvas>& EditPanel::GetCanvas() const
{ 
	return m_impl->GetCanvas(); 
}

std::shared_ptr<StageCanvas>& EditPanel::GetCanvas()
{ 
	return m_impl->GetCanvas(); 
}

void EditPanel::SetCanvas(const std::shared_ptr<StageCanvas>& canvas)
{ 
	m_impl->SetCanvas(canvas);
}

void EditPanel::OnMouse(wxMouseEvent& event)
{
	m_impl->OnMouse(event);
}

void EditPanel::OnKeyDown(wxKeyEvent& event)
{
	m_impl->OnKeyDown(event);
}

void EditPanel::OnKeyUp(wxKeyEvent& event)
{
	m_impl->OnKeyUp(event);
}

void EditPanel::OnMouseWheelRotation(int x, int y, int direction)
{
	m_impl->OnMouseWheelRotation(x, y, direction);
}

void EditPanel::SaveOpRecordList(const std::string& filepath, const std::vector<SprPtr>& sprs)
{
	m_impl->SaveOpRecordList(filepath, sprs);
}

void EditPanel::LoadOpRecordList(const std::string& filepath, const std::vector<SprPtr>& sprs)
{
	m_impl->LoadOpRecordList(filepath, sprs);
}

void EditPanel::OnSave()
{
	m_impl->OnSave();
}

bool EditPanel::IsEditDirty() const
{
	return m_impl->IsEditDirty();
}

void EditPanel::OnRightPopupMenu(wxCommandEvent& event)
{
	m_impl->OnRightPopupMenu(event);
}

bool EditPanel::GetKeyState(int key) const 
{ 
	return m_impl->GetKeyState(key);
}

const KeysState& EditPanel::GetKeyState() const 
{ 
	return m_impl->GetKeyState();
}

KeysState& EditPanel::GetKeyState() 
{ 
	return m_impl->GetKeyState(); 
}

void EditPanel::RefreshFrame()
{
	m_impl->RefreshFrame();
}

void EditPanel::OnSize(wxSizeEvent& event)
{
	m_impl->OnSize(event);
}

}