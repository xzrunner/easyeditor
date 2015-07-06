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

void Frame::onSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		wxString anim_filter = GetJsonFileFilter(d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anim));
		wxString inv_filter = GetJsonFileFilter(d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_p3dinv));
		wxString filter = GetFileFilter() + "|" + anim_filter + "|" + inv_filter;
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			wxString filename = dlg.GetPath();
			if (d2d::FileNameParser::isType(filename, d2d::FileNameParser::e_particle3d)) {
				SaveAsParticle3d(filename);
			} else if (d2d::FileNameParser::isType(filename, d2d::FileNameParser::e_anim)) {
				SaveAsAnim(filename);
			} else if (d2d::FileNameParser::isType(filename, d2d::FileNameParser::e_p3dinv)) {
				SaveAsInvert(filename);
			} else {
				throw d2d::Exception("error filepath %s", filename.ToStdString().c_str());
			}
		}
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
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

void Frame::SaveAsParticle3d(const wxString& filepath) const
{
	wxString fixed = d2d::FilenameTools::getFilenameAddTag(filepath, m_filetag, "json");
	m_curr_filename = fixed;
	m_task->Store(fixed);
}

void Frame::SaveAsAnim(const wxString& filepath) const
{
	static_cast<Task*>(m_task)->StoreAsAnim(filepath);
}

void Frame::SaveAsInvert(const wxString& filepath) const
{
	static_cast<Task*>(m_task)->StoreAsInvert(filepath);	
}

}