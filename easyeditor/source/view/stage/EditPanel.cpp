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
	m_impl->Release();
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

const EditOP* EditPanel::GetEditOP() const 
{ 
	return m_impl->GetEditOP();
}

EditOP* EditPanel::GetEditOP() 
{
	return m_impl->GetEditOP();
}

void EditPanel::SetEditOP(EditOP* editOP)
{
	m_impl->SetEditOP(editOP);
}

const StageCanvas* EditPanel::GetCanvas() const 
{ 
	return m_impl->GetCanvas(); 
}

StageCanvas* EditPanel::GetCanvas() 
{ 
	return m_impl->GetCanvas(); 
}

void EditPanel::SetCanvas(StageCanvas* canvas) 
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

void EditPanel::SaveOpRecordList(const std::string& filepath, const std::vector<Sprite*>& sprites)
{
	m_impl->SaveOpRecordList(filepath, sprites);
}

void EditPanel::LoadOpRecordList(const std::string& filepath, const std::vector<Sprite*>& sprites)
{
	m_impl->LoadOpRecordList(filepath, sprites);
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