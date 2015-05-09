#include "EditPanel.h"
#include "HistoryList.h"

#include "operator/AbstractEditOP.h"
#include "view/Camera.h"
#include "view/GLCanvas.h"
#include "view/Frame.h"

#include <fstream>

namespace d2d
{

BEGIN_EVENT_TABLE(EditPanel, wxPanel)
 	EVT_SIZE(EditPanel::onSize)

//	EVT_MENU(Menu_UpOneLayer, EditPanel::onMenuUpOneLayer)
// 	EVT_MENU(Menu_DownOneLayer, EditPanel::onMenuDownOneLayer)
// 	EVT_MENU(Menu_InsertToDTex, EditPanel::onMenuInsertToDTex)
// 	EVT_MENU(Menu_RemoveFromDTex, EditPanel::onMenuRemoveFromDTex)
END_EVENT_TABLE()

std::string EditPanel::menu_entries[] = 
{
	"Up One Layer",
	"Down One Layer",
	"Insert To DTex",
	"Remove From DTex",
};

EditPanel::EditPanel(wxWindow* parent, wxTopLevelWindow* frame)
	: wxPanel(parent)
	, m_frame(frame)
{
	m_editOP = NULL;
	m_canvas = NULL;
	m_camera = new Camera;
	SetFocus();		// For onMouseWheelRotation

	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanel::onMenuUpOneLayer, this, Menu_UpOneLayer);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanel::onMenuDownOneLayer, this, Menu_DownOneLayer);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanel::onMenuInsertToDTex, this, Menu_InsertToDTex);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditPanel::onMenuRemoveFromDTex, this, Menu_RemoveFromDTex);
}

EditPanel::~EditPanel()
{
	clear();

	delete m_camera;
	delete m_canvas;
	if (m_editOP) {
		m_editOP->Release();
	}
}

void EditPanel::clear()
{
	if (m_editOP) {
		m_editOP->clear();
	}

	m_historyList.clear();
}

Vector EditPanel::transPosScreenToProject(int x, int y) const
{
	return m_camera->transPosScreenToProject(x, y, GetSize().GetWidth(), GetSize().GetHeight());
}

Vector EditPanel::transPosProjectToScreen(const Vector& proj) const
{
	return m_camera->transPosProjectToScreen(proj, GetSize().GetWidth(), GetSize().GetHeight());
}

void EditPanel::drawEditTemp() const
{
	if (m_editOP) {
		m_editOP->onDraw();
	}
}

void EditPanel::setEditOP(AbstractEditOP* editOP)
{
	if (m_editOP == editOP) {
		return;
	}

	if (editOP) {
		editOP->Retain();
	}
	if (m_editOP)
	{
		m_editOP->clear();
		m_editOP->Release();
	}
	m_editOP = editOP;
	if (m_editOP) {
		m_editOP->onActive();
	}
	Refresh();
}

void EditPanel::onMouse(wxMouseEvent& event)
{
#ifdef _DEBUG
	if (wxFrame* frame = dynamic_cast<wxFrame*>(m_frame)) 
	{
		Vector pos = transPosScreenToProject(event.GetX(), event.GetY());
		wxString msg;
		msg.Printf("Mouse: %.1f, %.1f", pos.x, pos.y);
		static_cast<wxFrame*>(m_frame)->SetStatusText(msg);
	}
#endif

	if (!m_editOP) return;

	if (event.LeftDown())
		m_editOP->onMouseLeftDown(event.GetX(), event.GetY());
	else if (event.LeftUp())
		m_editOP->onMouseLeftUp(event.GetX(), event.GetY());
	else if (event.RightDown())
		m_editOP->onMouseRightDown(event.GetX(), event.GetY());
	else if (event.RightUp())
		m_editOP->onMouseRightUp(event.GetX(), event.GetY());
	else if (event.Moving())
		m_editOP->onMouseMove(event.GetX(), event.GetY());
	else if (event.Dragging())
		m_editOP->onMouseDrag(event.GetX(), event.GetY());
	else if (event.LeftDClick())
		m_editOP->onMouseLeftDClick(event.GetX(), event.GetY());
	else if (event.GetWheelRotation())
		m_editOP->onMouseWheelRotation(event.GetX(), event.GetY(), event.GetWheelRotation());

	OnMouseHook(event);
}

