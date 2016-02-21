#include "Frame.h"
#include "Task.h"

#include <ee/FileType.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/FileHelper.h>

#include <easyparticle3d.h>

#include <wx/splitter.h>

namespace eparticle3d
{

Frame::Frame(const std::string& title, const std::string& filetag)
	: ee::Frame(title, filetag)
{
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		std::string anim_filter = GetJsonFileFilter(ee::FileType::GetTag(ee::FileType::e_anim));
		std::string inv_filter = GetJsonFileFilter(ee::FileType::GetTag(ee::FileType::e_p3dinv));
		std::string filter = GetFileFilter() + "|" + anim_filter + "|" + inv_filter;
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			std::string filename = dlg.GetPath();
			int idx = dlg.GetFilterIndex();
			if (idx == 0) {
				SaveAsParticle3d(filename);
			} else if (idx == 1) {
				SaveAsAnim(filename);
			} else if (idx == 2) {
				SaveAsInvert(filename);
			} else {
				throw ee::Exception("error filepath %s", filename.c_str());
			}
		}
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::SaveAsParticle3d(const std::string& filepath) const
{
	std::string fixed = ee::FileHelper::GetFilenameAddTag(filepath, m_filetag, "json");
	m_curr_filename = fixed;
	m_task->Store(fixed.c_str());
}

void Frame::SaveAsAnim(const std::string& filepath) const
{
	static_cast<Task*>(m_task)->StoreAsAnim(filepath);
}

void Frame::SaveAsInvert(const std::string& filepath) const
{
	static_cast<Task*>(m_task)->StoreAsInvert(filepath);	
}

}