#ifndef _EASYDB_FORMAT_JSON_FILE_H_
#define _EASYDB_FORMAT_JSON_FILE_H_

#include "ICommand.h"

namespace edb
{

class FormatJsonFile : public ICommand
{
public:
	FormatJsonFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new FormatJsonFile(); }

private:
	void Trigger(const std::string& dir) const;

}; // FormatJsonFile

}

#endif // _EASYDB_FORMAT_JSON_FILE_H_