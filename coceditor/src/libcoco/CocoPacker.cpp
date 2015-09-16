#include "CocoPacker.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "tools.h"

#include "PackToLuaString.h"
#include "PackToBin.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyparticle3d.h>

namespace libcoco
{

CocoPacker::CocoPacker(const std::string& json_dir, const std::string& tp_name, 
					   const std::string& tp_dir)
	: m_tp(tp_dir)
{
	d2d::TextureFactory::Instance()->InitTexturePacker(tp_dir);

	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	bool old_cfg = data.load_image;
	data.load_image = false;

	PackNodeFactory::Instance()->SetFilesDir(json_dir);

	LoadTPData(tp_name);
	LoadJsonData(json_dir);

	Pack();

	data.load_image = old_cfg;
}

void CocoPacker::OutputLua(const std::string& outfile) const
{
	PackToLuaString::Pack(outfile, m_tp);
}

void CocoPacker::OutputEpe(const std::string& outfile, bool compress) const
{
	PackToBin::PackEPE(outfile + ".epe", m_tp, compress);
}

void CocoPacker::OutputEpt(const std::string& outfile, TextureType type) const
{
	PackToBin::PackEPT(outfile, m_tp, type);
}

void CocoPacker::LoadJsonData(const std::string& dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);

	std::vector<std::string> filepaths;
	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex) || 
			d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim) ||
			d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_particle3d)) {
			filepaths.push_back(filepath);
		}
	}

	std::sort(filepaths.begin(), filepaths.end());
	for (int i = 0, n = filepaths.size(); i < n; ++i) {
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepaths[i]);
		m_symbols.push_back(symbol);
	}
}

void CocoPacker::LoadTPData(const std::string& tp_name)
{
	int i = 1;
	while (true) {
		std::string tp_path = tp_name + d2d::StringTools::ToString(i) + ".json";
		if (d2d::FilenameTools::IsFileExist(tp_path)) {
			m_tp.Add(tp_path);
			d2d::TextureFactory::Instance()->AddTextureFromConfig(tp_path);
		} else {
			break;
		}
		++i;
	}
}

void CocoPacker::Pack() const
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	for (int i = 0, n = m_symbols.size(); i < n; ++i) 
	{
		const d2d::ISymbol* symbol = m_symbols[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol)) {
			factory->Create(complex);
		} else if (const libanim::Symbol* anim = dynamic_cast<const libanim::Symbol*>(symbol)) {
			factory->Create(anim);
		} else if (const eparticle3d::Symbol* p3d = dynamic_cast<const eparticle3d::Symbol*>(symbol)) {
			factory->Create(p3d);
		} else {
			throw d2d::Exception("CocoPacker::Pack unhandled type.");
		}
	}
}

}
