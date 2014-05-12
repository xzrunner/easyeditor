#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>

namespace edb {

class VerifyJsons
{
public:
	VerifyJsons(const std::string& dirpath);

private:
	void InitFiles(const std::string& dirpath);

	void VerifyExport();
	void VerifyConnection();
	void VerifySurplus();
	void Report() const;

 	void HandleSpritePath(const std::string& parent,
		const std::string& child);

private:
	enum Type
	{
		e_null,
		e_used,
		e_exported
	};

	struct Node
	{
		std::string filepath;
		Type type;

		std::set<Node*> parents;
		std::set<Node*> children;

		Node(const std::string& path) 
			: filepath(path), type(e_null) {}

		bool IsExported() const {
			if (type == e_exported)
				return true;

			std::set<Node*>::const_iterator itr = parents.begin();
			for ( ; itr != parents.end(); ++itr)
				if ((*itr)->IsExported())
					return true;

			return false;
		}
	};

private:
	std::vector<std::string> _complex_files;
	std::vector<std::string> _anim_files;

	std::map<std::string, Node*> _map_name2node;

	std::set<std::string> _reports;

}; // VerifyJsons

}