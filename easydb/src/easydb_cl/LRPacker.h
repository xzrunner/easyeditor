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
	void PackEP(const std::string& tmp_dir, const std::string& tmp_lr_file,
		const std::string& out_dir, int LOD);

}; // LRPacker

}

#endif // _EASYDB_LR_PACKER_H_