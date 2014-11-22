#include "CocoPacker.h"
#include "item_string.h"

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

	// duration
	int duration = val[ITEM_DURATION_TIME].asInt();
	if (duration == 0) {
		lua::assign_with_end(gen, "duration", -1);
	} else {
		lua::assign_with_end(gen, "duration", duration);
	}

	// positionType

	// direction
	lua::assign_with_end(gen, "angle", val[ITEM_DIRECTION][ITEM_ATTR_CENTER].asInt());
	lua::assign_with_end(gen, "angleVariance", val[ITEM_DIRECTION][ITEM_ATTR_OFFSET].asInt());

	// blend (no use)
	lua::assign_with_end(gen, "blendFuncDestination", 771);
	lua::assign_with_end(gen, "blendFuncSource", 1);

	// name
	lua::assign_with_end(gen, "configName", val["name"].asString());

	// static cfg
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
	symbol->Release();
	float size_start = width * val[ITEM_SCALE][ITEM_ATTR_START].asInt() * 0.01f;
	float size_finish = width * val[ITEM_SCALE][ITEM_ATTR_END].asInt() * 0.01f;
	lua::assign_with_end(gen, "size", width);
	lua::assign_with_end(gen, "startParticleSize", size_start);
	lua::assign_with_end(gen, "startParticleSizeVariance", 0);
	lua::assign_with_end(gen, "finishParticleSize", size_finish);
	lua::assign_with_end(gen, "finishParticleSizeVariance", 0);

	// gravity
	lua::assign_with_end(gen, "gravityx", 0);
	lua::assign_with_end(gen, "gravityy", val[ITEM_GRAVITY][ITEM_ATTR_CENTER].asInt());
	lua::assign_with_end(gen, "radialAcceleration", val[ITEM_RADIAL_ACC][ITEM_ATTR_CENTER].asInt());
	lua::assign_with_end(gen, "radialAccelVariance", val[ITEM_RADIAL_ACC][ITEM_ATTR_OFFSET].asInt());
	lua::assign_with_end(gen, "tangentialAcceleration", val[ITEM_TANGENTIAL_ACC][ITEM_ATTR_CENTER].asInt());
	lua::assign_with_end(gen, "tangentialAccelVariance", val[ITEM_TANGENTIAL_ACC][ITEM_ATTR_OFFSET].asInt());

	// count
	int count = val[ITEM_COUNT].asInt();
	int times = val[ITEM_LIFE][ITEM_ATTR_CENTER].asInt() / val[ITEM_EMISSION_TIME].asInt();
	count *= times;
	lua::assign_with_end(gen, "maxParticles", count);

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
	lua::assign_with_end(gen, "sourcePositionVariancex", val[ITEM_POSITION][ITEM_ATTR_X].asInt());
	lua::assign_with_end(gen, "sourcePositionVariancey", val[ITEM_POSITION][ITEM_ATTR_Y].asInt());

	// speed
	lua::assign_with_end(gen, "speed", val[ITEM_SPEED][ITEM_ATTR_CENTER].asInt());
	lua::assign_with_end(gen, "speedVariance", val[ITEM_SPEED][ITEM_ATTR_OFFSET].asInt());

	// life
	lua::assign_with_end(gen, "particleLifespan", val[ITEM_LIFE][ITEM_ATTR_CENTER].asInt() * 0.001f);
	lua::assign_with_end(gen, "particleLifespanVariance", val[ITEM_LIFE][ITEM_ATTR_OFFSET].asInt() * 0.001f);

	// cos
	lua::assign_with_end(gen, "cosAmplitude", val[ITEM_COS][ITEM_COS_AMPLITUDE][ITEM_ATTR_CENTER].asInt());
	lua::assign_with_end(gen, "cosAmplitudeVariance", val[ITEM_COS][ITEM_COS_AMPLITUDE][ITEM_ATTR_OFFSET].asInt());
	lua::assign_with_end(gen, "cosFrequency", val[ITEM_COS][ITEM_COS_FREQUENCY][ITEM_ATTR_CENTER].asInt() * 0.01f);
	lua::assign_with_end(gen, "cosFrequencyVariance", val[ITEM_COS][ITEM_COS_FREQUENCY][ITEM_ATTR_OFFSET].asInt() * 0.01f);

	// other
	lua::assign_with_end(gen, "yCoordFlipped", -1);
}

}