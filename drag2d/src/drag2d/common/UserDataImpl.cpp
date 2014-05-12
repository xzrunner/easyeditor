#include "UserDataImpl.h"

#include <stdlib.h>

namespace d2d
{

UserDataImpl::UserDataImpl() 
{ 
	m_userData = NULL; 
}

UserDataImpl::~UserDataImpl() 
{ 
//	clearUserData(true);
}

void UserDataImpl::setUserData(void* data) 
{ 
	if (m_userData != data)
	{
		clearUserData(true);
		m_userData = data;
	}
}

void* UserDataImpl::getUserData() const 
{
	return m_userData;
}

} // d2d