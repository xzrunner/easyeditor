#include <wx/dir.h>
#include <drag2d.h>

#include "../coceditor/ExtrudeImg.h"

//namespace coceditor {

class DirTraverser : public wxDirTraverser
{
public:
	DirTraverser(wxArrayString& files) 
		: _files(files) {}

	virtual wxDirTraverseResult OnFile(const wxString& filename)
	{
		_files.Add(filename);
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnDir(const wxString& dirname)
	{
		return wxDIR_CONTINUE;
	}

private:
	wxArrayString& _files;

}; // DirTraverser

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Need Resource Dir !" << std::endl;
		return 1;
	}

// 	if (argc < 3) {
// 		std::cerr << "Need Resource Dir & Extrude size !" << std::endl;
// 		return 1;
// 	}

	std::string dirpath = argv[1];
	wxArrayString files;
	DirTraverser traverser(files);

	wxDir dir(dirpath);
	dir.Traverse(traverser);

	std::map<std::string, d2d::Rect> map_name2rect;

	double size = 1;
//	wxString(argv[2]).ToDouble(&size);
	try {
		for (size_t i = 0, n = files.size(); i < n; ++i)
		{
			wxFileName filename(files[i]);
			filename.Normalize();
			wxString filepath = filename.GetFullPath();
			
			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
			{
				coceditor::ExtrudeImg extrude(filepath.ToStdString());
				extrude.Trigger((int)size);
				map_name2rect.insert(std::make_pair(extrude.GetFileName(), extrude.GetRectTrimed()));
			}
		}

		for (size_t i = 0, n= files.size(); i < n; ++i)
		{
			wxFileName filename(files[i]);
			filename.Normalize();
			wxString filepath = filename.GetFullPath();

			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex))
			{
				Json::Value value;
				Json::Reader reader;
				std::ifstream fin(filepath.fn_str());
				reader.parse(fin, value);
				fin.close();

				bool dirty = false;

				int i = 0;
				Json::Value spriteValue = value["sprite"][i++];
				while (!spriteValue.isNull()) {

					std::string path = spriteValue["filepath"].asString();
					path = d2d::FilenameTools::getFilenameWithExtension(path);
					std::map<std::string, d2d::Rect>::iterator itr = map_name2rect.find(path);
					if (itr != map_name2rect.end()) {
						dirty = true;

						const float x = spriteValue["position"]["x"].asDouble(),
							y = spriteValue["position"]["y"].asDouble();
						value["sprite"][i-1]["position"]["x"] = x + itr->second.xCenter() * spriteValue["x scale"].asDouble();
						value["sprite"][i-1]["position"]["y"] = y + itr->second.yCenter() * spriteValue["y scale"].asDouble();
					}
					spriteValue = value["sprite"][i++];
				}

				if (dirty)
				{
					Json::StyledStreamWriter writer;
					std::ofstream fout(filepath.fn_str());
					writer.write(fout, value);
					fout.close();
				}
			}
		}
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

//}