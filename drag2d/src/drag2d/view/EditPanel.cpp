#include "EditPanel.h"
#include "EditPanelImpl.h"

namespace d2d
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

Vector EditPanel::TransPosScrToProj(int x, int y) const
{
	return m_impl->TransPosScrToProj(x, y);
}

Vector EditPanel::TransPosProjToScr(const Vector& proj) const
{
	return m_impl->TransPosProjToScr(proj);
}

void EditPanel::DrawEditOP() const
{
	m_impl->DrawEditOP();
}

const AbstractEditOP* EditPanel::GetEditOP() const 
{ 
	return m_impl->GetEditOP();
}

AbstractEditOP* EditPanel::GetEditOP() 
{
	return m_impl->GetEditOP();
}

void EditPanel::SetEditOP(AbstractEditOP* editOP)
{
	m_impl->SetEditOP(editOP);
}

const IStageCanvas* EditPanel::GetCanvas() const 
{ 
	return m_impl->GetCanvas(); 
}

IStageCanvas* EditPanel::GetCanvas() 
{ 
	return m_impl->GetCanvas(); 
}

void EditPanel::SetCanvas(IStageCanvas* canvas) 
{ 
	m_impl->SetCanvas(canvas);
}

Camera* EditPanel::GetCamera() const 
{ 
	return m_impl->GetCamera();
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

void EditPanel::ResetCanvas()
{
	m_impl->ResetCanvas();
}

void EditPanel::Undo()
{
	m_impl->Undo();
}

void EditPanel::Redo()
{
	m_impl->Redo();
}

void EditPanel::AddOpRecord(AbstractAtomicOP* op)
{
	m_impl->AddOpRecord(op);
}

void EditPanel::SaveOpRecordList(const std::string& filepath, const std::vector<ISprite*>& sprites)
{
	m_impl->SaveOpRecordList(filepath, sprites);
}

void EditPanel::LoadOpRecordList(const std::string& filepath, const std::vector<ISprite*>& sprites)
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

void EditPanel::SetTitleStatus(bool dirty)
{
	m_impl->SetTitleStatus(dirty);
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

} // d2d