void EditPanel::onKeyDown(wxKeyEvent& event)
{
	if (wxGetKeyState(WXK_CONTROL_Z) && wxGetKeyState(WXK_CONTROL))
		undo();
	else if (wxGetKeyState(WXK_CONTROL_Y) && wxGetKeyState(WXK_CONTROL))
		redo();

	int key_code = event.GetKeyCode();
	switch (key_code) {
	case WXK_F5:
		{
			d2d::Frame* frame = dynamic_cast<Frame*>(m_frame);
			if (frame) {
				clear();
				frame->RefreshWithCurrFile();
			}
		}
		break;
	}
	
	if (m_editOP) {
		m_editOP->onKeyDown(key_code);
	}
}

void EditPanel::onKeyUp(wxKeyEvent& event)
{
	if (m_editOP) {
		m_editOP->onKeyUp(event.GetKeyCode());
	}
}

void EditPanel::onMouseWheelRotation(int x, int y, int direction)
{
	const float scale = direction > 0 ? 1 / 1.1f : 1.1f;
	const float cx = static_cast<float>(x),
		cy = static_cast<float>(GetSize().GetHeight() - y);
	m_camera->Scale(scale, cx, cy, GetSize().GetWidth(), GetSize().GetHeight());
	Refresh();
}

void EditPanel::resetCanvas()
{
	if (m_canvas)
	{
		m_canvas->resetInitState();
		Refresh();
	}
}

void EditPanel::ResetViewport()
{
	if (m_canvas)
	{
		m_canvas->resetViewport();
//		Refresh();
	}
}

void EditPanel::undo()
{
	HistoryList::Type type = m_historyList.undo();
	if (type != HistoryList::NO_CHANGE) {
		Refresh();
		if (type == HistoryList::DIRTY)
			setTitleStatus(true);
		else
			setTitleStatus(false);
	}
}

void EditPanel::redo()
{
	HistoryList::Type type = m_historyList.redo();
	if (type != HistoryList::NO_CHANGE) {
		Refresh();
		if (type == HistoryList::DIRTY)
			setTitleStatus(true);
		else
			setTitleStatus(false);
	}
}

void EditPanel::addHistoryOP(AbstractAtomicOP* op)
{
	m_historyList.insert(op);
	setTitleStatus(true);
}

void EditPanel::saveHistoryList(const std::string& filepath, const std::vector<ISprite*>& sprites)
{
	Json::Value value;
	m_historyList.store(value, sprites);

	std::string path = filepath.substr(0, filepath.find_last_of('.')) + "_history.json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(path.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void EditPanel::loadHistoryList(const std::string& filepath, const std::vector<ISprite*>& sprites)
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

	m_historyList.load(value, sprites);
}

void EditPanel::onSave()
{
	m_historyList.onSave();
	setTitleStatus(false);
}

bool EditPanel::isDirty() const
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

void EditPanel::setTitleStatus(bool dirty)
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

void EditPanel::onMenuUpOneLayer(wxCommandEvent& event)
{
	if (m_editOP) {
		m_editOP->onPopMenuSelected(Menu_UpOneLayer);
		Refresh();
	}
}

void EditPanel::onMenuDownOneLayer(wxCommandEvent& event)
{
	if (m_editOP) {
		m_editOP->onPopMenuSelected(Menu_DownOneLayer);
		Refresh();
	}
}

void EditPanel::onMenuInsertToDTex(wxCommandEvent& event)
{
	if (m_editOP) {
		m_editOP->onPopMenuSelected(Menu_InsertToDTex);
		Refresh();
	}
}

void EditPanel::onMenuRemoveFromDTex(wxCommandEvent& event)
{
	if (m_editOP) {
		m_editOP->onPopMenuSelected(Menu_RemoveFromDTex);
		Refresh();
	}
}

void EditPanel::onKeyUpOneLayer(wxKeyEvent& event)
{
	if (m_editOP) {
		m_editOP->onPopMenuSelected(Menu_UpOneLayer);
		Refresh();
	}
}

void EditPanel::onKeyDownOneLayer(wxKeyEvent& event)
{
	if (m_editOP) {
		m_editOP->onPopMenuSelected(Menu_DownOneLayer);
		Refresh();
	}
}

// void EditPanel::onKeyDelete(wxKeyEvent& event)
// {
// 	if (!this->HasFocus())
// 		m_editOP->onKeyDown(event.GetKeyCode());
// }

void EditPanel::onSize(wxSizeEvent& event)
{
	if (m_canvas) {
		m_canvas->SetSize(event.GetSize());
	}
	Refresh();	// no refresh when change window size
}

} // d2d