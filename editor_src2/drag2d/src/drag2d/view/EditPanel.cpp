#include "EditPanel.h"
#include "HistoryList.h"

#include "common/Context.h"
#include "operator/AbstractEditOP.h"
#include "view/Camera.h"
#include "view/GLCanvas.h"

namespace d2d
{

BEGIN_EVENT_TABLE(EditPanel, wxPanel)
	EVT_MOUSE_EVENTS(EditPanel::onMouse)
	EVT_KEY_DOWN(EditPanel::onKeyDown)
	EVT_KEY_UP(EditPanel::onKeyUp)
	EVT_SIZE(EditPanel::onSize)

	EVT_MENU(Menu_UpOneLayer, EditPanel::onMenuUpOneLayer)
	EVT_MENU(Menu_DownOneLayer, EditPanel::onMenuDownOneLayer)
	EVT_HOTKEY(Hot_UpOneLayer, EditPanel::onKeyUpOneLayer)
	EVT_HOTKEY(Hot_DownOneLayer, EditPanel::onKeyDownOneLayer)
//	EVT_HOTKEY(Hot_Delete, EditPanel::onKeyDelete)
END_EVENT_TABLE()

std::string EditPanel::menu_entries[] = 
{
	"Up One Layer",
	"Down One Layer"
};

EditPanel::EditPanel(wxWindow* parent, wxTopLevelWindow* frame)
	: wxPanel(parent)
	, m_frame(frame)
{
	RegisterHotKey(Hot_UpOneLayer, 0, VK_ADD);
	RegisterHotKey(Hot_DownOneLayer, 0, VK_SUBTRACT);
//	RegisterHotKey(Hot_Delete, 0, VK_DELETE);

	m_editOP = NULL;
	m_canvas = NULL;
	m_camera = new Camera;
	m_camera->addObserver(this);
	SetFocus();		// For onMouseWheelRotation
}

EditPanel::~EditPanel()
{
	clear();

	delete m_camera;
	delete m_canvas;
	if (m_editOP) m_editOP->release();
}

void EditPanel::onCameraChanged()
{
	if (m_canvas)
		m_canvas->resetViewport();
}

void EditPanel::clear()
{
	m_editOP->clear();

	m_historyList.clear();
}

Vector EditPanel::transPosScreenToProject(int x, int y) const
{
	return m_camera->transPosScreenToProject(x, y, GetSize().GetWidth(), GetSize().GetHeight());
}

// Vector EditPanel::transPosProjectToScreen(const Vector& proj) const
// {
// 	return m_camera->transPosProjectToScreen(proj, GetSize().GetWidth(), GetSize().GetHeight());
// }

void EditPanel::drawEditTemp() const
{
	if (m_editOP) m_editOP->onDraw();
}

void EditPanel::setEditOP(AbstractEditOP* editOP)
{
	if (m_editOP)
	{
		m_editOP->clear();
		m_editOP->release();
	}

	editOP->onActive();
	editOP->retain();
	m_editOP = editOP;
}

void EditPanel::onMouse(wxMouseEvent& event)
{
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
}

void EditPanel::onKeyDown(wxKeyEvent& event)
{
	if (wxGetKeyState(WXK_CONTROL_Z))
		undo();
	else if (wxGetKeyState(WXK_CONTROL_Y))
		redo();
	
	m_editOP->onKeyDown(event.GetKeyCode());
}

void EditPanel::onKeyUp(wxKeyEvent& event)
{
	m_editOP->onKeyUp(event.GetKeyCode());
}

void EditPanel::onMouseWheelRotation(int x, int y, int direction)
{
	const float oldScale = m_camera->getScale();
	const float newScale = direction > 0 ? oldScale / 1.1f : oldScale * 1.1f;
	const float cx = static_cast<float>(x),
		cy = static_cast<float>(GetSize().GetHeight() - y);
	m_camera->setScale(newScale, cx, cy, GetSize().GetWidth(), GetSize().GetHeight());
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

void EditPanel::undo()
{
	HistoryList::Type type = m_historyList.undo();
	if (type != HistoryList::NO_CHANGE) {
		Refresh();
		if (type == HistoryList::FIXED)
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
		if (type == HistoryList::FIXED)
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

void EditPanel::saveHistoryList(Json::Value& value, const std::vector<ISprite*>& sprites)
{
	m_historyList.store(value, sprites);
}

void EditPanel::loadHistoryList(const Json::Value& value, const std::vector<ISprite*>& sprites)
{
	m_historyList.load(value, sprites);
}

void EditPanel::onSave()
{
	m_historyList.onSave();
	setTitleStatus(false);
}

bool EditPanel::isDirty() const
{
	if (!m_frame) return false;

	wxString title = m_frame->GetTitle();
	return title[title.Len()-1] == '*';
}

void EditPanel::onMenuUpOneLayer(wxCommandEvent& event)
{
	m_editOP->onPopMenuSelected(Menu_UpOneLayer);
	Refresh();
}

void EditPanel::onMenuDownOneLayer(wxCommandEvent& event)
{
	m_editOP->onPopMenuSelected(Menu_DownOneLayer);
	Refresh();
}

void EditPanel::onKeyUpOneLayer(wxKeyEvent& event)
{
	m_editOP->onPopMenuSelected(Menu_UpOneLayer);
	Refresh();
}

void EditPanel::onKeyDownOneLayer(wxKeyEvent& event)
{
	m_editOP->onPopMenuSelected(Menu_DownOneLayer);
	Refresh();
}

// void EditPanel::onKeyDelete(wxKeyEvent& event)
// {
// 	if (!this->HasFocus())
// 		m_editOP->onKeyDown(event.GetKeyCode());
// }

void EditPanel::onSize(wxSizeEvent& event)
{
	if (m_canvas)
		m_canvas->SetSize(event.GetSize());
	m_camera->setCenter(Vector(0.0f, 0.0f));
	Refresh();	// no refresh when change window size
}

void EditPanel::setTitleStatus(bool fixed)
{
	if (!m_frame) return;

	wxString title = m_frame->GetTitle();
	if (title.IsNull()) return;
	if (fixed && title[title.Len()-1] != '*')
	{
		title.Append('*');
		m_frame->SetTitle(title);
	}
	else if (!fixed && title[title.Len()-1] == '*')
	{
		title = title.SubString(0, title.Len()-2);
		m_frame->SetTitle(title);
	}
}
} // d2d