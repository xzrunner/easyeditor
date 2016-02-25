#include "FileHelper.h"
#include "StringHelper.h"
#include "Config.h"

#include <wx/dir.h>
#include <wx/filename.h>
#ifndef _DEBUG
#include <wx/stdpaths.h>
#endif // _DEBUG

#include <set>

namespace ee
{

bool FileHelper::MkDir(const std::string& dir, bool rm)
{
	bool ret = true;
	if (wxDir::Exists(dir)) {
		if (rm) {
			wxFileName::Rmdir(dir, wxPATH_RMDIR_RECURSIVE);
			ret = wxFileName::Mkdir(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		}
	} else {
		ret = wxFileName::Mkdir(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}
	return ret;
}

bool FileHelper::RmFile(const std::string& filepath)
{
	return wxRemoveFile(filepath);
}

bool FileHelper::IsFileExist(const std::string& filepath)
{
	return wxFileName::FileExists(filepath);
}

bool FileHelper::IsDirExist(const std::string& filepath)
{
	return wxFileName::DirExists(filepath);
}

void FileHelper::FetchAllFiles(const std::string& dirpath, wxArrayString& files)
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

void FileHelper::FetchAllFiles(const std::string& dirpath, const std::string& ignore_dir, wxArrayString& files)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files, const wxString& ignore_dir) 
			: m_files(files)
			, m_ignore_dir(ignore_dir)
		{}

		virtual wxDirTraverseResult OnFile(const wxString& filename)
		{
			m_files.Add(filename);
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname)
		{
			if (dirname == m_ignore_dir) {
				return wxDIR_IGNORE;
			} else {
				return wxDIR_CONTINUE;
			}
		}

	private:
		wxArrayString& m_files;
		std::string m_ignore_dir;

	}; // DirTraverser

	DirTraverser traverser(files, ignore_dir);

	wxDir dir(dirpath);
	dir.Traverse(traverser);
}

void FileHelper::FetchAllFiles(const std::string& dirpath, wxArrayString& files, FileType::Type type)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(wxArrayString& files, FileType::Type type) 
			: files(files), type(type) {}

		virtual wxDirTraverseResult OnFile(const wxString& filename)
		{
			if (FileType::IsType(filename.ToStdString(), type)) {
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
		FileType::Type type;

	}; // DirTraverser

	DirTraverser traverser(files, type);

	wxDir dir(dirpath);
	dir.Traverse(traverser);
}

void FileHelper::FetchCurrDirs(const std::string& dirpath, wxArrayString& dirs)
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

std::string FileHelper::GetFilenameAddTag(const std::string& filename, const std::string& tag, 
									   const std::string& extension)
{
	std::string fixed;
	int start = filename.find_last_of('_');
	if (start != -1)
	{
		std::string check = filename.substr(start + 1, filename.find_last_of('.') - start - 1);
		if (check == tag) {
			fixed = filename;
		} else if (filename[0] == '.') {
			fixed = filename + wxT("_" + tag + "." + extension);
		} else {
			fixed = filename.substr(0, filename.find_last_of('.')) + wxT("_" + tag + "." + extension);
		}
	}
	else
	{
		if (filename[0] == '.') {
			fixed = filename.substr(0, filename.find_last_of('.')) + wxT("_" + tag + "." + extension);
		} else {
			int dot_pos = filename.find_last_of('.');
			if (dot_pos == -1) {
				fixed = filename + wxT("_" + tag + "." + extension);
			} else {
				fixed = filename;
				fixed.insert(dot_pos, "_" + tag);
			}
		}
	}
	return fixed;
}

std::string FileHelper::GetFilenameTag(const std::string& filepath)
{
	const size_t start = filepath.find_last_of('_') + 1,
		end = filepath.find_last_of('.');
	return filepath.substr(start, end - start);
}

std::string FileHelper::GetFilename(const std::string& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	const size_t start = pos_divide + 1,
		end = filepath.find_last_of('.');
	return filepath.substr(start, end - start);
}

