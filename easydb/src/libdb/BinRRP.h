#ifndef _EASYDB_BIN_RRP_H_
#define _EASYDB_BIN_RRP_H_

#include "ICommand.h"

namespace edb
{

class BinRRP : public ICommand
{
public:
	BinRRP() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new BinRRP(); }

private:
	void Trigger(const std::string& json_file, const std::string& image_id_file);

}; // BinRRP

}

#endif // _EASYDB_BIN_RRP_H_