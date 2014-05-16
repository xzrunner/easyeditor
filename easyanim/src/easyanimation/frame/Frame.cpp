#include <wx/splitter.h>

#include <easybuilder.h>

#include "Frame.h"
#include "FileIO.h"
#include "CodeSettingDlg.h"

#include "Love2dCode.h"

#include "frame/Context.h"
#include "view/LibraryPanel.h"
#include "view/StagePanel.h"
#include "view/StageCanvas.h"
#include "view/TimeLinePanel.h"
#include "view/ToolbarPanel.h"
#include "view/PreviewDialog.h"

#include "../../libanim/config.h"

#include <easycomplex.h>
#include <easymesh.h>
#include <easyscale9.h>

namespace eanim
{

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

	EVT_MENU(wxID_EXIT, Frame::onQuit)
	EVT_MENU(ID_PREVIEW, Frame::onPreview)
	EVT_MENU(ID_SETTINGS, Frame::onSettings)
	EVT_MENU(ID_BACKGROUND, Frame::onSetBackground)
//	EVT_MENU(wxID_HELP, Frame::onAbout)

	EVT_MENU(ID_CODESETTING, Frame::onCodeSetting)
	EVT_MENU(ID_LOVE2D, Frame::onCodeLove2d)
END_EVENT_TABLE()

Frame::Frame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	setCurrFilename();
	initMenuBar();
	initWorkingFrame();
}

