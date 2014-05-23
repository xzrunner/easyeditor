#ifndef _EASYDB_TASK_H_
#define _EASYDB_TASK_H_

#include <string>

namespace edb
{

class ICommand
{
public:
	virtual std::string Command() const = 0;
	virtual std::string Description() const = 0;
	virtual std::string Usage() const = 0;
	virtual void Run(int argc, char *argv[]) = 0;
	virtual ~ICommand() {}

}; // ITask

}

#endif // _EASYDB_TASK_H_