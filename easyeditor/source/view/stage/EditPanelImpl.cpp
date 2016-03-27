#include "EditPanelImpl.h"
#include "EditPanel.h"
#include "Frame.h"
#include "StageCanvas.h"
#include "panel_msg.h"
#include "subject_id.h"
#include "EditOP.h"
#include "sprite_msg.h"
#include "StringHelper.h"
#include "SpriteRenderer.h"
#include "CameraMgr.h"
#include "OrthoCamera.h"

#include <fstream>

namespace ee
{

EditPanelImpl::EditPanelImpl(wxTopLevelWindow* frame,
							 EditPanel* stage)
	: m_frame(frame)
	, m_stage(stage)
{
	m_edit_op = NULL;
	m_canvas = NULL;
	InitSubjects();
}

EditPanelImpl::~EditPanelImpl()
{
	Clear();

	if (m_edit_op) {
		m_edit_op->Release();
	}
	if (m_canvas) {
		m_canvas->Release();
		m_canvas = NULL;
	}
}

void EditPanelImpl::SetEditPanelNull()
{
	m_stage = NULL;
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
	return CameraMgr::Instance()->GetCamera()->TransPosScreenToProject(x, y, w, h);
}

Vector EditPanelImpl::TransPosProjToScr(const Vector& proj) const
{
	if (!m_stage) {
		return Vector(0, 0);
	}

	int w = m_stage->GetSize().GetWidth(),
		h = m_stage->GetSize().GetHeight();
	return CameraMgr::Instance()->GetCamera()->TransPosProjectToScreen(proj, w, h);
}

void EditPanelImpl::DrawEditOP() const
{
	if (m_edit_op) {
		m_edit_op->OnDraw();
	}
}

void EditPanelImpl::SetEditOP(EditOP* editOP)
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

void EditPanelImpl::SetCanvas(StageCanvas* canvas) 
{ 
	obj_assign<StageCanvas>(m_canvas, canvas);
}

void EditPanelImpl::OnMouse(wxMouseEvent& event)
{
#ifdef _DEBUG
	if (wxFrame* frame = dynamic_cast<wxFrame*>(m_frame)) 
	{
		Vector pos = TransPosScrToProj(event.GetX(), event.GetY());
		std::string msg = ee::StringHelper::Format("Mouse: %.1f, %.1f", pos.x, pos.y);
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
			Frame* frame = dynamic_cast<Frame*>(m_frame);
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
	if (!m_stage || !CameraMgr::Instance()->IsType(CameraMgr::ORTHO)) {
		return;
	}

	int w = m_stage->GetSize().GetWidth(),
		h = m_stage->GetSize().GetHeight();
	const float scale = direction > 0 ? 1 / 1.1f : 1.1f;
	const float cx = static_cast<float>(x),
		cy = static_cast<float>(h - y);
	static_cast<OrthoCamera*>(CameraMgr::Instance()->GetCamera())->Scale(scale, cx, cy, w, h);
}

void EditPanelImpl::Undo()
{
	HistoryList::Type type = m_history_list.Undo();
	if (type != HistoryList::NO_CHANGE) {
		SetCanvasDirtySJ::Instance()->SetDirty();
		if (type == HistoryList::DIRTY)
			SetWndDirty(true);
		else
			SetWndDirty(false);
	}
}

void EditPanelImpl::Redo()
{
	HistoryList::Type type = m_history_list.Redo();
	if (type != HistoryList::NO_CHANGE) {
		SetCanvasDirtySJ::Instance()->SetDirty();
		if (type == HistoryList::DIRTY)
			SetWndDirty(true);
		else
			SetWndDirty(false);
	}
}

void EditPanelImpl::SaveOpRecordList(const std::string& filepath, const std::vector<Sprite*>& sprites)
{
	Json::Value value;
	m_history_list.Store(value, sprites);

	std::string path = filepath.substr(0, filepath.find_last_of('.')) + "_history.json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(path.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void EditPanelImpl::LoadOpRecordList(const std::string& filepath, const std::vector<Sprite*>& sprites)
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

	m_history_list.Load(value, sprites);
}

void EditPanelImpl::OnSave()
{
	m_history_list.OnSave();
	SetWndDirty(false);
}

bool EditPanelImpl::IsEditDirty() const
{
	if (!m_frame) {
		return false;
	}

	std::string title = m_frame->GetTitle();
	if (title.empty()) {
		return false;
	}
	return title[title.size() - 1] == '*';
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

void EditPanelImpl::OnNotify(int sj_id, void* ud)
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
		AddOpRecord((AtomicOP*)ud);
		break;
	case MSG_GET_KEY_STATE:
		{
			GetKeyStateSJ::State* st = (GetKeyStateSJ::State*)ud;
			st->state = GetKeyState(st->key);
		}
		break;
	case MSG_SET_WND_DIRTY:
		SetWndDirty(true);
		break;
	}
}

void EditPanelImpl::Clear()
{
	if (m_edit_op) {
		m_edit_op->Clear();
	}

	m_history_list.Clear();
}

void EditPanelImpl::InitSubjects()
{
	RegistSubject(InsertSpriteSJ::Instance());

	RegistSubject(ClearSpriteSJ::Instance());
	RegistSubject(ClearPanelSJ::Instance());

	RegistSubject(EditUndoSJ::Instance());
	RegistSubject(EditRedoSJ::Instance());
	RegistSubject(EditAddRecordSJ::Instance());
	RegistSubject(GetKeyStateSJ::Instance());	
	RegistSubject(SetWndDirtySJ::Instance());
}

void EditPanelImpl::AddOpRecord(AtomicOP* op)
{
	m_history_list.Insert(op);
	SetWndDirty(true);
}

void EditPanelImpl::SetWndDirty(bool dirty)
{
	if (!m_frame) return;

	std::string title = m_frame->GetTitle();
	if (title.empty()) return;
	if (dirty && title[title.size() - 1] != '*')
	{
		title += "*";
		m_frame->SetTitle(title);
	}
	else if (!dirty && title[title.size() - 1] == '*')
	{
		title = title.substr(0, title.size() - 2);
		m_frame->SetTitle(title);
	}
}

}