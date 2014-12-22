#ifndef _EPBIN_EXCEPTION_H_
#define _EPBIN_EXCEPTION_H_

#include <exception>
#include <string>

namespace epbin
{

class Exception : public std::exception
{
public:
	Exception(const char* fmt, ...);
	virtual ~Exception() throw() {}

	inline virtual const char* what() const throw() { 
		return m_message.c_str(); 
	}

private:
	std::string m_message;

}; // Exception

}

#endif // _EPBIN_EXCEPTION_H_