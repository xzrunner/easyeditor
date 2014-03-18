#ifndef _EASYDB_COPY_FILES_H_
#define _EASYDB_COPY_FILES_H_

#include <string>
#include <set>
#include <wx/wx.h>

namespace edb
{

class CopyFiles
{
public:
	CopyFiles(const std::string& src_dir, const std::string& dst_dir);

	void CopyByExportNames(const std::set<std::string>& export_names) const;

private:
	void GetDependFiles(const wxString& filepath, std::set<std::string>& files) const;

	void Copy(const std::set<std::string>& files) const;

private:
	wxArrayString m_files;

	std::string m_src_dir;
	std::string m_dst_dir;

}; // CopyFiles

}

#endif // _EASYDB_COPY_FILES_H_