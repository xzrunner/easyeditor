#ifndef _EASYDB_FORMAT_TID_STRING_H_
#define _EASYDB_FORMAT_TID_STRING_H_

#include "ICommand.h"

namespace edb
{

class FormatTIDString : public ICommand
{
public:
	FormatTIDString() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new FormatTIDString(); }

private:
	void Trigger(const std::string& dir) const;

}; // FormatTIDString

}

#endif // _EASYDB_FORMAT_TID_STRING_H_