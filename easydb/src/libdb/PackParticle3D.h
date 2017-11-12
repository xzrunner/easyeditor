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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackParticle3D(); }

private:
	void Trigger(const std::string& dir, const std::string& out);

}; // PackParticle3D

}

#endif // _EASYDB_PACK_PARTICLE3D_H_