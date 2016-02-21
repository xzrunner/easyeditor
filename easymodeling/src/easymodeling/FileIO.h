#ifndef _EASYMODELING_FILE_IO_H_
#define _EASYMODELING_FILE_IO_H_

#include <easymodeling.h>

#include <json/json.h>

namespace emodeling
{

class FileIO
{
public:
	static void Load(const char* filename);
	static void Store(const char* filename);

private:
	static Json::Value B2J(const World* world);
	static Json::Value B2J(const Body* body, const std::string& dlg);
	static Json::Value B2J(const Fixture* fixture);
	static Json::Value B2J(Joint* joint, 
		const std::map<Body*, int>& bodyIndexMap);

}; // FileIO

}

#endif // _EASYMODELING_FILE_IO_H_
