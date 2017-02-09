#include "TransNewTextFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>
#include <easytext.h>

#include <sprite2/S2_Sprite.h>
#include <sprite2/SymType.h>

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

int TransNewTextFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void TransNewTextFile::Run(const std::string& folder)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_COMPLEX) 
		{
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			if (ecomplex::Symbol* complex = dynamic_cast<ecomplex::Symbol*>(sym)) 
			{
				bool dirty = false;
				const std::vector<s2::Sprite*>& children = complex->GetAllChildren();
				for (int i = 0, n = children.size(); i < n; ++i) 
				{
					s2::Sprite* child = children[i];
					if (etext::Sprite* text = dynamic_cast<etext::Sprite*>(child)) 
					{
						dirty = true;
						s2::Textbox& tb = text->GetTextbox();
						tb.font_size = tb.font_size + 8;
					}
				}
				if (dirty) {
					ecomplex::FileStorer::Store(filepath, complex, ee::FileHelper::GetFileDir(filepath));
				}
			}
			sym->RemoveReference();
		}
	}
}

}