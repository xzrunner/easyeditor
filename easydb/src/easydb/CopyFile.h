#ifndef _EASYDB_COPY_FILES_H_
#define _EASYDB_COPY_FILES_H_

#include "ICommand.h"

#include <set>
#include <wx/wx.h>

namespace edb
{

class CopyFile : public ICommand
{
public:
	CopyFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new CopyFile(); }

private:
	void Init(const std::string& src_dir, const std::string& dst_dir);

	void CopyByExportNames(const std::set<std::string>& export_names) const;

	void GetDependFiles(const std::string& filepath, std::set<std::string>& files) const;

	void Copy(const std::set<std::string>& files) const;

private:
	wxArrayString m_files;

	std::string m_src_dir;
	std::string m_dst_dir;

}; // CopyFiles

}

#endif // _EASYDB_COPY_FILES_H_