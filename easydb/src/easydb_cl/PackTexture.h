#ifndef _EASYDB_PACK_TEXTURE_H_
#define _EASYDB_PACK_TEXTURE_H_

#include "ICommand.h"

namespace edb
{

class PackTexture : public ICommand
{
public:
	PackTexture() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackTexture(); }

	void Trigger(const std::string& cfg_file);

}; // PackTexture

}

#endif // _EASYDB_PACK_TEXTURE_H_