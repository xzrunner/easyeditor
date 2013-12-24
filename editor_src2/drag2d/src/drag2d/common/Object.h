#pragma once

namespace d2d
{
	class Object
	{
	public:
		Object();
		virtual ~Object();

		int getReferenceCount() const;

		void retain();
		void release();

	private:
		int m_count;

	}; // Object
}

