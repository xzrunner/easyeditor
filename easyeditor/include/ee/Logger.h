#ifndef _EASYEDITOR_LOGGER_H_
#define _EASYEDITOR_LOGGER_H_

#include <string>
#include <fstream>

namespace ee
{

class Logger
{
public:
	Logger(const std::string& filepath);
	~Logger();

	void Print(const std::string& str);

private:
	std::ofstream m_fout;

}; // Logger

}

#endif // _EASYEDITOR_LOGGER_H_