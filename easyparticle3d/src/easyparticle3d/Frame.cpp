#include "Frame.h"
#include "Task.h"

#include <wx/splitter.h>

#include <easybuilder.h>
#include <easyparticle3d.h>

namespace eparticle3d
{

enum MenuID
{
	ID_COC = 700,
};

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_COC, Frame::onCodeCOC)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_code_menu->Append(ID_COC, wxT("Info\tCtrl+C"), wxT("code"));
}

void Frame::onCodeCOC(wxCommandEvent& event)
{
	ebuilder::CodeDialog dlg(this);

	ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, wxT("particle.lua"));

	ebuilder::CodeGenerator gen;
	Code code(gen, static_cast<Task*>(m_task)->getToolbar());
	code.resolve();
	page->SetReadOnly(false);
	page->SetText(gen.toText());
	page->SetReadOnly(true);

	dlg.notebook->AddPage(page, page->getName());

	dlg.ShowModal();
}

}