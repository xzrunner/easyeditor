#ifndef _DRAG2D_FILE_NAME_TOOLS_H_
#define _DRAG2D_FILE_NAME_TOOLS_H_

#include <wx/wx.h>

#include "FileNameParser.h"

namespace d2d
{

class FilenameTools
{
public:
	// add _* between end and '.', will not add if already has one
	static wxString getFilenameAddTag(const wxString& filename, const wxString& tag, 
		const wxString& extension);

	// get string between the last '_' and '.'
	static wxString getFilenameTag(const wxString& filepath);

	// get string between the last '\' and '.'
	static wxString getFilename(const wxString& filepath);

	// get string after the last '\'
	static wxString getFilenameWithExtension(const wxString& filepath);

	// get string after the last '\'
	static wxString GetDirName(const wxString& dir);

	// get path absolute - dir
	static wxString getRelativePath(const wxString& dir, const wxString& absolute);

	// get path dir + relative
	static wxString getAbsolutePath(const wxString& dir, const wxString& relative);

	// get path dir(base) + relative
	static wxString getAbsolutePathFromFile(const wxString& base, const wxString& relative);

	// get string from begin to '.'
	static wxString getFilePathExceptExtension(const wxString& filepath);

	// get string after '.'
	static wxString getExtension(const wxString& filepath);

	// get string from begin to the last '\'
	static wxString getFileDir(const wxString& filepath);

	//////////////////////////////////////////////////////////////////////////

	static bool IsFileExist(const wxString& filepath);
	static bool IsDirExist(const wxString& filepath);

	static wxString getExistFilepath(const wxString& filepath, const wxString& dir = wxEmptyString);

	// use '\' replace '/'
	static void FormatSeparators(std::string& filepath);

	static void fetchAllFiles(const std::string& dirpath, wxArrayString& files);
	static void fetchAllFiles(const std::string& dirpath, wxArrayString& files, FileNameParser::Type type);
	static void FetchCurrDirs(const std::string& dirpath, wxArrayString& dirs);

	static std::string FormatFilepath(const std::string& filepath);
	static std::string FormatFilepathAbsolute(const std::string& filepath);

}; // FilenameTools

}

#endif // _DRAG2D_FILE_NAME_TOOLS_H_