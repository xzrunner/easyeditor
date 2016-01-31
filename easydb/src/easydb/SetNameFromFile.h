#ifndef _EASYDB_SET_NAME_FROM_FILE_H_
#define _EASYDB_SET_NAME_FROM_FILE_H_

#include "ICommand.h"

#include <wx/wx.h>

namespace edb
{

class SetNameFromFile : public ICommand
{
public:
	SetNameFromFile();

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new SetNameFromFile(); }

private:
	void AddNameFromFile(const std::string& dir) const;

	void AddName(const wxString& filename) const;

private:
	bool m_do_complex;

}; // SetNameFromFile

}

#endif // _EASYDB_SET_NAME_FROM_FILE_H_