void Frame::initWithFile(const wxString& path)
{	
	clear();
	m_currFilename = path;
	SetTitle(path);
	try {
		FileIO::load(m_currFilename);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

d2d::GLCanvas* Frame::getCanvas()
{
	d2d::EditPanel* panel = static_cast<d2d::EditPanel*>(Context::Instance()->stage);
	return panel->getCanvas();
}

void Frame::onNew(wxCommandEvent& event)
{
	setCurrFilename();
	clear();
	Refresh();
}

void Frame::onOpen(wxCommandEvent& event)
{
	wxString filter = wxString("DOMDocument.xml") + ";" 
		+ wxString(wxT("*_") + wxString(FILE_TAG) + wxT(".json"));
  	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
  	if (dlg.ShowModal() == wxID_OK)
  	{
 		clear();
 		m_currFilename = dlg.GetPath().ToStdString();
 		SetTitle(d2d::FilenameTools::getFilename(dlg.GetPath()));
		try {
			if (d2d::FilenameTools::getFilenameWithExtension(m_currFilename) == wxT("DOMDocument.xml"))
				FileIO::loadFlash(m_currFilename);
			else
				FileIO::load(m_currFilename);
		} catch (d2d::Exception& e) {
			d2d::ExceptionDlg dlg(this, e);
			dlg.ShowModal();
		}
 		Refresh();
  	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (m_currFilename.empty()) {
		return;
	}

	wxMessageDialog* dlg = new wxMessageDialog(NULL, 
		wxT("Are you sure to save?"), wxT("Question"), 
		wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if (dlg->ShowModal() == wxID_YES)
	{
		SetTitle(d2d::FilenameTools::getFilename(m_currFilename));
		FileIO::store(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	wxString filter = "JSON files (*.json)|*.json|GIF files (*.gif)|*.gif";
 	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
 	if (dlg.ShowModal() == wxID_OK)
 	{
		wxString filename = dlg.GetPath();
		wxString ext = d2d::FilenameTools::getExtension(filename);
		if (ext == "gif")
		{
			FileIO::storeAsGif(m_currFilename, filename);
		}
		else
		{
			wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), FILE_TAG, "json");
			m_currFilename = fixed;
			FileIO::store(m_currFilename);
		}
 	}
}

void Frame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::onAbout(wxCommandEvent& event)
{
	wxString msg;
	msg.Printf(wxT("Game animation tools.\n(c) zz, 2012"));
	wxMessageBox(msg, wxT("About FruitsAnimator"), wxOK | wxICON_INFORMATION, this);
}

void Frame::onPreview(wxCommandEvent& event)
{
	PreviewDialog dlg(this);
	dlg.ShowModal();
	static_cast<StagePanel*>(Context::Instance()->stage)->resetCanvas();
}

void Frame::onCodeSetting(wxCommandEvent& event)
{
	CodeSettingDlg dlg(this);
	dlg.ShowModal();
}

void Frame::onCodeLove2d(wxCommandEvent& event)
{
 	ebuilder::CodeDialog dlg(this);
 
 	ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, wxT("main.lua"));
 
 	ebuilder::CodeGenerator gen;
  	Love2dCode code(gen);
  	code.resolve();
 	page->SetReadOnly(false);
 	page->SetText(gen.toText());
 	page->SetReadOnly(true);
 
 	dlg.notebook->AddPage(page, page->getName());
 
 	dlg.ShowModal();
}

void Frame::onSettings(wxCommandEvent& event)
{
	d2d::SettingsDialog dlg(this);
	dlg.ShowModal();
}

void Frame::onSetBackground(wxCommandEvent& event)
{
	wxString formatFilter = wxT("*.png;*.jpg");
	wxFileDialog dlg(this, wxT("Choose Background Image"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString filename = dlg.GetPath();
		d2d::Image* img = d2d::ImageMgr::Instance()->getItem(filename);
		d2d::GLCanvas* canvas = static_cast<StagePanel*>(Context::Instance()->stage)->getCanvas();
		static_cast<StageCanvas*>(canvas)->setBackground(img);
	}
}

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	menuBar->Append(initViewBar(), "&View");
	menuBar->Append(initSettingsBar(), "&Settings");
	menuBar->Append(initCodesBar(), "&Codes");
//	menuBar->Append(initHelpBar(), "&Help");	
	SetMenuBar(menuBar);
}

wxMenu* Frame::initFileBar()
{
	wxMenu* fileMenu = new wxMenu;
 	fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a project"));
 	fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
 	fileMenu->AppendSeparator();
// 	fileMenu->Append(wxID_CLOSE, wxT("&Close\tCtrl+C"), wxT("Close the project"));
 	fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
 	fileMenu->Append(wxID_SAVEAS, wxT("&Save as...\tF11"), wxT("Save to a new file"));
 	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit GameFruits"));
	return fileMenu;
}

wxMenu* Frame::initViewBar()
{
	wxMenu* viewMenu = new wxMenu;

	viewMenu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play the animation"));

	return viewMenu;
}

wxMenu* Frame::initSettingsBar()
{
	wxMenu* settingsMenu = new wxMenu;
	settingsMenu->Append(ID_SETTINGS, wxT("Settings"), wxT("Settings"));
	settingsMenu->Append(ID_BACKGROUND, wxT("Background", wxT("Background")));
	return settingsMenu;
}


wxMenu* Frame::initHelpBar()
{
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About...\tF1"), wxT("Show about dialog"));
	return helpMenu;
}

wxMenu* Frame::initCodesBar()
{
	wxMenu* menu = new wxMenu;
	menu->Append(ID_CODESETTING, wxT("setting..."), wxEmptyString);
	menu->Append(ID_LOVE2D, wxT("love2d"), wxEmptyString);

	return menu;
}

void Frame::initWorkingFrame()
{
	wxSplitterWindow* rightSplit = new wxSplitterWindow(this);
	wxSplitterWindow* leftSplit = new wxSplitterWindow(rightSplit);

	wxWindow* left = createLeftFrame(leftSplit);
	wxWindow* center = createCenterFrame(leftSplit);
	wxWindow* right = createRightFrame(rightSplit);

	leftSplit->SetSashGravity(0.12f);
	leftSplit->SplitVertically(left, center);

	rightSplit->SetSashGravity(0.9f);
	rightSplit->SplitVertically(leftSplit, right);
}

wxWindow* Frame::createLeftFrame(wxWindow* parent)
{
	Context* context = Context::Instance();

	wxSplitterWindow* split = new wxSplitterWindow(parent);

	// library
	d2d::LibraryPanel* library = new d2d::LibraryPanel(split);
	library->addPage(context->imagePage = new d2d::LibraryImagePage(library->getNotebook()));
	library->addPage(new ecomplex::LibraryPage(library->getNotebook()));
	library->addPage(new emesh::LibraryPage(library->getNotebook()));
	library->addPage(new escale9::LibraryPage(library->getNotebook()));
	context->library = library;

	// property
	context->property = new d2d::PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(library, context->property);

	return split;
}

wxWindow* Frame::createCenterFrame(wxWindow* parent)
{
	Context* context = Context::Instance();

	wxSplitterWindow* bottomSplit = new wxSplitterWindow(parent);
	wxSplitterWindow* topSplit = new wxSplitterWindow(bottomSplit);

	// stage
	StagePanel* stage = new StagePanel(topSplit, this);
	context->stage = stage;
	static_cast<d2d::LibraryPanel*>(context->library)
		->setCanvas(stage->getCanvas());

	// toolbar
	context->toolbar = new ToolbarPanel(topSplit, stage, false);

	// timeline
	TimeLinePanel* timeline = new TimeLinePanel(bottomSplit);

	topSplit->SetSashGravity(0.1f);
	topSplit->SplitHorizontally(context->toolbar, stage);

	bottomSplit->SetSashGravity(0.9f);
	bottomSplit->SplitHorizontally(topSplit, timeline);

	return bottomSplit;
}

wxWindow* Frame::createRightFrame(wxWindow* parent)
{
	Context* context = Context::Instance();

	// viewlist
	StagePanel* stage = static_cast<StagePanel*>(context->stage);
	d2d::PropertySettingPanel* property = static_cast<d2d::PropertySettingPanel*>(context->property);
	context->viewlist = new d2d::ViewlistPanel(parent, stage, stage, property);

	return context->viewlist;
}

void Frame::clear()
{
	Context* context = Context::Instance();
	static_cast<d2d::LibraryPanel*>(context->library)->clear();
	static_cast<StagePanel*>(context->stage)->clear();
	context->layers.clear();

//	d2d::Statistics::PrintRes();
}
// 
// void Frame::refresh()
// {
// 	Context* context = Context::Instance();
// 	context->library->Refresh();
// 	context->stage->Refresh();
// 	context->layersPanel->Refresh();
// 	context->keysPanel->Refresh();
// }

void Frame::setCurrFilename()
{
	int id = 0;
	while (true)
	{
		wxString str = 
			wxT("new") + 
			wxString::FromDouble(id++) + 
			wxT("_") + 
			FILE_TAG + 
			wxT(".json");

		if (!d2d::FilenameTools::isExist(str))
		{
			m_currFilename = str.ToStdString();
			break;
		}
	}
}

} // eanim