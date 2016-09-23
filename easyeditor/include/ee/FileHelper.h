#ifndef _EASYEDITOR_FILE_HELPER_H_
#define _EASYEDITOR_FILE_HELPER_H_

#include "FileType.h"

#include <wx/arrstr.h>

#include <string>
#include <vector>

namespace ee
{

class FileHelper
{
public:
	//
	// file system
	//

	static bool MkDir(const std::string& dir, bool rm = true);

	static bool RmFile(const std::string& filepath);

	static bool IsFileExist(const std::string& filepath);
	static bool IsDirExist(const std::string& filepath);

	static void FetchAllFiles(const std::string& dirpath, wxArrayString& files);
	static void FetchAllFiles(const std::string& dirpath, const std::vector<std::string>& ignore_dirs, wxArrayString& files);
	static void FetchAllFiles(const std::string& dirpath, wxArrayString& files, FileFormat type);
	static void FetchCurrDirs(const std::string& dirpath, wxArrayString& dirs);

	//
	// path
	//

	// add _* between end and '.', will not add if already has one
	static std::string GetFilenameAddTag(const std::string& filename, const std::string& tag, 
		const std::string& extension);

	// get string between the last '_' and '.'
	static std::string GetFilenameTag(const std::string& filepath);

	// get string between the last '\' and '.'
	static std::string GetFilename(const std::string& filepath);

	// get string after the last '\'
	static std::string GetFilenameWithExtension(const std::string& filepath);

	// get path absolute - dir
	static std::string GetRelativePath(const std::string& dir, const std::string& absolute);

	// get path dir + relative
	static std::string GetAbsolutePath(const std::string& dir, const std::string& relative);
	static std::string GetAbsolutePath(const std::string& filepath);

	// get path dir(base) + relative
	static std::string GetAbsolutePathFromFile(const std::string& base, const std::string& relative);

	// get string from begin to '.'
	static std::string GetFilePathExceptExtension(const std::string& filepath);

	// get string after '.'
	static std::string GetExtension(const std::string& filepath);

	// get string from begin to the last '\'
	static std::string GetFileDir(const std::string& filepath);

	static std::string GetExistFilepath(const std::string& filepath, const std::string& dir = "");

	// use '\' replace '/'
	static void FormatSeparators(std::string& filepath);

	static std::string FormatFilepath(const std::string& filepath);
	static std::string FormatFilepathAbsolute(const std::string& filepath);

}; // FileHelper

}

#endif // _EASYEDITOR_FILE_HELPER_H_