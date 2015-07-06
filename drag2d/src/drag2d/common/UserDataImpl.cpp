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
//	ClearUserData(true);
}

void UserDataImpl::SetUserData(void* data) 
{ 
	if (m_userData != data)
	{
		ClearUserData(true);
		m_userData = data;
	}
}

void* UserDataImpl::GetUserData() const 
{
	return m_userData;
}

} // d2d