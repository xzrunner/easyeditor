#include "EditPanel.h"
#include "HistoryList.h"
#include "IEditOPMonitor.h"

#include "operator/AbstractEditOP.h"
#include "view/Camera.h"
#include "view/IStageCanvas.h"
#include "view/Frame.h"

#include <fstream>

namespace d2d
{

BEGIN_EVENT_TABLE(EditPanel, wxPanel)
 	EVT_SIZE(EditPanel::OnSize)
END_EVENT_TABLE()

EditPanel::EditPanel(wxWindow* parent, wxTopLevelWindow* frame)
	: wxPanel(parent)
	, m_frame(frame)
	, m_op_monitor(NULL)
{
	m_edit_op = NULL;
	m_canvas = NULL;
	m_camera = new Camera;
	SetFocus();		// For OnMouseWheelRotation
}

EditPanel::~EditPanel()
{
	Clear();

	delete m_camera;
	delete m_canvas;
	if (m_edit_op) {
		m_edit_op->Release();
	}
}

void EditPanel::Clear()
{
	if (m_edit_op) {
		m_edit_op->Clear();
	}

	m_history_list.clear();
}

Vector EditPanel::TransPosScrToProj(int x, int y) const
{
	return m_camera->transPosScreenToProject(x, y, GetSize().GetWidth(), GetSize().GetHeight());
}

Vector EditPanel::TransPosProjToScr(const Vector& proj) const
{
	return m_camera->transPosProjectToScreen(proj, GetSize().GetWidth(), GetSize().GetHeight());
}

void EditPanel::DrawEditOP() const
{
	if (m_edit_op) {
		m_edit_op->OnDraw();
	}
}

void EditPanel::SetEditOP(AbstractEditOP* editOP)
{
	if (m_edit_op == editOP) {
		return;
	}

	if (editOP) {
		editOP->Retain();
	}
	if (m_edit_op)
	{
		m_edit_op->Clear();
		m_edit_op->Release();
	}
	m_edit_op = editOP;
	if (m_edit_op) {
		m_edit_op->OnActive();
	}
	Refresh();
}

void EditPanel::OnMouse(wxMouseEvent& event)
{
#ifdef _DEBUG
	if (wxFrame* frame = dynamic_cast<wxFrame*>(m_frame)) 
	{
		Vector pos = TransPosScrToProj(event.GetX(), event.GetY());
		wxString msg;
		msg.Printf("Mouse: %.1f, %.1f", pos.x, pos.y);
		static_cast<wxFrame*>(m_frame)->SetStatusText(msg);
	}
#endif

	if (!m_edit_op) return;

	if (event.LeftDown())
		m_edit_op->OnMouseLeftDown(event.GetX(), event.GetY());
	else if (event.LeftUp())
		m_edit_op->OnMouseLeftUp(event.GetX(), event.GetY());
	else if (event.RightDown())
		m_edit_op->OnMouseRightDown(event.GetX(), event.GetY());
	else if (event.RightUp())
		m_edit_op->OnMouseRightUp(event.GetX(), event.GetY());
	else if (event.Moving())
		m_edit_op->OnMouseMove(event.GetX(), event.GetY());
	else if (event.Dragging())
		m_edit_op->OnMouseDrag(event.GetX(), event.GetY());
	else if (event.LeftDClick())
		m_edit_op->OnMouseLeftDClick(event.GetX(), event.GetY());
	else if (event.GetWheelRotation())
		m_edit_op->OnMouseWheelRotation(event.GetX(), event.GetY(), event.GetWheelRotation());

	OnMouseHook(event);
}

void EditPanel::OnKeyDown(wxKeyEvent& event)
{
	if (wxGetKeyState(WXK_CONTROL_Z) && wxGetKeyState(WXK_CONTROL))
		Undo();
	else if (wxGetKeyState(WXK_CONTROL_Y) && wxGetKeyState(WXK_CONTROL))
		Redo();

	int key_code = event.GetKeyCode();
	switch (key_code) {
	case WXK_F5:
		{
			d2d::Frame* frame = dynamic_cast<Frame*>(m_frame);
			if (frame) {
				Clear();
				frame->RefreshWithCurrFile();
			}
		}
		break;
	}
	
	if (m_edit_op) {
		m_edit_op->OnKeyDown(key_code);
	}

	OnKeyHook(key_code);
}

void EditPanel::OnKeyUp(wxKeyEvent& event)
{
	if (m_edit_op) {
		m_edit_op->OnKeyUp(event.GetKeyCode());
	}
}

void EditPanel::OnMouseWheelRotation(int x, int y, int direction)
{
	const float scale = direction > 0 ? 1 / 1.1f : 1.1f;
	const float cx = static_cast<float>(x),
		cy = static_cast<float>(GetSize().GetHeight() - y);
	m_camera->Scale(scale, cx, cy, GetSize().GetWidth(), GetSize().GetHeight());
	Refresh();
}

void EditPanel::ResetCanvas()
{
	if (m_canvas)
	{
		m_canvas->ResetInitState();
		Refresh();
	}
}

void EditPanel::ResetViewport()
{
	if (m_canvas)
	{
		m_canvas->ResetViewport();
//		Refresh();
	}
}

void EditPanel::Undo()
{
	HistoryList::Type type = m_history_list.undo();
	if (type != HistoryList::NO_CHANGE) {
		Refresh();
		if (type == HistoryList::DIRTY)
			SetTitleStatus(true);
		else
			SetTitleStatus(false);
	}
}

void EditPanel::Redo()
{
	HistoryList::Type type = m_history_list.redo();
	if (type != HistoryList::NO_CHANGE) {
		Refresh();
		if (type == HistoryList::DIRTY)
			SetTitleStatus(true);
		else
			SetTitleStatus(false);
	}
}

void EditPanel::AddOpRecord(AbstractAtomicOP* op)
{
	if (m_op_monitor) {
		m_op_monitor->AddEditOP(op);
	}

	m_history_list.insert(op);
	SetTitleStatus(true);
}

void EditPanel::SaveOpRecordList(const std::string& filepath, const std::vector<ISprite*>& sprites)
{
	Json::Value value;
	m_history_list.store(value, sprites);

	std::string path = filepath.substr(0, filepath.find_last_of('.')) + "_history.json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(path.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void EditPanel::LoadOpRecordList(const std::string& filepath, const std::vector<ISprite*>& sprites)
{
	std::string path = filepath.substr(0, filepath.find_last_of('.')) + "_history.json";

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(path.c_str());
	std::locale::global(std::locale("C"));
	if (fin.fail()) {
		return;
	}
	reader.parse(fin, value);
	fin.close();

	m_history_list.load(value, sprites);
}

void EditPanel::OnSave()
{
	m_history_list.onSave();
	SetTitleStatus(false);
}

bool EditPanel::IsEditDirty() const
{
	if (!m_frame) {
		return false;
	}

	wxString title = m_frame->GetTitle();
	if (title.IsEmpty()) {
		return false;
	}
	return title[title.Len()-1] == '*';
}

void EditPanel::SetTitleStatus(bool dirty)
{
	if (!m_frame) return;

	wxString title = m_frame->GetTitle();
	if (title.IsNull()) return;
	if (dirty && title[title.Len()-1] != '*')
	{
		title.Append('*');
		m_frame->SetTitle(title);
	}
	else if (!dirty && title[title.Len()-1] == '*')
	{
		title = title.SubString(0, title.Len()-2);
		m_frame->SetTitle(title);
	}
}

void EditPanel::OnSize(wxSizeEvent& event)
{
	OnSizeDebug(event);

	if (m_canvas) {
		m_canvas->SetSize(event.GetSize());
	}
	Refresh();	// no refresh when change window size
}

void EditPanel::OnRightPopupMenu(wxCommandEvent& event)
{
	if (m_edit_op) {
		m_edit_op->OnPopMenuSelected(event.GetId());
		Refresh();
	}
}

} // d2d