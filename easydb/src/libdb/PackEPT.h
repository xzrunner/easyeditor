#ifndef _EASYDB_PACK_EPT_H_
#define _EASYDB_PACK_EPT_H_

#include "ICommand.h"

namespace edb
{

class PackEPT : public ICommand
{
public:
	PackEPT() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackEPT(); }

	void Trigger(const std::string& tp_json, const std::string& tp_dir, 
		const std::string& out_file);

}; // PackEPT

}

#endif // _EASYDB_PACK_EPT_H_