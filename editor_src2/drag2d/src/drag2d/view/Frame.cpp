#include "Frame.h"
#include "EJPreviewDialog.h"
#include "SettingsDialog.h"

#include "common/filetools.h"
#include "common/FileNameTools.h"
#include "common/Exception.h"
#include "view/ExceptionDlg.h"
#include "view/ExitDlg.h"

#include <easycoco.h>

namespace d2d
{

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

	EVT_MENU(ID_EJ_PREVIEW, Frame::onEJPreview)
	EVT_MENU(ID_SCREENSHOT, Frame::onScreenshot)
	
	EVT_MENU(ID_SETTINGS, Frame::onSettings)

	EVT_MENU(wxID_EXIT, Frame::onQuit)
	
	EVT_CLOSE(Frame::onClose)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600) /*wxDefaultSize, wxMAXIMIZE | wxCLOSE_BOX*/)
	, m_task(NULL)
	, m_filetag(filetag)
	, m_recent(this)
{
	initMenuBar();
}

void Frame::setTask(ITask* task)
{
	m_task = task;
}

void Frame::initWithFile(const wxString& path)
{
	m_task->clear();
	m_currFilename = path;
	SetTitle(path);
	try {
		m_task->load(path);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::onSettings(wxCommandEvent& event)
{
	SettingsDialog dlg(this);
	dlg.ShowModal();
}

void Frame::onNew(wxCommandEvent& event)
{
	if (!m_task) return;

	setCurrFilename();
	m_task->clear();
}

void Frame::onOpen(wxCommandEvent& event)
{
	if (!m_task) return;

	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		getFileFilter(), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		openFile(dlg.GetPath());
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (!m_task) return;

	if (!m_currFilename.empty())
	{
		SetTitle(m_currFilename);
		m_task->store(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		getFileFilter(), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), m_filetag, "json");
		m_currFilename = fixed;
		m_task->store(fixed);
	}
}

void Frame::onOpenRecent1(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(0);
	openFile(filename);
}

void Frame::onOpenRecent2(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(1);
	openFile(filename);
}

void Frame::onOpenRecent3(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(2);
	openFile(filename);
}

void Frame::onOpenRecent4(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(3);
	openFile(filename);
}

void Frame::onOpenRecent5(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(4);
	openFile(filename);
}

void Frame::onEJPreview(wxCommandEvent& event)
{
	std::vector<const ISprite*> sprites;
	m_task->getAllSprite(sprites);

	//////////////////////////////////////////////////////////////////////////

// 		EJPreviewDialog dlg(this, sprites);
// 		dlg.ShowModal();

	//////////////////////////////////////////////////////////////////////////

	const char* folder = "_tmp_ejoy2d_preview";
	MkDirRF(folder);

	libcoco::PackLuaFile pack;
	pack.pack(sprites, folder);

#ifdef _DEBUG
	wxString cwd = wxFileName::GetCwd();
	std::string workpath = cwd + "\\..\\..\\..\\..\\..\\editor_bin\\";
	std::string cmd = workpath + "ejoy2d.exe " + workpath + "ejoy2d\\preview\\play.lua";
#else
	std::string cmd = "ejoy2d.exe ejoy2d/preview/play.lua";
#endif
	WinExec(cmd.c_str(), /*SW_SHOWMAXIMIZED*/SW_NORMAL);
}

void Frame::onScreenshot(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save as PNG file"), wxEmptyString, wxEmptyString, 
		"PNG files (*.png)|*.png", wxFD_SAVE);
	if (dlg.ShowModal() != wxID_OK) {
		return;
	}

	wxString filepath = dlg.GetPath();

	d2d::Rect rect;
	rect.makeInfinite();
	std::vector<const ISprite*> sprites;
	m_task->getAllSprite(sprites);
	if (sprites.empty()) {
		return;
	}
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		const ISprite* sprite = sprites[i];
		love::Matrix t;
		t.setTransformation(sprite->getPosition().x, sprite->getPosition().y, sprite->getAngle(),
			sprite->getScale().x, sprite->getScale().y, 0, 0, sprite->getShear().x, sprite->getShear().y);
		
		float hw = sprite->getSymbol().getSize().xLength() * 0.5f;
		float hh = sprite->getSymbol().getSize().yLength() * 0.5f;

		Vector pos;
		pos = Math::transVector(Vector(-hw, -hh), t);
		rect.combine(pos);
		pos = Math::transVector(Vector(-hw,  hh), t);
		rect.combine(pos);
		pos = Math::transVector(Vector( hw,  hh), t);
		rect.combine(pos);
		pos = Math::transVector(Vector( hw, -hh), t);
		rect.combine(pos);
	}

	const d2d::EditPanel* stage = m_task->getEditPanel();
	d2d::Vector lefttop = stage->transPosProjectToScreen(d2d::Vector(rect.xMin, rect.yMax));
	d2d::Vector rightbottom = stage->transPosProjectToScreen(d2d::Vector(rect.xMax, rect.yMin));

	int x = lefttop.x;
	int y = lefttop.y;
	int width = rightbottom.x - x;
	int height = rightbottom.y - y;
	if (width <= 0 || height <= 0) {
		return;
	}

	size_t size = width*height*4;
	GLubyte* pixels = new GLubyte[size];
	if(!pixels) {
		return;
	}
	memset(&pixels[0], 0, size);

	glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	ImageSaver::storeToFile(pixels, width, height, filepath.ToStdString(), ImageSaver::e_png);

	delete[] pixels;
}

