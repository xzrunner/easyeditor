#include "FormatJsonFile.h"

#include <wx/wx.h>
#include <drag2d.h>

namespace edb
{

void FormatJsonFile::implement(const std::string& dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();

		int pos = filepath.rfind('.');
		if (pos == -1) {
			continue;
		}

		std::string ext = filepath.substr(pos);
		if (ext == ".json")
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			Json::StyledStreamWriter writer;
			std::ofstream fout(filepath.fn_str());
			writer.write(fout, value);
			fout.close();
		}
	}
}

}