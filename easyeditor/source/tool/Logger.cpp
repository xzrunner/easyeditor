#include "Logger.h"

namespace ee
{

Logger::Logger(const std::string& filepath)
	: m_fout(filepath.c_str(), std::ios_base::app)
{
}

Logger::~Logger()
{
	m_fout.close();
}

void Logger::Print(const std::string& str)
{
	m_fout << str;
}

}