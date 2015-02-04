#ifndef _EASYDB_BIN_B4R_H_
#define _EASYDB_BIN_B4R_H_

#include "ICommand.h"

namespace edb
{

class BinB4R : public ICommand
{
public:
	BinB4R() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new BinB4R(); }

private:
	void Trigger(const std::string& src_dir, const std::string& img_id_file, 
		const std::string& dst_file);

}; // BinB4R

}

#endif // _EASYDB_BIN_B4R_H_