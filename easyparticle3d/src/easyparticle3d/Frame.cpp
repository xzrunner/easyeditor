#include "Frame.h"
#include "Task.h"

#include <wx/splitter.h>

#include <easyparticle3d.h>

namespace eparticle3d
{

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
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
			int idx = dlg.GetFilterIndex();
			if (idx == 0) {
				SaveAsParticle3d(filename);
			} else if (idx == 1) {
				SaveAsAnim(filename);
			} else if (idx == 2) {
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