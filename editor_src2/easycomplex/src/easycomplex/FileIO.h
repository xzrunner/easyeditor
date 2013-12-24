#pragma once

#include <drag2d.h>

namespace ecomplex
{
	class Task;
	class FileIO
	{
	public:
		static void load(const Task* task, const char* filename);
		static void store(const Task* task, const char* filename);

	}; // FileIO
}

