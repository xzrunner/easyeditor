#pragma once

#include "IFileAdapter.h"

#include "common/Vector.h"

#include <json/json.h>

namespace d2d
{
	class ComplexFileAdapter
	{
	public:
		struct Entry
		{
			std::string filepath;

			std::string name;
			std::string multiColor, addColor;

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
		std::string m_name;

		std::vector<Entry> m_data;

	}; // ComplexFileAdapter
}

