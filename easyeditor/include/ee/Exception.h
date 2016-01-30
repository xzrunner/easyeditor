/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#ifndef _EASYEDITOR_EXCEPTION_H_
#define _EASYEDITOR_EXCEPTION_H_

#include <exception>
#include <string>

namespace ee
{

class Exception : public std::exception
{
public:
	Exception(const std::string& msg);
	Exception(const char* fmt, ...);
	virtual ~Exception() throw() {}

	inline virtual const char* What() const throw() { 
		return m_message.c_str(); 
	}

private:
	std::string m_message;

}; // Exception 

////
//// Exceptions
////
//class Exception
//{
//public:
//	virtual std::string what() = 0;
//	virtual ~Exception() {}
//}; // Exception
//
//class IndexOutOfBoundsException : public Exception
//{
//public:
//	IndexOutOfBoundsException(size_t i) { m_error = StringHelper::intToString(i); }
//	virtual ~IndexOutOfBoundsException() {}
//	virtual std::string what() { return "IndexOutOfBoundsException: " + m_error; }
//
//private:
//	std::string m_error;
//}; // IndexOutOfBoundsException
//
//class IllegalArgumentException : public Exception
//{
//public:
//	IllegalArgumentException(std::string s) : m_error(s) {}
//	virtual ~IllegalArgumentException() {}
//	virtual std::string what() { return "IllegalArgumentException: " + m_error; }
//
//private:
//	std::string m_error;
//}; // IllegalArgumentException
//
//class IllegalStateException : public Exception
//{
//public:
//	IllegalStateException(std::string s) : m_error(s) {}
//	virtual ~IllegalStateException() {}
//	virtual std::string what() { return "IllegalStateException: " + m_error + "\nPlease contact "/* + PACKAGE_BUGREPORT*/; }
//
//private:
//	std::string m_error;
//}; // IllegalStateException
//
//class NotSupportedException : public Exception
//{
//public:
//	NotSupportedException(std::string s) : m_error(s) {}
//	virtual ~NotSupportedException() {}
//	virtual std::string what() { return "NotSupportedException: " + m_error; }
//
//private:
//	std::string m_error;
//}; // NotSupportedException

}

#endif // _EASYEDITOR_EXCEPTION_H_
