#include "VerifyJson.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/Exception.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <fstream>
#include <iostream>

namespace edb 
{
	
std::string VerifyJson::Command() const
{
	return "verify-json";
}

std::string VerifyJson::Description() const
{
	return "check json file's link or surplus";
}

std::string VerifyJson::Usage() const
{
	return Command() + " [dir path]";
}

int VerifyJson::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

void VerifyJson::Trigger(const std::string& dirpath)
{
	InitFiles(dirpath);

	VerifyExport();
	VerifyConnection();
	VerifySurplus();
	Report();
}

void VerifyJson::InitFiles(const std::string& dirpath)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dirpath, files);

	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		std::string name = ee::FileHelper::GetFilenameWithExtension(filepath);
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX:
			_complex_files.push_back(filepath);
			_map_name2node.insert(std::make_pair(name, new Node(filepath)));
			break;
		case s2::SYM_ANIMATION:
			_anim_files.push_back(filepath);
			_map_name2node.insert(std::make_pair(name, new Node(filepath)));
			break;
		}
	}
}

void VerifyJson::VerifyExport()
{
	std::map<std::string, Node*>::iterator itr = _map_name2node.begin();
	for ( ; itr != _map_name2node.end(); ++itr)
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(itr->second->filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		std::string name = value["name"].asString();
		if (!name.empty())
			itr->second->type = e_exported;
	}
}

void VerifyJson::VerifyConnection()
{
	for (size_t i = 0, n = _complex_files.size(); i < n; ++i)
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(_complex_files[i].c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int j = 0;
		Json::Value spriteValue = value["sprite"][j++];
		while (!spriteValue.isNull()) {
			HandleSpritePath(_complex_files[i], spriteValue["filepath"].asString());
			spriteValue = value["sprite"][j++];
		}		
	}
	for (size_t i = 0, n = _anim_files.size(); i < n; ++i)
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(_anim_files[i].c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int j = 0;
		Json::Value layerValue = value["layer"][j++];
		while (!layerValue.isNull()) {
			int k = 0;
			Json::Value frameValue = layerValue["frame"][k++];
			while (!frameValue.isNull()) {
				int m = 0;
				Json::Value entryValue = frameValue["actor"][m++];
				while (!entryValue.isNull()) {
					HandleSpritePath(_anim_files[i], entryValue["filepath"].asString());
					entryValue = frameValue["actor"][m++];
				}
				frameValue = layerValue["frame"][k++];
			}
			layerValue = value["layer"][j++];
		}
	}
}

void VerifyJson::VerifySurplus()
{
	std::map<std::string, Node*>::iterator itr = _map_name2node.begin();
	for ( ; itr != _map_name2node.end(); ++itr)
	{
		Node* node = itr->second;
		if (node->type == e_exported) {
			continue;
		} else {
			if (!node->IsExported())
				_reports.insert("Surplus Json " + node->filepath);
		}
	}
}

void VerifyJson::Report() const
{
	if (_reports.empty()) {
		std::cout << "Jsons OK!" << std::endl;
	} else {
		std::set<std::string>::const_iterator itr = _reports.begin();
		for ( ; itr != _reports.end(); ++itr)
			std::cout << *itr << std::endl;
	}
}

void VerifyJson::HandleSpritePath(const std::string& parent,
								   const std::string& child)
{
	int type = ee::SymbolFile::Instance()->Type(child);
	if (type != s2::SYM_COMPLEX && type != s2::SYM_ANIMATION) {
		return;
	}
 
	std::string childpath = ee::FileHelper::GetAbsolutePath(child);
	std::string childname = ee::FileHelper::GetFilenameWithExtension(childpath);
	ee::StringHelper::ToLower(childname);
	
	std::map<std::string, Node*>::iterator itr = _map_name2node.find(childname);
	if (itr == _map_name2node.end()) {
		_reports.insert("Lack Json " + child + "!");
	} else {
		std::string parentname = ee::FileHelper::GetFilenameWithExtension(parent);
		std::map<std::string, Node*>::iterator itr_p = _map_name2node.find(parentname);
		if (itr_p == _map_name2node.end()) {
			std::string str = "HandleSpritePath fail to find \""+parentname+"\"";
			throw ee::Exception(str.c_str());
		}
		itr_p->second->children.insert(itr->second);
		itr->second->parents.insert(itr_p->second);
	}
}

}