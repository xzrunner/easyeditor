#include "CopyFile.h"
#include "check_params.h"

#include <drag2d.h>

namespace edb
{

std::string CopyFile::Command() const
{
	return "copy";
}

std::string CopyFile::Description() const
{
	return "copy files";
}

std::string CopyFile::Usage() const
{
	std::string usage = Command() + " [src path] [dst path] [options] <parms>";
	usage += "-l, --list \t copy by file list of export names: [file path]";
	return usage;
}

void CopyFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	// todo
	std::string opt = argv[3];
	if (opt != "-l" || opt != "--list") {
		std::cerr << "error opt" << std::endl;
		std::cerr << Usage() << std::endl;
		return;
	}

//	Trigger(argv[2]);

	Init(argv[2], argv[3]);
	if (opt == "-s" || opt == "--scale") {
//		CopyByExportNames(argv[4], atof(argv[5]));
	} else {
		std::cerr << "unknown option" << std::endl;
	}
}

void CopyFile::Init(const std::string& src_dir, const std::string& dst_dir)
{
	m_src_dir = src_dir;
	m_dst_dir = dst_dir;

	d2d::FilenameTools::fetchAllFiles(src_dir, m_files);
}

void CopyFile::CopyByExportNames(const std::set<std::string>& export_names) const
{
	std::set<std::string> files;
	for (int i = 0, n = m_files.size(); i < n; ++i)
	{
		wxFileName filename(m_files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex) ||
			d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			std::string name = value["name"].asString();
			if (export_names.find(name) != export_names.end())
			{
				GetDependFiles(filepath, files);
				files.insert(filepath.ToStdString());
			}
		}
	}

	Copy(files);
}

void CopyFile::GetDependFiles(const wxString& filepath, std::set<std::string>& files) const
{
	if (files.find(filepath.ToStdString()) != files.end()) {
		return;
	}

	if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex))
	{
		Json::Value value;
		Json::Reader reader;
		wxString fullpath = d2d::FilenameTools::getAbsolutePath(m_src_dir, filepath);
		std::locale::global(std::locale(""));
		std::ifstream fin(fullpath.fn_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int i = 0;
		Json::Value spriteValue = value["sprite"][i++];
		while (!spriteValue.isNull()) {
			std::string item_path = spriteValue["filepath"].asString();
			if (d2d::FileNameParser::isType(item_path, d2d::FileNameParser::e_complex)
				|| d2d::FileNameParser::isType(item_path, d2d::FileNameParser::e_anim))
			{
				GetDependFiles(item_path, files);
			}
			files.insert(item_path);
			spriteValue = value["sprite"][i++];
		}	
	}
	else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
	{
		Json::Value value;
		Json::Reader reader;
		wxString fullpath = d2d::FilenameTools::getAbsolutePath(m_src_dir, filepath);
		std::locale::global(std::locale(""));
		std::ifstream fin(fullpath.fn_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int i = 0;
		Json::Value layerValue = value["layer"][i++];
		while (!layerValue.isNull()) {
			int j = 0;
			Json::Value frameValue = layerValue["frame"][j++];
			while (!frameValue.isNull()) {
				int m = 0;
				Json::Value entryValue = frameValue["actor"][m++];
				while (!entryValue.isNull()) {
					std::string item_path = entryValue["filepath"].asString();
					if (d2d::FileNameParser::isType(item_path, d2d::FileNameParser::e_complex)
						|| d2d::FileNameParser::isType(item_path, d2d::FileNameParser::e_anim))
					{
						GetDependFiles(item_path, files);
					}
					files.insert(item_path);
					entryValue = frameValue["actor"][m++];
				}
				frameValue = layerValue["frame"][j++];
			}
			layerValue = value["layer"][i++];
		}
	}
}

void CopyFile::Copy(const std::set<std::string>& files) const
{
	std::set<std::string>::const_iterator itr = files.begin();
	for ( ; itr != files.end(); ++itr)
	{
		wxString filename = d2d::FilenameTools::getFilenameWithExtension(*itr);
		wxString src = m_src_dir + "\\" + filename;
		wxString dst = m_dst_dir + "\\" + filename;
		wxCopyFile(src, dst, true);
	}
}

}