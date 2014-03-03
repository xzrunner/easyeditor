
#include "Frame.h"
#include "SettingGameDlg.h"
#include "SettingOutputDlg.h"
#include "FileIO.h"

#include "dataset/Game.h"
#include "view/Context.h"
#include "view/LibraryPanel.h"
#include "view/PropertySettingPanel.h"
#include "view/StagePanel.h"
#include "view/BackstagePanel.h"
#include "view/LayersPage.h"
#include "view/ScenesPage.h"
#include "view/SceneItem.h"
#include "output/DesignerPage.h"
#include "preview/Dialog.h"

#include <wx/splitter.h>

using namespace ebuilder;

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)
	EVT_MENU(myID_MENU_SAVE_CODES, Frame::onSaveCodes)

	EVT_MENU(wxID_EXIT, Frame::onQuit)
	EVT_MENU(myID_MENU_PREVIEW, Frame::onPreview)
	EVT_MENU(myID_MENU_REFRESH, Frame::onRefresh)
	EVT_MENU(myID_MENU_FULL, Frame::onFullScreenStage)
	EVT_MENU(wxID_HELP, Frame::onAbout)
END_EVENT_TABLE()

const float Frame::SASH_GRAVITY_HOR = 0.5F;
const float Frame::SASH_GRAVITY_VER = 0.15F;

static const wxString VERSION = wxT("0.13.0514");

Frame::Frame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	initMenuBar();
	initWorkingFrame();
}

void Frame::onNew(wxCommandEvent& event)
{
	setCurrFilename();
	clear();
	Context::Instance()->library->getScenePage()->insert();
}

