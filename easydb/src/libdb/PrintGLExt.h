#ifndef _EASYDB_PRINT_GL_EXT_H_
#define _EASYDB_PRINT_GL_EXT_H_

#include "ICommand.h"

namespace edb
{

class PrintGLExt : public ICommand
{
public:
	PrintGLExt() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new PrintGLExt(); }

}; // PrintGLExt

}

#endif // _EASYDB_PRINT_GL_EXT_H_