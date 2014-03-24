#pragma once

namespace d2d
{
	class Object
	{
	public:
		Object();
		virtual ~Object();

		int getReferenceCount() const;

		virtual void retain() const;
		virtual void release() const;

	private:
		mutable int m_count;

	}; // Object
}

