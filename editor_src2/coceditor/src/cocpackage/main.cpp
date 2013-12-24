#include "../coceditor/Context.h"
#include "../coceditor/COCCode.h"

#include <iostream>
#include <wx/dir.h>
#include <drag2d.h>
#include <easybuilder.h>

#define CHARACTER

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

int main(int argc, char *argv[])
{
	if (argc < 4) {
		std::cerr << "Need Resource Dir & Texture Packer Filepath & Output filepath !" << std::endl;
		return 1;
	}

	if (argc > 4)
	{
		double scale;
		wxString(argv[4]).ToDouble(&scale);
		coceditor::Context::Instance()->scale = scale;
	}

	std::string dirpath = argv[1];
	wxArrayString files;
	DirTraverser traverser(files);

	wxDir dir(dirpath);
	dir.Traverse(traverser);

	try {
		for (size_t i = 0, n = files.size(); i < n; ++i)
		{
			wxFileName filename(files[i]);
			filename.Normalize();
			wxString filepath = filename.GetFullPath();
			
			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)
				|| d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
			{
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
				coceditor::Context::Instance()->symbols.push_back(symbol);
			}
		}
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}

	std::string texpackerpath = argv[2];
	size_t i = 1;
	while (true)
	{
		std::string path = texpackerpath + wxString::FromDouble(i) + ".json";
		if (wxFileName::FileExists(path))
			coceditor::Context::Instance()->texMgr.initTexture(path, i-1);
		else
			break;
		++i;
	}

	ebuilder::CodeGenerator gen;
	coceditor::COCCode code(gen);
	code.resolve();
	std::ofstream fout(argv[3]);
	fout << gen.toText() << std::endl;
	fout.close();

	return 0;
}