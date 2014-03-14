#include "FixJsonFiles.h"

namespace edb
{

FixJsonFiles::FixJsonFiles(const std::string& dir, d2d::FileNameParser::Type type)
	: m_type(type)
{
	d2d::FilenameTools::fetchAllFiles(dir, m_files);
}

void FixJsonFiles::Scale(const std::string& key, float times)
{
	for (int i = 0, n = m_files.size(); i < n; ++i)
	{
		wxFileName filename(m_files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, m_type))
		{
			Json::Value value;
			Json::Reader reader;
			std::ifstream fin(filepath.fn_str());
			reader.parse(fin, value);
			fin.close();

			float old = value[key].asDouble();
			value[key] = (int)old * times;

			Json::StyledStreamWriter writer;
			std::ofstream fout(filepath.fn_str());
			writer.write(fout, value);
			fout.close();
		}
	}
}

}