#pragma once

#include <string>
#include <vector>

#include "IFileAdapter.h"

#include "common/Vector.h"

namespace d2d
{
	class CombinationFileAdapter : public IFileAdapter
	{
	public:
		struct Entry
		{
			std::string filepath;

			Vector pos;
			float angle;
			float scale;
			bool xMirror, yMirror;
		};

	public:
		virtual void load(const char* filename);

	public:
		std::vector<Entry> m_data;

	}; // CombinationFileAdapter
}

