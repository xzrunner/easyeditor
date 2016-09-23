#ifndef _EASYDB_CHANGE_JSON_FILE_H_
#define _EASYDB_CHANGE_JSON_FILE_H_

#include "ICommand.h"

#include <wx/arrstr.h>

namespace edb
{

class ChangeJsonFile : public ICommand
{
public:
	ChangeJsonFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new ChangeJsonFile(); }

private:
	void Run(const std::string& dir/*, ee::FileFormat type*/);

	void Scale(const std::string& key, float times);

private:
	wxArrayString m_files;

//	ee::FileFormat m_type;

}; // ChangeJsonFile

}

#endif // _EASYDB_CHANGE_JSON_FILE_H_