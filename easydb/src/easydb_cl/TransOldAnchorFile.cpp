#include "TransOldAnchorFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/FontBlankSprite.h>

#include <easycomplex.h>
#include <easyanim.h>

namespace edb
{

std::string TransOldAnchorFile::Command() const
{
	return "trans-old-anchor";
}

std::string TransOldAnchorFile::Description() const
{
	return "trans old anchor file";
}

std::string TransOldAnchorFile::Usage() const
{
	// trans-old-anchor dir
	std::string usage = Command() + " [folder]";
	return usage;
}

int TransOldAnchorFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void TransOldAnchorFile::Run(const std::string& folder)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			TransComplex(filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			TransAnimation(filepath);
		}
	}
}

void TransOldAnchorFile::TransComplex(const std::string& filepath) const
{
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	ecomplex::Symbol* complex = static_cast<ecomplex::Symbol*>(sym);
	bool dirty = false;
	for (int i = 0, n = complex->m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = complex->m_sprites[i];
		if (IsAnchor(spr)) {
			spr->SetAnchor(true);
			dirty = true;
		}
	}
	if (dirty) {
		ecomplex::FileStorer::Store(filepath.c_str(), complex);
	}
	sym->Release();
}

void TransOldAnchorFile::TransAnimation(const std::string& filepath) const
{
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	eanim::Symbol* anim = static_cast<eanim::Symbol*>(sym);
	bool dirty = false;
	for (int i = 0, n = anim->m_layers.size(); i < n; ++i) {
		eanim::Symbol::Layer* layer = anim->m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			eanim::Symbol::Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprites.size(); k < l; ++k) {
				ee::Sprite* spr = frame->sprites[k];
				if (IsAnchor(spr)) {
					spr->SetAnchor(true);
					dirty = true;
				}
			}
		}
	}
	if (dirty) {
		eanim::FileSaver::Store(filepath, *anim);
	}
	sym->Release();
}

bool TransOldAnchorFile::IsAnchor(const ee::Sprite* spr) const
{	
	if (const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(spr)) {
		return font->font.empty() && font->font_color == ee::Colorf(0, 0, 0, 0);
	} else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(spr)) {
		if (complex->GetSymbol().m_sprites.size() == 1) {
			return IsAnchor(complex->GetSymbol().m_sprites[0]);
		} else {
			return false;
		}
	}
	return false;
}

}