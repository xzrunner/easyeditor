#ifndef _EASYDB_CREATE_COL_LUT_H_
#define _EASYDB_CREATE_COL_LUT_H_

#include "ICommand.h"

#include <stdint.h>

namespace ee { class CreateColLUT; }

namespace edb
{

class CreateColLUT : public ICommand
{
public:
	CreateColLUT() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new CreateColLUT(); }

private:
	void Run(const std::string& filepath) const;

	static void Pixel16To256(uint8_t* dst, int r, int g, int b);
	static uint8_t Int16To256(int i16);

}; // CreateColLUT

}

#endif // _EASYDB_CREATE_COL_LUT_H_