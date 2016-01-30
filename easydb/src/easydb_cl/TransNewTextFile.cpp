#include "TransNewTextFile.h"
#include "check_params.h"

#include <easycomplex.h>
#include <easytext.h>

namespace edb
{

std::string TransNewTextFile::Command() const
{
	return "trans-new-text";
}

std::string TransNewTextFile::Description() const
{
	return "trans new text file";
}

std::string TransNewTextFile::Usage() const
{
	// trans-old-text dir
	std::string usage = Command() + " [folder]";
	return usage;
}

void TransNewTextFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Run(argv[2]);
}

void TransNewTextFile::Run(const std::string& folder)
{
	wxArrayString files;
	d2d::FileHelper::FetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (d2d::FileType::IsType(filepath, d2d::FileType::e_complex)) {
			d2d::Symbol* sym = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			if (ecomplex::Symbol* complex = dynamic_cast<ecomplex::Symbol*>(sym)) {
				bool dirty = false;
				for (int i = 0, n = complex->m_sprites.size(); i < n; ++i) {
					if (etext::Sprite* text = dynamic_cast<etext::Sprite*>(complex->m_sprites[i])) {
						dirty = true;
						text->SetFontSize(text->GetFontSize() + 8);
					}
				}
				if (dirty) {
					ecomplex::FileStorer::Store(filepath.c_str(), complex);
				}
			}
			sym->Release();
		}
	}
}

}