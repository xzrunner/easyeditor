
#include "StagePanel.h"
#include "LibraryPanel.h"

#include "view/Context.h"
#include "view/SceneItem.h"
#include "frame/Settings.h"
#include "output/DesignerPage.h"
#include "output/love2d/Builder.h"
#include "output/easy2d/Builder.h"
#include "output/libgdx/Builder.h"
#include "output/cocos2d-x/Builder.h"

using namespace ebuilder;

BEGIN_EVENT_TABLE(StagePanel, wxPanel)
	EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, StagePanel::onPageChanged)
END_EVENT_TABLE()

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: wxPanel(parent)
	, m_frame(frame)
	, m_notebook(NULL)
	, m_designerPage(NULL)
{
	initLayout();
}

void StagePanel::rebuildCodePages()
{
	int count = m_notebook->GetPageCount();
	for (size_t i = 1; i < count; ++i)
		m_notebook->DeletePage(1);

	CodePageBuilder* builder = getCodePageBuilder();
	if (builder) builder->loadPages();
	delete builder;
}

void StagePanel::updateCodePages(SceneItem* scene) const
{
	CodePageBuilder* builder = getCodePageBuilder();

	if (!scene)
	{
		std::vector<SceneItem*> scenes;
		Context::Instance()->library->getSceneList()->traverse(
			ee::FetchAllVisitor<SceneItem>(scenes));
		for (size_t i = 0, n = scenes.size(); i < n; ++i)
			builder->updatePage(*scenes[i]);
	}
	else
	{
		builder->updatePage(*scene);
	}

	delete builder;
}

void StagePanel::addCodePage(SceneItem* scene)
{
	CodePageBuilder* builder = getCodePageBuilder();
	builder->insertPage(*scene);
	delete builder;
}

void StagePanel::removeCodePage(SceneItem* scene)
{
	if (!scene)
	{
		int count = m_notebook->GetPageCount();
		for (size_t i = 1; i < count; ++i)
			m_notebook->DeletePage(1);
	}
	else
	{
		CodePageBuilder* builder = getCodePageBuilder();
		builder->removePage(*scene);
		delete builder;
	}
}

void StagePanel::storeAllCodePages(const wxString& dir) const
{
	for (size_t i = 1, n = m_notebook->GetPageCount(); i < n; ++i)
	{
		CodePage* page = static_cast<CodePage*>(m_notebook->GetPage(i));

		wxString filepath = dir + "\\" + page->getName();
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.fn_str());
		std::locale::global(std::locale("C"));	
		fout << page->GetText();
		fout.close();
	}
}

void StagePanel::clear()
{
	m_designerPage->clear();
	removeCodePage(NULL);
	Refresh();
}

void StagePanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	initTitle(sizer);
	initTabPages(sizer);
	SetSizer(sizer);
}

void StagePanel::initTitle(wxSizer* sizer)
{
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("  Stage"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);
}

void StagePanel::initTabPages(wxSizer* sizer)
{
 	m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);

 	m_notebook->AddPage(m_designerPage = new DesignerPage(m_notebook, m_frame), wxT("Designer"));

	rebuildCodePages();

 	sizer->Add(m_notebook, 1, wxEXPAND);
}

void StagePanel::onPageChanged(wxNotebookEvent& event)
{
//  	if (m_notebook->GetSelection() != 0)
//  	{
//  		IOutputPage* src = static_cast<IOutputPage*>(m_notebook->GetPage(0));
//  		IOutputPage* dst = static_cast<IOutputPage*>(m_notebook->GetCurrentPage());
//  		InterimData interim;
//  		src->storeToInterimData(interim);
//  		dst->loadFromInterimData(interim);
//  	}
}

CodePageBuilder* StagePanel::getCodePageBuilder() const
{
	CodePageBuilder* builder = NULL;

	switch (Settings::code)
	{
	case Settings::e_love2d:
		builder = new love2d::Builder(m_notebook);
		break;
	case Settings::e_easy2d:
		builder = new e2d::Builder(m_notebook);
		break;
	case Settings::e_libgdx:
		builder = new libgdx::Builder(m_notebook);
		break;
	case Settings::e_cocos2dx:
		builder = new cc::Builder(m_notebook);
		break;
	}

	return builder;
}
