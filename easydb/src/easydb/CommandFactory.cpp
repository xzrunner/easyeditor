#include "CommandFactory.h"

namespace edb
{

CommandFactory* CommandFactory::m_instance = NULL;

CommandFactory* CommandFactory::Instance()
{
	if (!m_instance)
	{
		m_instance = new CommandFactory();
	}
	return m_instance;
}

void CommandFactory::Register(ICommand* cmd)
{
	m_commands.push_back(cmd);
}

}