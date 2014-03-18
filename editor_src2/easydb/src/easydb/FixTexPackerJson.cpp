#include "FixTexPackerJson.h"

namespace edb
{

FixTexPackerJson::FixTexPackerJson(const std::string& dir)
{
	d2d::FilenameTools::fetchAllFiles(dir, m_files);
}

void FixTexPackerJson::TranslateFrameXY(int dx, int dy)
{
	for (int i = 0, n = m_files.size(); i < n; ++i)
	{
		wxFileName filename(m_files[i]);
		//filename.Normalize();
		wxString filepath = filename.GetFullPath();
		wxString ext = filename.GetExt();
		if (ext != "json") {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::ifstream fin(filepath.fn_str());
		reader.parse(fin, value);
		fin.close();

 		int j = 0;
 		Json::Value itemValue = value["frames"][j++];
 		while (!itemValue.isNull()) {
 			int x = itemValue["frame"]["x"].asInt();
 			int y = itemValue["frame"]["y"].asInt();
   			value["frames"][j-1]["frame"]["x"] = x + dx;
   			value["frames"][j-1]["frame"]["y"] = y + dy;
 			itemValue = value["frames"][j++];
 		}

		Json::StyledStreamWriter writer;
		std::ofstream fout(filepath.fn_str());
		writer.write(fout, value);
		fout.close();
	}
}

}