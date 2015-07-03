#include "FilenameTools.h"

#include "common/tools.h"
#include "common/Exception.h"
#include "common/Config.h"

#include <wx/filename.h>
#include <wx/dir.h>

namespace d2d
{

wxString FilenameTools::getFilenameAddTag(const wxString& filename, const wxString& tag, 
										   const wxString& extension)
{
	wxString fixed;
	int start = filename.find_last_of('_');
	if (start != -1)
	{
		wxString check = filename.substr(start + 1, filename.find_last_of('.') - start - 1);
		if (check == tag)
			fixed = filename;
		else if (filename[0] == '.')
			fixed = filename + wxT("_" + tag + "." + extension);
		else
			fixed = filename.substr(0, filename.find_last_of('.')) + wxT("_" + tag + "." + extension);
	}
	else
	{
		if (filename[0] == '.')
			fixed = filename.substr(0, filename.find_last_of('.')) + wxT("_" + tag + "." + extension);
		else
			fixed = filename + wxT("_" + tag + "." + extension);
	}
	return fixed;
}

wxString FilenameTools::getFilenameTag(const wxString& filepath)
{
	const size_t start = filepath.find_last_of('_') + 1,
		end = filepath.find_last_of('.');
	return filepath.substr(start, end - start);
}

wxString FilenameTools::getFilename(const wxString& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	const size_t start = pos_divide + 1,
		end = filepath.find_last_of('.');
	return filepath.substr(start, end - start);
}

wxString FilenameTools::getFilenameWithExtension(const wxString& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	return filepath.substr(pos_divide + 1);
}

wxString FilenameTools::GetDirName(const wxString& dir)
{
	return getFilenameWithExtension(dir);
}

wxString FilenameTools::getRelativePath(const wxString& dir, const wxString& absolute)
{
	wxFileName filename(absolute);
	filename.MakeRelativeTo(dir);
	return filename.GetFullPath().ToStdString();
}

wxString FilenameTools::getAbsolutePath(const wxString& dir, const wxString& relative)
{
	wxFileName filename(relative);
	filename.MakeAbsolute(dir);
	filename.Normalize();
	wxString filepath = filename.GetFullPath();
 	if (!isExist(filepath))
 		return getExistFilepath(relative, dir);
 	else
		return filepath;
}

wxString FilenameTools::getAbsolutePathFromFile(const wxString& base, const wxString& relative)
{
	wxString dir = d2d::FilenameTools::getFileDir(base);
	return d2d::FilenameTools::getAbsolutePath(dir, relative);
}

wxString FilenameTools::getFilePathExceptExtension(const wxString& filepath)
{
	return filepath.substr(0, filepath.find_last_of('.'));
}

wxString FilenameTools::getExtension(const wxString& filepath)
{
	return filepath.substr(filepath.find_last_of('.') + 1);
}

wxString FilenameTools::getFileDir(const wxString& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	if (pos_divide == -1) {
		return ".";
	} else {
		return filepath.substr(0, pos_divide);
	}
}

bool FilenameTools::isExist(const wxString& filepath)
{
	return wxFileName::FileExists(filepath);
}

wxString FilenameTools::getExistFilepath(const wxString& filepath, const wxString& dir /*= wxEmptyString*/)
{
	wxString filepathFixed = filepath;
	if (!isExist(filepathFixed))
	{
		wxString filename = filepathFixed = getFilenameWithExtension(filepathFixed);
		if (!isExist(filepathFixed))
		{
			wxString cwd = wxFileName::GetCwd();

			Config* cfg = Config::Instance();
			std::set<wxString>::const_iterator 
				itr = cfg->GetResPathes().begin();
			for ( ; itr != cfg->GetResPathes().end(); ++itr)
			{
				filepathFixed = *itr + filename;
				if (isExist(filepathFixed))
					return filepathFixed;

				filepathFixed = cwd + *itr + filename;
				if (isExist(filepathFixed))
					return filepathFixed;
			}
			if (dir != wxEmptyString)
			{
				filepathFixed = dir + filename;
				if (isExist(filepathFixed))
					return filepathFixed;
			}
// 			throw Exception("File: %s don't exist!", filepath.ToStdString().c_str());
			return wxEmptyString;
		}
		else
		{
			return filepathFixed;
		}
	}
	else
	{
		return filepathFixed;
	}
}

void FilenameTools::formatSeparators(std::string& filepath)
{
	const std::string oldVal = "\\", newVal = "/";
	for(std::string::size_type pos(0); pos != std::string::npos; pos += oldVal.length())   
	{   
		if((pos = filepath.find(oldVal, pos)) != std::string::npos)
			filepath.replace(pos, oldVal.length(), newVal);   
		else   
			break;   
	}   
}

void FilenameTools::fetchAllFiles(const std::string& dirpath, wxArrayString& files)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files) 
			: m_files(files) {}

		virtual wxDirTraverseResult OnFile(const wxString& filename)
		{
			m_files.Add(filename);
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname)
		{
			return wxDIR_CONTINUE;
		}

	private:
		wxArrayString& m_files;

	}; // DirTraverser

	DirTraverser traverser(files);

	wxDir dir(dirpath);
	dir.Traverse(traverser);
}

void FilenameTools::fetchAllFiles(const std::string& dirpath, wxArrayString& files, FileNameParser::Type type)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files, FileNameParser::Type type) 
			: files(files), type(type) {}

		virtual wxDirTraverseResult OnFile(const wxString& filename)
		{
			if (FileNameParser::isType(filename, type)) {
				files.Add(filename);
			}
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname)
		{
			return wxDIR_CONTINUE;
		}

	private:
		wxArrayString& files;
		FileNameParser::Type type;

	}; // DirTraverser

	DirTraverser traverser(files, type);

	wxDir dir(dirpath);
	dir.Traverse(traverser);
}

void FilenameTools::FetchCurrDirs(const std::string& dirpath, wxArrayString& dirs)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files) 
			: m_files(files) {}

		virtual wxDirTraverseResult OnFile(const wxString& filename)
		{
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname)
		{
			m_files.Add(dirname);
			return wxDIR_IGNORE;
		}

	private:
		wxArrayString& m_files;

	}; // DirTraverser

	DirTraverser traverser(dirs);

	wxDir dir(dirpath);
	dir.Traverse(traverser);
}

wxString FilenameTools::FormatFilepath(const wxString& filepath)
{
 	wxFileName filename(filepath);
 	filename.Normalize();
	return filename.GetFullPath().Lower();
}

} // d2d