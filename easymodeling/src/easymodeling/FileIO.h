
#ifndef EMODELING_FILE_IO_H
#define EMODELING_FILE_IO_H

#include <drag2d.h>
#include <json/json.h>
#include <easymodeling.h>

namespace emodeling
{
	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static Json::Value b2j(const libmodeling::World* world);
		static Json::Value b2j(const libmodeling::Body* body, const std::string& dlg);
		static Json::Value b2j(const libmodeling::Fixture* fixture);
		static Json::Value b2j(libmodeling::Joint* joint, 
			const std::map<libmodeling::Body*, int>& bodyIndexMap);

	}; // FileIO
}

#endif // EMODELING_FILE_IO_H
