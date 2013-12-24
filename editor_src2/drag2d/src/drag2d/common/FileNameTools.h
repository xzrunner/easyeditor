#pragma once

#include <wx/wx.h>

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

		// get path absolute - dir
		static wxString getRelativePath(const wxString& dir, const wxString& absolute);

		// get path dir + relative
		static wxString getAbsolutePath(const wxString& dir, const wxString& relative);

		// get string from begin to '.'
		static wxString getFilePathExceptExtension(const wxString& filepath);

		// get string after '.'
		static wxString getExtension(const wxString& filepath);

		// get string from begin to the last '\'
		static wxString getFileDir(const wxString& filepath);

		//////////////////////////////////////////////////////////////////////////

		static bool isExist(const wxString& filepath);

		static wxString getExistFilepath(const wxString& filepath, const wxString& dir = wxEmptyString);

		// use '/' replace '\'
		static void formatSeparators(std::string& filepath);

		static void fetchAllFiles(const std::string& dirpath, wxArrayString& files);

	}; // FilenameTools
}

