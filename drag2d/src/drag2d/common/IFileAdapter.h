#pragma once

namespace d2d
{
	class IFileAdapter
	{
	public:
		virtual ~IFileAdapter() {}

		virtual void load(const char* filename) {}
		virtual void store(const char* filename) {}

	}; // IFileAdapter
}

