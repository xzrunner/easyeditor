#pragma once

#include "IFileAdapter.h"

#include "common/Vector.h"

#include <json/json.h>

namespace d2d
{
	class Scale9FileAdapter
	{
	public:
		struct Entry
		{
			std::string filepath;

			std::string name;

			d2d::Vector pos;
			float angle;
			float xscale, yscale;
			float xshear, yshear;
			bool xMirror, yMirror;
		};

	public:
		virtual void load(const char* filename);

	private:
		Entry load(const Json::Value& value);

	public:
		int type;

		float width, height;

		std::vector<Entry> m_data;

	}; // Scale9FileAdapter
}