void Frame::onOpen(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("open from json file"), wxEmptyString, wxEmptyString, 
		wxT("*.json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		clear();
		m_currFilename = dlg.GetPath();
		SetTitle(d2d::FilenameTools::getFilename(dlg.GetPath()));
		Game::DLG_PATH = d2d::FilenameTools::getFileDir(dlg.GetPath()) + "\\";
		d2d::Settings::RESOURCE_PATH.insert(Game::DLG_PATH);
		try {
			FileIO::load(dlg.GetPath());
		} catch (d2d::Exception& e) {
			d2d::ExceptionDlg dlg(this, e);
			dlg.ShowModal();
		}
		Context::Instance()->stage->rebuildCodePages();
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (!m_currFilename.empty())
	{
		SetTitle(d2d::FilenameTools::getFilename(m_currFilename));
		FileIO::store(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("save to json file"), wxEmptyString, wxEmptyString, 
		wxT("*.json"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_currFilename = d2d::FilenameTools::getFilePathExceptExtension(dlg.GetPath()) + wxT(".json");
		FileIO::store(m_currFilename);
	}
}

void Frame::onSaveCodes(wxCommandEvent& event)
{
	wxDirDialog dlg(NULL, "Choose directory", wxEmptyString,
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		Context::Instance()->stage->storeAllCodePages(dlg.GetPath()); 
	}
}

void Frame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::onPreview(wxCommandEvent& event)
{
	Context* context = Context::Instance();
	preview::Dialog dlg(this, 
		dynamic_cast<SceneItem*>(context->library->getSceneList()->getItem())->getScene(), 
		context->library);
	dlg.ShowModal();

	context->stage->getDesignerPage()->resetCanvas();
}

void Frame::onRefresh(wxCommandEvent& event)
{
	
}

void Frame::onFullScreenStage(wxCommandEvent& event)
{
	if (m_hRightSplitter->IsSplit())
		m_hRightSplitter->Unsplit();
	else
	{
		Context* context = Context::Instance();
		context->stage->Show(true);
		context->backstage->Show(true);
 		m_hRightSplitter->SetSashGravity(SASH_GRAVITY_HOR);
 		m_hRightSplitter->SplitHorizontally(
			context->stage, context->backstage);
	}
}

void Frame::onSettingGameMenu(wxCommandEvent& event)
{
	SettingGameDlg dlg(this);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString name = dlg.getName();
		if (name != Game::NAME)
		{
			Game::NAME = name;
			Context::Instance()->stage->updateCodePages(NULL);
		}

		int width = dlg.getWidth(),
			height = dlg.getHeight();
		if (width != Game::WIDTH || height != Game::HEIGHT)
		{
			Game::WIDTH = dlg.getWidth();
			Game::HEIGHT = dlg.getHeight();
			Context::Instance()->stage->Refresh();
		}
	}
}

void Frame::onSettingOutputMenu(wxCommandEvent& event)
{
	SettingOutputDlg dlg(this);
	if (dlg.ShowModal() == wxID_OK)
	{
		Settings::CodeType type = dlg.getType();
		if (type != Settings::code)
		{
			Settings::code = type;
			Context::Instance()->stage->rebuildCodePages();
		}
	}
}

void Frame::onAbout(wxCommandEvent& event)
{
	wxString msg;
	msg.Printf(wxT("Version: ") + VERSION);
	wxMessageBox(msg, wxT("About"), wxOK | wxICON_INFORMATION, this);
}

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	menuBar->Append(initViewBar(), "&View");
	menuBar->Append(initSettingsBar(), "&Settings");
	menuBar->Append(initHelpBar(), "&Help");	
	SetMenuBar(menuBar);
}

wxMenu* Frame::initFileBar()
{
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a project"));
	fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
	fileMenu->Append(wxID_SAVEAS, wxT("&Save as...\tF11", wxT("Save to a new file")));
	fileMenu->Append(myID_MENU_SAVE_CODES, wxT("&Save code...\t", wxT("Save code to file")));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit GameFruits"));
	return fileMenu;
}

wxMenu* Frame::initViewBar()
{
	wxMenu* viewMenu = new wxMenu;
	viewMenu->Append(myID_MENU_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Running"));
	viewMenu->Append(myID_MENU_REFRESH, wxT("&Refresh\tF5"), wxT("Refresh code pages"));
	viewMenu->Append(myID_MENU_FULL, wxT("&Full Screen\tF6"), wxT("Full screen stage"));
	return viewMenu;
}

wxMenu* Frame::initSettingsBar()
{
	wxMenu* menu = new wxMenu;

	menu->Append(myID_MENU_SETTINGS_GAME, wxT("Game..."));
	Connect(myID_MENU_SETTINGS_GAME, wxEVT_COMMAND_MENU_SELECTED, 
		wxCommandEventHandler(Frame::onSettingGameMenu));

	menu->Append(myID_MENU_SETTINGS_OUTPUT, wxT("Output..."));
	Connect(myID_MENU_SETTINGS_OUTPUT, wxEVT_COMMAND_MENU_SELECTED, 
		wxCommandEventHandler(Frame::onSettingOutputMenu));

	return menu;
}

wxMenu* Frame::initHelpBar()
{
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About...\tF1"), wxT("Show about dialog"));
	return helpMenu;
}

void Frame::initWorkingFrame()
{
	Context* context = Context::Instance();

	m_vSplitter = new wxSplitterWindow(this);

	m_hLeftSplitter = new wxSplitterWindow(m_vSplitter);
	m_hRightSplitter = new wxSplitterWindow(m_vSplitter);
	m_vSplitter->SetSashGravity(SASH_GRAVITY_VER);
	m_vSplitter->SplitVertically(m_hLeftSplitter, m_hRightSplitter);
	
	// left

	context->library = new LibraryPanel(m_hLeftSplitter);
	context->library->SetBackgroundColour(wxColour("#afafaf"));

	context->property = new PropertySettingPanel(m_hLeftSplitter);
	context->property->SetBackgroundColour(wxColour("#afafaf"));

	m_hLeftSplitter->SetSashGravity(SASH_GRAVITY_HOR);
	m_hLeftSplitter->SplitHorizontally(
		context->library, context->property);

	// right

	context->backstage = new BackstagePanel(m_hRightSplitter);
	context->backstage->SetBackgroundColour(wxColour("#afafaf"));

	context->stage = new StagePanel(m_hRightSplitter, this);
	context->stage->SetBackgroundColour(wxColour("#afafaf"));

	m_hRightSplitter->SetSashGravity(SASH_GRAVITY_HOR);
	m_hRightSplitter->SplitHorizontally(
		context->stage, context->backstage);

	Context::Instance()->library->getScenePage()->insert();
	Context::Instance()->library->getLayerPage()->insert();
}

void Frame::clear()
{
	Context* context = Context::Instance();
	context->library->clear();
	context->stage->clear();
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
