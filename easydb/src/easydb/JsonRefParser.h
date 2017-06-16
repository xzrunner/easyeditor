#ifndef _EASYDB_JSON_REF_PARSER_H_
#define _EASYDB_JSON_REF_PARSER_H_

#include <json/json.h>

#include <string>
#include <set>

namespace edb
{

class Database;

class JsonRefParser
{
public:
	static void GetOutput(const Database& db, const std::string& filepath, 
		std::set<int>& output);

private:
	static void ParserFile(const Database& db, const std::string& base_dir,
		const Json::Value& val, const std::string& key, std::set<int>& output);
	static void ParserGroup(const Database& db, const std::string& base_dir,
		const Json::Value& val, std::set<int>& output);

	static void LoadJson(const std::string& filepath, Json::Value& val);

	static void ParserCommonArray(const Database& db, const std::string& filepath,
		const std::string& key, std::set<int>& output);
	static void ParserTexture(const Database& db, const std::string& filepath,
		std::set<int>& output);
	static void ParserAnim(const Database& db, const std::string& filepath,
		std::set<int>& output);
	static void ParserMesh(const Database& db, const std::string& filepath,
		std::set<int>& output);	
	static void ParserMask(const Database& db, const std::string& filepath,
		std::set<int>& output);	
	
}; // JsonRefParser

}

#endif // _EASYDB_JSON_REF_PARSER_H_