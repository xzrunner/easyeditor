#ifndef _EASYDB_TRANS_TO_ETC2_H_
#define _EASYDB_TRANS_TO_ETC2_H_

#include "ICommand.h"

#include <wx/string.h>

namespace edb
{

class TransToETC2 : public ICommand
{
public:
	TransToETC2() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new TransToETC2(); }

private:
	void Trigger(const std::string& path);

	void Format(const std::string& filepath);

}; // TransToETC2

}

#endif // _EASYDB_TRANS_TO_ETC2_H_