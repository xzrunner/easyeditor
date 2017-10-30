#ifndef _EASYEDITOR_LOG_MGR_H_
#define _EASYEDITOR_LOG_MGR_H_

#include <cu/cu_macro.h>

#include <string>

namespace ee
{

class LogMgr
{
public:
	
	void AddException(const std::string& exception) { m_exception += exception; }
	const std::string& GetException() const { return m_exception; }
	void ClearException() { m_exception.clear(); }

private:
	std::string m_exception;

	CU_SINGLETON_DECLARATION(LogMgr)

}; // LogMgr

}

#endif // _EASYEDITOR_LOG_MGR_H_