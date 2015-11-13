#include "TransOldAnchorFile.h"
#include "check_params.h"

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

void TransOldAnchorFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Run(argv[2]);
}

void TransOldAnchorFile::Run(const std::string& folder)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			TransComplex(filepath);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			TransAnimation(filepath);
		}
	}
}

void TransOldAnchorFile::TransComplex(const std::string& filepath) const
{
	d2d::ISymbol* sym = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	ecomplex::Symbol* complex = static_cast<ecomplex::Symbol*>(sym);
	bool dirty = false;
	for (int i = 0, n = complex->m_sprites.size(); i < n; ++i) {
		d2d::ISprite* spr = complex->m_sprites[i];
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
	d2d::ISymbol* sym = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	libanim::Symbol* anim = static_cast<libanim::Symbol*>(sym);
	bool dirty = false;
	for (int i = 0, n = anim->m_layers.size(); i < n; ++i) {
		libanim::Symbol::Layer* layer = anim->m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			libanim::Symbol::Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprites.size(); k < l; ++k) {
				d2d::ISprite* spr = frame->sprites[k];
				if (IsAnchor(spr)) {
					spr->SetAnchor(true);
					dirty = true;
				}
			}
		}
	}
	if (dirty) {
		libanim::FileSaver::store(filepath, *anim);
	}
	sym->Release();
}

bool TransOldAnchorFile::IsAnchor(const d2d::ISprite* spr) const
{	
	if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(spr)) {
		return font->font.empty() && font->color == d2d::Colorf(0, 0, 0, 0);
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