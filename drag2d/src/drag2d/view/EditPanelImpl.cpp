#include "EditPanelImpl.h"
#include "EditPanel.h"

#include "operator/AbstractEditOP.h"
#include "view/Camera.h"
#include "view/Frame.h"
#include "view/IStageCanvas.h"

#include "message/subject_id.h"
#include "message/ClearSpriteSJ.h"
#include "message/ClearPanelSJ.h"
#include "message/SetCanvasDirtySJ.h"

#include "message/panel_msg.h"

#include <fstream>

namespace d2d
{

EditPanelImpl::EditPanelImpl(wxTopLevelWindow* frame,
							 EditPanel* stage)
	: m_frame(frame)
	, m_stage(stage)
{
	m_edit_op = NULL;
	m_canvas = NULL;
	m_camera = new Camera;

	RegistSubjects();
}

EditPanelImpl::~EditPanelImpl()
{
	Clear();

	delete m_camera;
	if (m_edit_op) {
		m_edit_op->Release();
	}
	if (m_canvas) {
		m_canvas->Release();
		m_canvas = NULL;
	}

	UnRegistSubjects();
}

void EditPanelImpl::SetEditPanelNull()
{
	m_stage = NULL;
}

void EditPanelImpl::Notify(int sj_id, void* ud)
{
	switch (sj_id) 
	{
	case MSG_CLEAR_SPRITE: case MSG_CLEAR_PANEL:
		Clear();
		break;

	case MSG_EDIT_UNDO:
		Undo();
		break;
	case MSG_EDIT_REDO:
		Redo();
		break;
	case MSG_EDIT_ADD_RECORD:
		AddOpRecord((AbstractAtomicOP*)ud);
		break;
	case MSG_GET_KEY_STATE:
		{
			GetKeyStateSJ::State* st = (GetKeyStateSJ::State*)ud;
			st->state = GetKeyState(st->key);
		}
		break;
	}
}

bool EditPanelImpl::Update(int version)
{
	if (m_stage) {
		return m_stage->Update(version);
	} else {
		return false;
	}
}

Vector EditPanelImpl::TransPosScrToProj(int x, int y) const
{
	if (!m_stage) {
		return Vector(0, 0);
	}

	int w = m_stage->GetSize().GetWidth(),
		h = m_stage->GetSize().GetHeight();
	return m_camera->transPosScreenToProject(x, y, w, h);
}

Vector EditPanelImpl::TransPosProjToScr(const Vector& proj) const
{
	if (!m_stage) {
		return Vector(0, 0);
	}

	int w = m_stage->GetSize().GetWidth(),
		h = m_stage->GetSize().GetHeight();
	return m_camera->transPosProjectToScreen(proj, w, h);
}

void EditPanelImpl::DrawEditOP() const
{
	if (m_edit_op) {
		m_edit_op->OnDraw();
	}
}

void EditPanelImpl::SetEditOP(AbstractEditOP* editOP)
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
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void EditPanelImpl::SetCanvas(IStageCanvas* canvas) 
{ 
	obj_assign<IStageCanvas>(m_canvas, canvas);
}

void EditPanelImpl::OnMouse(wxMouseEvent& event)
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

	if (m_stage) {
		m_stage->OnMouseHook(event);
	}
}

void EditPanelImpl::OnKeyDown(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyDown(key_code);

	if (GetKeyState(WXK_CONTROL) && (key_code == 'z' || key_code == 'Z')) {
		Undo();
	} else if (GetKeyState(WXK_CONTROL) && (key_code == 'y' || key_code == 'Y')) {
		Redo();
	}

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

	if (m_stage) {
		m_stage->OnKeyHook(key_code);
	}
}

void EditPanelImpl::OnKeyUp(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyUp(key_code);

	if (m_edit_op) {
		m_edit_op->OnKeyUp(key_code);
	}
}

