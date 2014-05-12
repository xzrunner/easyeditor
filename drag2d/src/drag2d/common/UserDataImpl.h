#pragma once

namespace d2d
{
	class UserDataImpl
	{
	public:
		UserDataImpl();
		virtual ~UserDataImpl();

		virtual void clearUserData(bool deletePtr) = 0;

		void setUserData(void* data);
		void* getUserData() const;

	protected:
		void* m_userData;

	}; // UserDataImpl
}

