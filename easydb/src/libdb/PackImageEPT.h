#ifndef _EASYDB_PACK_IMAGE_EPT_H_
#define _EASYDB_PACK_IMAGE_EPT_H_

#include "ICommand.h"

#include <easyrespacker.h>

namespace edb
{

class PackImageEPT : public ICommand
{
public:
	PackImageEPT() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackImageEPT(); }

	void Trigger(const std::string& src, const std::string& dst, int type);

}; // PackImageEPT

}

#endif // _EASYDB_PACK_IMAGE_EPT_H_