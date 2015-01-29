#ifndef _EASYDB_BINARY_RRR_H_
#define _EASYDB_BINARY_RRR_H_

#include "ICommand.h"

namespace edb
{

class BinaryRRR : public ICommand
{
public:
	BinaryRRR() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new BinaryRRR(); }

private:
	void Trigger(const std::string& src_dir, const std::string& img_id_file,
		const std::string& dst_file);

}; // BinaryRRR

}

#endif // _EASYDB_BINARY_RRR_H_