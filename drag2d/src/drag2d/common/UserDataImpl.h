#ifndef _DRAG2D_USER_DATA_IMPL_H_
#define _DRAG2D_USER_DATA_IMPL_H_

namespace d2d
{

class UserDataImpl
{
public:
	UserDataImpl();
	virtual ~UserDataImpl();

	virtual void ClearUserData(bool deletePtr) = 0;

	void SetUserData(void* data);
	void* GetUserData() const;

protected:
	void* m_userData;

}; // UserDataImpl

}

#endif // _DRAG2D_USER_DATA_IMPL_H_