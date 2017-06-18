#ifndef _EASYDB_LR_PACKER_NEW_H_
#define _EASYDB_LR_PACKER_NEW_H_

#include "ICommand.h"

namespace edb
{

class LRPackerNew : public ICommand
{
public:
	LRPackerNew() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRPackerNew(); }

private:
	void PackEP(const std::string& tmp_dir, const std::string& tmp_lr_file,
		const std::string& out_dir, int LOD, const std::string& fmt, int pkg_id);

	void ClearCached();

}; // LRPackerNew

}

#endif // _EASYDB_LR_PACKER_NEW_H_