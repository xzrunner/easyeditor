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
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new BinRRR(); }

private:
	void Trigger(const std::string& src_dir, const std::string& img_id_file,
		const std::string& dst_file, const std::string& tex_format);

}; // BinRRR

}

#endif // _EASYDB_BIN_RRR_H_