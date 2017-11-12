#ifndef _EASYDB_PACK_PARTICLE2D_H_
#define _EASYDB_PACK_PARTICLE2D_H_

#include "ICommand.h"

namespace edb
{

class PackParticle2D : public ICommand
{
public:
	PackParticle2D() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackParticle2D(); }

private:
	void Trigger(const std::string& dir, const std::string& out);

}; // PackParticle2D

}

#endif // _EASYDB_PACK_PARTICLE2D_H_