void EditPanelImpl::OnMouseWheelRotation(int x, int y, int direction)
{
	if (!m_stage) {
		return;
	}

	int w = m_stage->GetSize().GetWidth(),
		h = m_stage->GetSize().GetHeight();
	const float scale = direction > 0 ? 1 / 1.1f : 1.1f;
	const float cx = static_cast<float>(x),
		cy = static_cast<float>(h - y);
	m_camera->Scale(scale, cx, cy, w, h);
}

void EditPanelImpl::ResetCanvas()
{
	if (m_canvas)
	{
		m_canvas->ResetInitState();
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void EditPanelImpl::Undo()
{
	HistoryList::Type type = m_history_list.undo();
	if (type != HistoryList::NO_CHANGE) {
		SetCanvasDirtySJ::Instance()->SetDirty();
		if (type == HistoryList::DIRTY)
			SetTitleStatus(true);
		else
			SetTitleStatus(false);
	}
}

void EditPanelImpl::Redo()
{
	HistoryList::Type type = m_history_list.redo();
	if (type != HistoryList::NO_CHANGE) {
		SetCanvasDirtySJ::Instance()->SetDirty();
		if (type == HistoryList::DIRTY)
			SetTitleStatus(true);
		else
			SetTitleStatus(false);
	}
}

void EditPanelImpl::AddOpRecord(AbstractAtomicOP* op)
{
	m_history_list.insert(op);
	SetTitleStatus(true);
}

void EditPanelImpl::SaveOpRecordList(const std::string& filepath, const std::vector<ISprite*>& sprites)
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

void EditPanelImpl::LoadOpRecordList(const std::string& filepath, const std::vector<ISprite*>& sprites)
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

void EditPanelImpl::OnSave()
{
	m_history_list.onSave();
	SetTitleStatus(false);
}

bool EditPanelImpl::IsEditDirty() const
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

void EditPanelImpl::SetTitleStatus(bool dirty)
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

void EditPanelImpl::OnRightPopupMenu(wxCommandEvent& event)
{
	if (m_edit_op) {
		m_edit_op->OnPopMenuSelected(event.GetId());
	}
}

bool EditPanelImpl::GetKeyState(int key) const 
{ 
	return m_keys_state.GetKeyState(key);
}

void EditPanelImpl::RefreshFrame()
{
	m_frame->Refresh();
}

void EditPanelImpl::OnSize(wxSizeEvent& event)
{
	if (m_stage) {
		m_stage->OnSizeDebug(event);
	}

	if (m_canvas) {
		m_canvas->SetSize(event.GetSize());
	}
	SetCanvasDirtySJ::Instance()->SetDirty();	// no refresh when change window size
}

void EditPanelImpl::SetCursor(wxCursor cursor)
{
	if (m_stage) {
		m_stage->SetCursor(cursor);
	}
}

void EditPanelImpl::SetFocus()
{
	if (m_stage) {
		m_stage->SetFocus();
	}
}

void EditPanelImpl::PopupMenu(wxMenu* menu, int x, int y)
{
	if (m_stage) {
		m_stage->PopupMenu(menu, x, y);
	}
}

void EditPanelImpl::SetDropTarget(wxDropTarget* target)
{
	if (m_stage) {
		m_stage->SetDropTarget(target);
	}
}

void EditPanelImpl::Clear()
{
	if (m_edit_op) {
		m_edit_op->Clear();
	}

	m_history_list.clear();
}

void EditPanelImpl::RegistSubjects()
{
	m_subjects.push_back(ClearSpriteSJ::Instance());
	m_subjects.push_back(ClearPanelSJ::Instance());

	m_subjects.push_back(EditUndoSJ::Instance());
	m_subjects.push_back(EditRedoSJ::Instance());
	m_subjects.push_back(EditAddRecordSJ::Instance());
	m_subjects.push_back(GetKeyStateSJ::Instance());	

	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

void EditPanelImpl::UnRegistSubjects()
{
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

}