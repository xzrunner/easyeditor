#include "UserDataImpl.h"

#include <stdlib.h>

namespace ee
{

UserDataImpl::UserDataImpl() 
{ 
	m_ud = NULL; 
}

UserDataImpl::~UserDataImpl() 
{ 
//	ClearUserData(true);
}

void UserDataImpl::SetUserData(void* data) 
{ 
	if (m_ud != data)
	{
		ClearUserData(true);
		m_ud = data;
	}
}

void* UserDataImpl::GetUserData() const 
{
	return m_ud;
}

}