std::string FileHelper::GetFilenameWithExtension(const std::string& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	return filepath.substr(pos_divide + 1);
}

std::string FileHelper::GetDirName(const std::string& dir)
{
	return GetFilenameWithExtension(dir);
}

std::string FileHelper::GetRelativePath(const std::string& dir, const std::string& absolute)
{
	wxFileName filename(absolute);
	filename.MakeRelativeTo(dir);
	return filename.GetFullPath().Lower().ToStdString();
}

std::string FileHelper::GetAbsolutePath(const std::string& dir, const std::string& relative)
{
	wxFileName filename(relative);
	filename.MakeAbsolute(dir);
	filename.Normalize();
	return filename.GetFullPath().ToStdString();
}

std::string FileHelper::GetAbsolutePath(const std::string& filepath)
{
	wxFileName filename(filepath);
#ifndef _DEBUG
	wxStandardPathsBase& stdp = wxStandardPaths::Get();
	std::string exe_path = stdp.GetExecutablePath();
	filename.MakeAbsolute(GetFileDir(exe_path));
#endif
	filename.Normalize();
	return filename.GetFullPath().ToStdString();
}

std::string FileHelper::GetAbsolutePathFromFile(const std::string& base, const std::string& relative)
{
	std::string dir = FileHelper::GetFileDir(base);
	return FileHelper::GetAbsolutePath(dir, relative);
}

std::string FileHelper::GetFilePathExceptExtension(const std::string& filepath)
{
	return filepath.substr(0, filepath.find_last_of('.'));
}

std::string FileHelper::GetExtension(const std::string& filepath)
{
	return filepath.substr(filepath.find_last_of('.') + 1);
}

std::string FileHelper::GetFileDir(const std::string& filepath)
{
	int pos_divide = std::max((int)filepath.find_last_of('/'), (int)filepath.find_last_of('\\'));
	if (pos_divide == -1) {
		return ".";
	} else {
		return filepath.substr(0, pos_divide);
	}
}

std::string FileHelper::GetExistFilepath(const std::string& filepath, const std::string& dir /*= wxEmptyString*/)
{
	std::string fixed = filepath;
	if (!IsFileExist(fixed))
	{
		std::string filename = fixed = GetFilenameWithExtension(fixed);
		if (!IsFileExist(fixed))
		{
			std::string cwd = wxFileName::GetCwd();

			Config* cfg = Config::Instance();
			std::set<std::string>::const_iterator 
				itr = cfg->GetResPathes().begin();
			for ( ; itr != cfg->GetResPathes().end(); ++itr)
			{
				fixed = *itr + filename;
				if (IsFileExist(fixed))
					return fixed;

				fixed = cwd + *itr + filename;
				if (IsFileExist(fixed))
					return fixed;
			}
			if (dir != wxEmptyString)
			{
				fixed = dir + filename;
				if (IsFileExist(fixed))
					return fixed;
			}
// 			throw Exception("File: %s don't exist!", filepath.ToStdString().c_str());
			return "";
		}
		else
		{
			return fixed;
		}
	}
	else
	{
		return fixed;
	}
}

void FileHelper::FormatSeparators(std::string& filepath)
{
	const std::string oldVal = "/", newVal = "\\";
	for(std::string::size_type pos(0); pos != std::string::npos; pos += oldVal.length())   
	{   
		if((pos = filepath.find(oldVal, pos)) != std::string::npos)
			filepath.replace(pos, oldVal.length(), newVal);   
		else   
			break;   
	}   
}

std::string FileHelper::FormatFilepath(const std::string& filepath)
{
	std::string ret = filepath;
	StringHelper::ToLower(ret);
	FormatSeparators(ret);
	return ret;
}

std::string FileHelper::FormatFilepathAbsolute(const std::string& filepath)
{
	wxFileName filename(FormatFilepath(filepath));
	filename.Normalize();
	return filename.GetFullPath().Lower().ToStdString();
}

}