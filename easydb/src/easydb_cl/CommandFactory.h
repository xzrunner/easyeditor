#ifndef _EASYDB_COMMAND_FACTORY_H_
#define _EASYDB_COMMAND_FACTORY_H_

#include "ICommand.h"

#include <vector>

namespace edb
{

class CommandFactory
{
public:
	static CommandFactory* Instance();
	
	void Register(ICommand* cmd);

	const std::vector<ICommand*>& GetAllCmds() const {
		return m_commands;
	}

private:
	CommandFactory() {}

private:
	std::vector<ICommand*> m_commands;

	static CommandFactory* m_instance;

}; // CommandFactory

}

#endif // _EASYDB_COMMAND_FACTORY_H_
