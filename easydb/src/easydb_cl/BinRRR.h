#ifndef _EASYDB_BIN_RRR_H_
#define _EASYDB_BIN_RRR_H_

#include "ICommand.h"

namespace edb
{

class BinRRR : public ICommand
{
public:
	BinRRR() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new BinRRR(); }

private:
	void Trigger(const std::string& src_dir, const std::string& img_id_file,
		const std::string& dst_file, const std::string& tex_format);

}; // BinRRR

}

#endif // _EASYDB_BIN_RRR_H_