#ifndef _EASYDB_BIN_EP_H_
#define _EASYDB_BIN_EP_H_

#include "ICommand.h"

namespace edb
{

class BinEP : public ICommand
{
public:
	BinEP() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new BinEP(); }

private:
	void Trigger(const std::string& filename, const std::string& type, 
		const std::string& output);

}; // BinEP

}

#endif // _EASYDB_BIN_EP_H_