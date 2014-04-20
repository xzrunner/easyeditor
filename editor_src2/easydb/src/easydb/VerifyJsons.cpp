#include "VerifyJsons.h"

#include <wx/wx.h>
#include <drag2d.h>

namespace edb {
	
VerifyJsons::VerifyJsons(const std::string& dirpath)
{
	InitFiles(dirpath);

	VerifyExport();
	VerifyConnection();
	VerifySurplus();
	Report();
}

void VerifyJsons::InitFiles(const std::string& dirpath)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dirpath, files);

	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		std::string name = d2d::FilenameTools::getFilenameWithExtension(filepath).ToStdString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			_complex_files.push_back(filepath.ToStdString());
			_map_name2node.insert(std::make_pair(name, new Node(filepath.ToStdString())));
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			_anim_files.push_back(filepath.ToStdString());
			_map_name2node.insert(std::make_pair(name, new Node(filepath.ToStdString())));
		}
	}
}

void VerifyJsons::VerifyExport()
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

void VerifyJsons::VerifyConnection()
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

void VerifyJsons::VerifySurplus()
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

void VerifyJsons::Report() const
{
	if (_reports.empty()) {
		std::cout << "Jsons OK!" << std::endl;
	} else {
		std::set<std::string>::const_iterator itr = _reports.begin();
		for ( ; itr != _reports.end(); ++itr)
			std::cout << *itr << std::endl;
	}
}

void VerifyJsons::HandleSpritePath(const std::string& parent,
								   const std::string& child)
{
 	if (!d2d::FileNameParser::isType(child, d2d::FileNameParser::e_complex) &&
		!d2d::FileNameParser::isType(child, d2d::FileNameParser::e_anim))
 		return;
 
	wxFileName filename(child);
	filename.Normalize();
	std::string childname = d2d::FilenameTools::getFilenameWithExtension(filename.GetFullPath());
	StringTools::toLower(childname);
	
	std::map<std::string, Node*>::iterator itr = _map_name2node.find(childname);
	if (itr == _map_name2node.end()) {
		_reports.insert("Lack Json " + child + "!");
	} else {
		std::string parentname = d2d::FilenameTools::getFilenameWithExtension(parent);
		std::map<std::string, Node*>::iterator itr_p = _map_name2node.find(parentname);
		assert(itr_p != _map_name2node.end());
		itr_p->second->children.insert(itr->second);
		itr->second->parents.insert(itr_p->second);
	}
}

}