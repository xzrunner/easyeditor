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
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new BinEP(); }

private:
	void Trigger(const std::string& dir, const std::string& filename,
		const std::string& type, const std::string& output);

}; // BinEP

}

#endif // _EASYDB_BIN_EP_H_