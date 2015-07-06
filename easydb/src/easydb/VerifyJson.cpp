#include "VerifyJson.h"
#include "check_params.h"

#include <wx/wx.h>
#include <drag2d.h>

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

void VerifyJson::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
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
 	if (!d2d::FileNameParser::isType(child, d2d::FileNameParser::e_complex) &&
		!d2d::FileNameParser::isType(child, d2d::FileNameParser::e_anim))
 		return;
 
	wxFileName filename(child);
	filename.Normalize();
	std::string childname = d2d::FilenameTools::getFilenameWithExtension(filename.GetFullPath());
	d2d::StringTools::ToLower(childname);
	
	std::map<std::string, Node*>::iterator itr = _map_name2node.find(childname);
	if (itr == _map_name2node.end()) {
		_reports.insert("Lack Json " + child + "!");
	} else {
		std::string parentname = d2d::FilenameTools::getFilenameWithExtension(parent);
		std::map<std::string, Node*>::iterator itr_p = _map_name2node.find(parentname);
		if (itr_p == _map_name2node.end()) {
			std::string str = "HandleSpritePath fail to find \""+parentname+"\"";
			throw d2d::Exception(str.c_str());
		}
		itr_p->second->children.insert(itr->second);
		itr->second->parents.insert(itr_p->second);
	}
}

}