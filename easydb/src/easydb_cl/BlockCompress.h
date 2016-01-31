#ifndef _EASYDB_BLOCK_COMPRESS_H_
#define _EASYDB_BLOCK_COMPRESS_H_

#include "ICommand.h"

namespace edb
{

class BlockCompress : public ICommand
{
public:
	BlockCompress() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new BlockCompress(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_dir) const;

}; // BlockCompress
}

#endif // _EASYDB_BLOCK_COMPRESS_H_