#pragma once

#include <string>

namespace coceditor
{
	class ExtractImg
	{
	public:
		static void trigger(const std::string& path);

	private:
		static void create(const std::string& src, int pos[], 
			const std::string& dst);

	}; // ExtractImg
}