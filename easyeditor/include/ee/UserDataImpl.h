#ifndef _EASYEDITOR_USER_DATA_IMPL_H_
#define _EASYEDITOR_USER_DATA_IMPL_H_

namespace ee
{

class UserDataImpl
{
public:
	UserDataImpl();
	virtual ~UserDataImpl();

	virtual void ClearUserData(bool del) = 0;

	void SetUserData(void* data);
	void* GetUserData() const;

protected:
	void* m_ud;

}; // UserDataImpl

}

#endif // _EASYEDITOR_USER_DATA_IMPL_H_