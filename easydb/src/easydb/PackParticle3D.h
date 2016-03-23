#ifndef _EASYDB_PACK_PARTICLE3D_H_
#define _EASYDB_PACK_PARTICLE3D_H_

#include "ICommand.h"

namespace edb
{

class PackParticle3D : public ICommand
{
public:
	PackParticle3D() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackParticle3D(); }

private:
	void Trigger(const std::string& dir, const std::string& out);

}; // PackParticle3D

}

#endif // _EASYDB_PACK_PARTICLE3D_H_