void Frame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::onClose(wxCloseEvent& event)
{
	if (!m_task->isDirty())
	{
		Destroy();
		return;
	}

	ExitDlg dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_OK)
		m_task->store(m_currFilename);

	Destroy();
}

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	menuBar->Append(initViewBar(), "&View");
	menuBar->Append(m_setting_menu = initSettingsBar(), "&Settings");
	SetMenuBar(menuBar);
}

wxMenu* Frame::initFileBar()
{
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a project"));
	fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
	fileMenu->Append(wxID_SAVEAS, wxT("&Save as...\tF11"), wxT("Save to a new file"));
	fileMenu->AppendSeparator();
	fileMenu->AppendSubMenu(m_recent.getMenu(), wxT("Recent Files"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit"));
	return fileMenu;
}

wxMenu* Frame::initViewBar()
{
	wxMenu* viewMenu = new wxMenu;
	viewMenu->Append(ID_EJ_PREVIEW, wxT("&EJOY2D\tF5"), wxT("Preview by ejoy2d"));
	viewMenu->Append(ID_SCREENSHOT, wxT("Screenshot\tCtrl+Shift+c"), wxT("Screenshot"));
	return viewMenu;
}

wxMenu* Frame::initSettingsBar()
{
	wxMenu* settingsMenu = new wxMenu;
	settingsMenu->Append(ID_SETTINGS, wxT("Settings"), wxT("Settings"));
	return settingsMenu;
}

wxMenu* Frame::initHelpBar()
{
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About...\tF1"), wxT("Show about dialog"));
	return helpMenu;
}

wxString Frame::getFileFilter() const
{
	return wxT("*_") + m_filetag + wxT(".json");
}

void Frame::setCurrFilename()
{
	int id = 0;
	while (true)
	{
		wxString str = 
			wxT("new") + 
			wxString::FromDouble(id++) + 
			wxT(".json");

		if (!d2d::FilenameTools::isExist(str))
		{
			m_currFilename = str;
			break;
		}
	}
}

void Frame::openFile(const wxString& filename)
{
	if (!m_task || filename.IsEmpty()) {
		return;
	}

	m_task->clear();

	m_currFilename = filename;
	m_recent.insert(m_currFilename);
	SetTitle(m_currFilename);

	try {
		m_task->load(m_currFilename);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

//////////////////////////////////////////////////////////////////////////
// class Frame::RecentFiles
//////////////////////////////////////////////////////////////////////////

Frame::RecentFiles::RecentFiles(wxFrame* frame)
	: m_frame(frame)
	, m_menu(new wxMenu)
{
}

void Frame::RecentFiles::insert(const wxString& filename)
{
	std::deque<wxString>::iterator itr = m_files.begin();
	for ( ; itr != m_files.end(); ++itr)
	{
		if (*itr == filename) {
			return;
		}
	}

	if (m_files.size() >= CAPACITY)
	{
		m_files.pop_front();

		while (m_menu->GetMenuItemCount() > 0) {
			m_menu->Remove(m_menu->FindItemByPosition(0));
		}

		std::deque<wxString>::iterator itr = m_files.begin();
		for ( ; itr != m_files.end(); ++itr)
		{
			insertOnlyMenu(*itr);
		}
	}

	m_files.push_back(filename);
	insertOnlyMenu(filename);
}

void Frame::RecentFiles::insertOnlyMenu(const wxString& filename)
{
	int size = m_files.size();
	if (size > CAPACITY) {
		return;
	}
	int id = ID_RECENT_FILES + size;
	m_menu->Append(id, filename);
	if (size == 1) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent1));
	} else if (size == 2) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent2));
	} else if (size == 3) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent3));
	} else if (size == 4) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent4));
	} else if (size == 5) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent5));
	}
}

}