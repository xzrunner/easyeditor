#ifndef _EASYDB_DECODE_EJOY2D_RES_H_
#define _EASYDB_DECODE_EJOY2D_RES_H_

#include "ICommand.h"

#include <epbin.h>

#include <vector>

#include <stdint.h>

struct lua_State;

namespace edb
{

class DecodeEjoy2dRes : public ICommand
{
public:
	DecodeEjoy2dRes() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new DecodeEjoy2dRes(); }

private:
	void Run(const std::string& lua_path, const std::string& img_path, 
		const std::string& dst_dir);

}; // DecodeEjoy2dRes

}

#endif // _EASYDB_DECODE_EJOY2D_RES_H_