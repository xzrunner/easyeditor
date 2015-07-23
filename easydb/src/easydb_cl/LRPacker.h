#ifndef _EASYDB_LR_PACKER_H_
#define _EASYDB_LR_PACKER_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class LRPacker : public ICommand
{
public:
	LRPacker() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRPacker(); }

private:
	void Run(const std::string& filepath);

}; // LRPacker

}

#endif // _EASYDB_LR_PACKER_H_