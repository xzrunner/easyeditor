#include "CocoPacker.h"

#include <drag2d.h>
#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace eparticle2d
{

void CocoPacker::pack(const wxString& srcdir, const wxString& dstfilename)
{
	ebuilder::CodeGenerator gen;

	gen.line("return {");
	gen.tab();

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(srcdir.ToStdString(), files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_particle2d))
		{
			pack(filepath, gen);
		}
	}

	gen.detab();
	gen.line("}");

	std::locale::global(std::locale(""));
	std::ofstream fout(dstfilename.fn_str());
	std::locale::global(std::locale("C"));
	if (!fout.fail())
	{
		fout << gen.toText();
		fout.close();
	}
}

void CocoPacker::pack(const wxString& filepath, ebuilder::CodeGenerator& gen)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string s = val["name"].asString();

	gen.line(s + " = {");
	gen.tab();

	wxString dir = d2d::FilenameTools::getFileDir(filepath) + "\\";
	pack(val, gen, dir);

	gen.detab();
	gen.line("},");
}

void CocoPacker::pack(const Json::Value& val, ebuilder::CodeGenerator& gen, const wxString& dir)
{
	lua::assign_with_end(gen, "texId", 0); // no use

	// positionType

	// direction
	lua::assign_with_end(gen, "angle", val["direction"]["center"].asInt());
	lua::assign_with_end(gen, "angleVariance", val["direction"]["offset"].asInt());

	// blend (no use)
	lua::assign_with_end(gen, "blendFuncDestination", 771);
	lua::assign_with_end(gen, "blendFuncSource", 1);

	// name
	lua::assign_with_end(gen, "configName", val["name"].asString());

	// static cfg
	lua::assign_with_end(gen, "duration", -1);	// Ð´ËÀ-1£¬ÎÞÏÞ
	lua::assign_with_end(gen, "emitterType", 0);	

	// color
	lua::assign_with_end(gen, "startColorRed", 0);
	lua::assign_with_end(gen, "startColorVarianceRed", 0);
	lua::assign_with_end(gen, "startColorGreen", 0);
	lua::assign_with_end(gen, "startColorVarianceGreen", 0);
	lua::assign_with_end(gen, "startColorBlue", 0);
	lua::assign_with_end(gen, "startColorVarianceBlue", 0);
	lua::assign_with_end(gen, "startColorAlpha", 1);
	lua::assign_with_end(gen, "startColorVarianceAlpha", 0);
	lua::assign_with_end(gen, "finishColorRed", 0);
	lua::assign_with_end(gen, "finishColorVarianceRed", 0);
	lua::assign_with_end(gen, "finishColorGreen", 0);
	lua::assign_with_end(gen, "finishColorVarianceGreen", 0);
	lua::assign_with_end(gen, "finishColorBlue", 0);
	lua::assign_with_end(gen, "finishColorVarianceBlue", 0);
	lua::assign_with_end(gen, "finishColorAlpha", 1);
	lua::assign_with_end(gen, "finishColorVarianceAlpha", 0);

	// scale
	wxString path = d2d::FilenameTools::getAbsolutePath(dir, val["symbol_path"].asString());
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
	float width = symbol->getSize().xLength();
	symbol->release();
	float size_start = width * val["scale"]["start"].asInt() * 0.01f;
	float size_finish = width * val["scale"]["end"].asInt() * 0.01f;
	lua::assign_with_end(gen, "startParticleSize", size_start);
	lua::assign_with_end(gen, "startParticleSizeVariance", 0);
	lua::assign_with_end(gen, "finishParticleSize", size_finish);
	lua::assign_with_end(gen, "finishParticleSizeVariance", 0);

	// gravity
	lua::assign_with_end(gen, "gravityx", 0);
	lua::assign_with_end(gen, "gravityy", val["gravity"]["center"].asInt());
	lua::assign_with_end(gen, "radialAcceleration", val["radial_acc"]["center"].asInt());
	lua::assign_with_end(gen, "radialAccelVariance", val["radial_acc"]["offset"].asInt());
	lua::assign_with_end(gen, "tangentialAcceleration", val["tangential_acc"]["center"].asInt());
	lua::assign_with_end(gen, "tangentialAccelVariance", val["tangential_acc"]["offset"].asInt());

	// count
	lua::assign_with_end(gen, "maxParticles", val["count"].asInt());

	// radius (no use, another mode)
	lua::assign_with_end(gen, "maxRadius", 0);
	lua::assign_with_end(gen, "maxRadiusVariance", 0);
	lua::assign_with_end(gen, "minRadius", 0);
	lua::assign_with_end(gen, "minRadiusVariance", 0);

	// rotate
	lua::assign_with_end(gen, "rotatePerSecond", 360);
	lua::assign_with_end(gen, "rotatePerSecondVariance", 0);
	lua::assign_with_end(gen, "rotationStart", 0);
	lua::assign_with_end(gen, "rotationStartVariance", 0);
	lua::assign_with_end(gen, "rotationEnd", 0);
	lua::assign_with_end(gen, "rotationEndVariance", 0);

	// position
	lua::assign_with_end(gen, "sourcePositionx", 0);
	lua::assign_with_end(gen, "sourcePositiony", 0);
	lua::assign_with_end(gen, "sourcePositionVariancex", val["position"]["x"].asInt());
	lua::assign_with_end(gen, "sourcePositionVariancey", val["position"]["y"].asInt());

	// speed
	lua::assign_with_end(gen, "speed", val["speed"]["center"].asInt());
	lua::assign_with_end(gen, "speedVariance", val["speed"]["offset"].asInt());

	// life
	lua::assign_with_end(gen, "particleLifespan", val["emission_time"].asInt() * 0.001f);
	lua::assign_with_end(gen, "particleLifespanVariance", 0);

	// cos
	lua::assign_with_end(gen, "cosAmplitude", val["cos"]["amplitude"]["center"].asInt());
	lua::assign_with_end(gen, "cosAmplitudeVariance", val["cos"]["amplitude"]["offset"].asInt());
	lua::assign_with_end(gen, "cosFrequency", val["cos"]["frequency"]["center"].asInt() * 0.01f);
	lua::assign_with_end(gen, "cosFrequencyVariance", val["cos"]["frequency"]["offset"].asInt() * 0.01f);

	// other
	lua::assign_with_end(gen, "yCoordFlipped", -1);
}

}