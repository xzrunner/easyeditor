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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new LRPackerNew(); }

private:
	void PackEP(const std::string& tmp_dir, const std::string& tmp_lr_file,
		const std::string& out_dir, int LOD, const std::string& fmt, int pkg_id, int pkg_id_base);

	void PackBaseEP(const std::string& tmp_dir, const std::string& tmp_lr_file,
		const std::string& out_dir, int LOD, const std::string& fmt, int pkg_id);	
	void PackOthersEP(const std::string& tmp_dir, const std::string& tmp_lr_file,
		const std::string& out_dir, int LOD, const std::string& fmt, int pkg_id);	

	void ClearCached();

}; // LRPackerNew

}

#endif // _EASYDB_LR_PACKER_NEW_H_