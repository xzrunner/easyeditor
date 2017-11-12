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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransToETC2(); }

private:
	void Trigger(const std::string& path);

	void Format(const std::string& filepath);

}; // TransToETC2

}

#endif // _EASYDB_TRANS_TO_ETC2_H_