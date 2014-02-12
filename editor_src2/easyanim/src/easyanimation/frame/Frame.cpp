#include <wx/splitter.h>

#include <easybuilder.h>

#include "Frame.h"
#include "FileIO.h"
#include "CodeSettingDlg.h"

#include "Love2dCode.h"

#include "frame/Context.h"
#include "view/LibraryPanel.h"
#include "view/StagePanel.h"
#include "view/TimeLinePanel.h"
#include "view/ToolbarPanel.h"
#include "view/PreviewDialog.h"

#include <easycomplex.h>

namespace eanim
{

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

	EVT_MENU(wxID_EXIT, Frame::onQuit)
	EVT_MENU(ID_PREVIEW, Frame::onPreview)
//	EVT_MENU(wxID_HELP, Frame::onAbout)

	EVT_MENU(ID_CODESETTING, Frame::onCodeSetting)
	EVT_MENU(ID_LOVE2D, Frame::onCodeLove2d)
END_EVENT_TABLE()

static const wxString FILE_TAG = wxT("anim");

Frame::Frame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	setCurrFilename();
	initMenuBar();
	initWorkingFrame();
}

void Frame::initWithFile(const wxString& path)
{
	StagePanel* stage = static_cast<StagePanel*>(Context::Instance()->stage);
	stage->getCanvas()->SetCurrentCanvas();
	
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

void Frame::onNew(wxCommandEvent& event)
{
	setCurrFilename();
	clear();
	Refresh();
}

void Frame::onOpen(wxCommandEvent& event)
{
	wxString filter = wxString("DOMDocument.xml") + ";" 
		+ wxString(wxT("*_") + FILE_TAG + wxT(".json"));
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
	if (!m_currFilename.empty())
	{
		SetTitle(d2d::FilenameTools::getFilename(m_currFilename));
		FileIO::store(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
 	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString,
		wxT("*_") + FILE_TAG + wxT(".json"), wxFD_SAVE);
 	if (dlg.ShowModal() == wxID_OK)
 	{
		wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), FILE_TAG, "json");
		m_currFilename = fixed;
		FileIO::store(m_currFilename);
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

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	menuBar->Append(initViewBar(), "&View");
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
 	fileMenu->Append(wxID_SAVEAS, wxT("&Save as...\tF11", wxT("Save to a new file")));
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
	Context* context = Context::Instance();

	wxSplitterWindow* rootSplitter = new wxSplitterWindow(this);
	wxSplitterWindow* leftSplitter = new wxSplitterWindow(rootSplitter);
	wxSplitterWindow* rightSplitter = new wxSplitterWindow(rootSplitter);
	rootSplitter->SetSashGravity(0.15f);
	rootSplitter->SplitVertically(leftSplitter, rightSplitter);

	d2d::LibraryPanel* library = new d2d::LibraryPanel(leftSplitter);
	context->library = library;
	library->addPage(context->imagePage = new d2d::LibraryImagePage(library->getNotebook()));
	library->addPage(new complex::LibraryPage(library->getNotebook()));
	library->addPage(new d2d::Library9PatchPage(library->getNotebook()));
	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(leftSplitter);
	context->property = property;
	leftSplitter->SetSashGravity(0.65f);
	leftSplitter->SplitHorizontally(library, property);

	wxSplitterWindow* stageSplitter = new wxSplitterWindow(rightSplitter);
	StagePanel* stage = new StagePanel(stageSplitter, this);
	context->stage = stage;
	context->toolbar = new ToolbarPanel(stageSplitter, stage);
	stageSplitter->SetSashGravity(0.9f);
	stageSplitter->SplitVertically(context->stage, context->toolbar);

	TimeLinePanel* timeline = new TimeLinePanel(rightSplitter);
	rightSplitter->SetSashGravity(0.8f);
	rightSplitter->SplitHorizontally(stageSplitter, timeline);
}

void Frame::clear()
{
	Context* context = Context::Instance();
	static_cast<d2d::LibraryPanel*>(context->library)->clear();
	static_cast<StagePanel*>(context->stage)->clear();
	context->layers.clear();
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